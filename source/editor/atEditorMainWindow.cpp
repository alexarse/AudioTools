/*
 * Copyright (c) 2016 AudioTools - All Rights Reserved
 *
 * This Software may not be distributed in parts or its entirety
 * without prior written agreement by AudioTools.
 *
 * Neither the name of the AudioTools nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY AUDIOTOOLS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL AUDIOTOOLS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Written by Alexandre Arsenault <alx.arsenault@gmail.com>
 */

#include "editor/atEditorMainWindow.hpp"
#include "editor/GlobalEvents.hpp"

#include <axlib/Knob.hpp>
#include <axlib/Label.hpp>
#include <axlib/Panel.hpp>
#include <axlib/ScrollBar.hpp>
#include <axlib/Slider.hpp>
#include <axlib/WidgetLoader.hpp>
#include <axlib/WindowManager.hpp>
#include <axlib/WindowTree.hpp>

#include <boost/filesystem.hpp>

#include "PyoAudio.h"
#include "atCommon.hpp"
#include "atHelpBar.h"
#include "editor/atEditorLoader.hpp"

#include "dialog/atSaveWorkDialog.hpp"

#include <time.h>

namespace at {
namespace editor {
	MainWindow::MainWindow(const ax::Rect& rect, const std::string& proj_path)
		: _font(0)
		, _view_handler(this)
		, _widget_handler(this)
		, _project_handler(this)
	{
		// Create window.
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &MainWindow::OnPaint);
		win->event.OnPaintOverChildren = ax::WBind<ax::GC>(this, &MainWindow::OnPaintOverChildren);
		win->event.OnResize = ax::WBind<ax::Size>(&_view_handler, &MainWindowViewHandler::OnResize);
		win->event.OnKeyDown = ax::WBind<char>(this, &MainWindow::OnGlobalKey);
		win->event.OnAssignToWindowManager = ax::WBind<int>(this, &MainWindow::OnAssignToWindowManager);

		win->AddConnection(999, GetOnHelpBar());

		_font.SetFontSize(10);

		// Create top menu.
		ax::Rect top_menu_rect(0, 0, rect.size.w, STATUS_BAR_HEIGHT);
		_statusBar = new StatusBar(top_menu_rect);
		win->node.Add(std::shared_ptr<ax::Window::Backbone>(_statusBar));

		if (!proj_path.empty()) {
			_project.Open(proj_path);
			_statusBar->SetLayoutFilePath(_project.GetLayoutPath());
		}
		else {
			_statusBar->SetLayoutFilePath("default.xml");
		}

		ax::Window* sb_win = _statusBar->GetWindow();
		sb_win->AddConnection(StatusBar::SAVE_LAYOUT, _project_handler.GetOnSaveProject());
		sb_win->AddConnection(StatusBar::SAVE_AS_LAYOUT, _project_handler.GetOnSaveAsProject());
		sb_win->AddConnection(StatusBar::OPEN_LAYOUT, _project_handler.GetOnOpenProject());
		sb_win->AddConnection(StatusBar::CREATE_NEW_LAYOUT, _project_handler.GetOnCreateNewProject());

		sb_win->AddConnection(StatusBar::RELOAD_SCRIPT, GetOnReloadScript());
		sb_win->AddConnection(StatusBar::STOP_SCRIPT, GetOnStopScript());

		sb_win->AddConnection(StatusBar::TOGGLE_LEFT_PANEL, _view_handler.GetOnToggleLeftPanel());
		sb_win->AddConnection(StatusBar::TOGGLE_BOTTOM_PANEL, _view_handler.GetOnToggleBottomPanel());
		sb_win->AddConnection(StatusBar::TOGGLE_RIGHT_PANEL, _view_handler.GetOnToggleRightPanel());

		sb_win->AddConnection(StatusBar::VIEW_LAYOUT, _view_handler.GetOnViewLayout());

		// Events from MainWindow to status bar.
		win->AddConnection(HAS_WIDGET_ON_GRID, _statusBar->GetOnHasWidgetOnGrid());

		// Create grid window.
		ax::Rect grid_rect(WIDGET_MENU_WIDTH, STATUS_BAR_HEIGHT,
			rect.size.w - WIDGET_MENU_WIDTH - INSPECTOR_MENU_WIDTH,
			rect.size.h - STATUS_BAR_HEIGHT - 200 - BOTTOM_BAR_HEIGHT);
		win->node.Add(_gridWindow = ax::shared<GridWindow>(grid_rect));

		_gridWindow->GetWindow()->AddConnection(
			GridWindow::SELECT_WIDGET, _widget_handler.GetOnSelectWidget());
		_gridWindow->GetWindow()->AddConnection(
			GridWindow::UNSELECT_ALL, _widget_handler.GetOnUnSelectAllWidget());
		_gridWindow->GetWindow()->AddConnection(
			GridWindow::SAVE_PANEL_TO_WORKSPACE, GetOnSavePanelToWorkspace());

		_gridWindow->GetWindow()->AddConnection(
			GridWindow::DELETE_SELECTED_WIDGET, _widget_handler.GetOnDeleteSelectedWidget());

		_gridWindow->GetWindow()->AddConnection(
			GridWindow::DUPLICATE_SELECTED_WIDGET, _widget_handler.GetOnDuplicateSelectedWidget());

		_gridWindow->GetWindow()->AddConnection(
			GridWindow::DELETE_SELECTED_WIDGET_FROM_RIGHT_CLICK, GetOnRemoveWidgetFromRightClickMenu());

		_gridWindow->GetWindow()->AddConnection(
			GridWindow::DUPLICATE_SELECTED_WIDGET_FROM_RIGHT_CLICK, GetOnDuplicateWidgetFromRightClickMenu());

		_gridWindow->GetWindow()->AddConnection(
			GridWindow::SNAP_WIDGET_TO_GRID_FROM_RIGHT_CLICK, GetOnSnapToGridWidgetFromRightClickMenu());

		_gridWindow->GetWindow()->AddConnection(
			GridWindow::SELECT_MULTIPLE_WIDGET, _widget_handler.GetOnSelectMultipleWidget());

		_gridWindow->GetWindow()->AddConnection(
			GridWindow::ARROW_MOVE_SELECTED_WIDGET, _widget_handler.GetOnArrowMoveSelectedWidget());

		if (!proj_path.empty()) {
			_gridWindow->OpenLayout(_project.GetLayoutPath());
		}
		else {
			_gridWindow->OpenLayout("layouts/default.xml");
		}

		// Create widget menu.
		ax::Rect widget_menu_rect(
			0, STATUS_BAR_HEIGHT, WIDGET_MENU_WIDTH, rect.size.h - STATUS_BAR_HEIGHT - BOTTOM_BAR_HEIGHT);

		auto l_side_menu = ax::shared<LeftSideMenu>(widget_menu_rect);
		win->node.Add(l_side_menu);
		_left_menu = l_side_menu.get();

		l_side_menu->GetWindow()->AddConnection(
			WidgetMenu::SMALLER_MENU, _view_handler.GetOnSmallerLeftMenu());

		// Create info menu.
		ax::Rect info_rect(rect.size.w - INSPECTOR_MENU_WIDTH, STATUS_BAR_HEIGHT, INSPECTOR_MENU_WIDTH,
			rect.size.h - STATUS_BAR_HEIGHT - BOTTOM_BAR_HEIGHT);

		auto right_menu = ax::shared<RightSideMenu>(info_rect);
		win->node.Add(right_menu);
		_right_menu = right_menu.get();

		// Create code editor.
		TextEditor::Info txt_info;
		txt_info.bg_color = ax::Color(1.0);
		txt_info.cursor_color = ax::Color(0.0);
		txt_info.line_number_bg_color = ax::Color(0.95);
		txt_info.line_number_color = ax::Color(0.4);
		txt_info.text_color = ax::Color(0.0);

		ax::Rect bottom_rect(WIDGET_MENU_WIDTH + 1, rect.size.h - 200 - BOTTOM_BAR_HEIGHT,
			rect.size.w - WIDGET_MENU_WIDTH - INSPECTOR_MENU_WIDTH, 200);

		std::string script_path;

		if (!proj_path.empty()) {
			script_path = _project.GetScriptPath();
		}
		else {
			script_path = "scripts/default.py";
		}

		auto b_section = ax::shared<BottomSection>(bottom_rect, script_path);
		win->node.Add(b_section);
		_bottom_section = b_section.get();
		_bottom_section->GetWindow()->AddConnection(
			BottomSection::RESIZE, _view_handler.GetOnResizeCodeEditor());

		_bottom_section->GetWindow()->AddConnection(10020, ax::event::Function([&](ax::event::Msg* msg) {
			ax::console::Print("Save");
			std::vector<std::shared_ptr<ax::Window>>& children = _gridWindow->GetWindow()->node.GetChildren();

			for (auto& n : children) {
				n->Update();
			}
		}));

		win->AddConnection(global::CREATE_DRAGGING_WIDGET, _widget_handler.GetOnCreateDraggingWidget());
		win->AddConnection(global::DRAGGING_WIDGET, _widget_handler.GetOnDraggingWidget());
		win->AddConnection(global::RELEASE_WIDGET, _widget_handler.GetOnReleaseObjWidget());
		win->AddConnection(
			global::CREATE_CUSTOM_DRAGGING_WIDGET, _widget_handler.GetOnCreateCustomDraggingWidget());

		// Midi feedback.
		auto midi_feedback = ax::shared<at::MidiFeedback>(
			ax::Rect(ax::Point(rect.size.w - 17, rect.size.h - 15), ax::Size(10, 10)));
		_midi_feedback = midi_feedback.get();

		win->node.Add(midi_feedback);
	}

	void MainWindow::OnAssignToWindowManager(const int& v)
	{
		//		win->event.GrabGlobalMouse();
		win->event.GrabGlobalKey();
	}

	std::vector<ax::Window*> MainWindow::GetSelectedWindows() const
	{
		return _selected_windows;
	}

	ax::Window* MainWindow::GetWidgetsByName(const std::string& name)
	{
		return _gridWindow->GetWidgetByName(name);
	}

	//	void MainWindow::DeleteCurrentWidgets()
	//	{
	//		// @todo Remove multiple widgets.
	//		if (_selected_windows.size()) {
	//
	//			auto& children = _selected_windows[0]->node.GetParent()->node.GetChildren();
	//			ax::Window::Ptr current_win;
	//
	//			int index = -1;
	//
	//			for (int i = 0; i < children.size(); i++) {
	//				if (children[i]->GetId() == _selected_windows[0]->GetId()) {
	//					current_win = children[i];
	//					index = i;
	//					break;
	//				}
	//			}
	//
	//			if (current_win && index != -1) {
	//				win->event.UnGrabMouse();
	//				ax::App::GetInstance().GetWindowManager()->ReleaseMouseHover();
	//				children.erase(children.begin() + index);
	//			}
	//		}
	//
	//		_selected_windows.clear();
	//		_right_menu->RemoveInspectorHandle();
	//
	//		if (_gridWindow->GetMainWindow() == nullptr) {
	//			_left_menu->SetOnlyMainWindowWidgetSelectable();
	//		}
	//	}

	void MainWindow::OnSavePanelToWorkspace(const ax::event::EmptyMsg& msg)
	{
		ax::App::GetInstance().GetPopupManager()->Clear();
		_need_to_save_widget_img_on_paint = true;
		win->Update();

		//		if(_selected_windows.size() == 1) {
		//			ax::Window* w = _selected_windows[0];
		//			unsigned char* pdata = nullptr;
		//			ax::Rect rect = w->GetWindowPixelData(pdata);
		//
		//			ax::Image img((void*)pdata, rect.size);
		//			img.SaveImage("/Users/alexarse/Desktop/test.png");
		//			delete[] pdata;
		//		}

		//		// Empty popup window tree.
		//		ax::App& app(ax::App::GetInstance());
		//		app.GetPopupManager()->Clear();
		//		//		app.GetPopupManager()->SetPastKeyWindow(nullptr);
		//		//		app.GetPopupManager()->SetPastWindow(nullptr);
		//		//		app.GetPopupManager()->SetScrollCaptureWindow(nullptr);
		//		//		app.GetPopupManager()->GetWindowTree()->GetNodeVector().clear();
		//
		//		ax::Point pos(0, STATUS_BAR_HEIGHT - 1);
		//
		//		ax::Size size = ax::App::GetInstance().GetFrameSize();
		//		size.h -= (STATUS_BAR_HEIGHT + BOTTOM_BAR_HEIGHT - 1);
		//
		//		auto pref_dialog = ax::shared<at::SaveWorkDialog>(ax::Rect(pos, size));
		//		ax::App::GetInstance().GetPopupManager()->GetWindowTree()->AddTopLevel(
		//			std::shared_ptr<ax::Window>(pref_dialog->GetWindow()));
		//
		//		pref_dialog->GetWindow()->backbone = pref_dialog;
		//
		//		pref_dialog->GetWindow()->AddConnection(at::SaveWorkPanel::SAVE,
		// GetOnAcceptSavePanelToWorkpace());
		//		pref_dialog->GetWindow()->AddConnection(at::SaveWorkPanel::CANCEL,
		// GetOnCancelSavePanelToWorkpace());
	}

	void MainWindow::OnAcceptSavePanelToWorkpace(const at::SaveWorkPanel::Msg& msg)
	{
	}

	void MainWindow::OnCancelSavePanelToWorkpace(const ax::event::EmptyMsg& msg)
	{
	}

	void MainWindow::OnRemoveWidgetFromRightClickMenu(const ax::event::EmptyMsg& msg)
	{
		// Empty popup window tree.
		//		ax::App& app(ax::App::GetInstance());
		ax::App::GetInstance().GetPopupManager()->Clear();
		//		app.GetPopupManager()->UnGrabKey();
		//		app.GetPopupManager()->UnGrabMouse();
		//		app.GetPopupManager()->SetPastKeyWindow(nullptr);
		//		app.GetPopupManager()->SetPastWindow(nullptr);
		//		app.GetPopupManager()->SetScrollCaptureWindow(nullptr);
		//		app.GetPopupManager()->GetWindowTree()->GetNodeVector().clear();

		_widget_handler.DeleteCurrentWidgets();
	}

	void MainWindow::OnDuplicateWidgetFromRightClickMenu(const ax::event::EmptyMsg& msg)
	{
		// Empty popup window tree.
		//		ax::App& app(ax::App::GetInstance());
		ax::App::GetInstance().GetPopupManager()->Clear();
		//		app.GetPopupManager()->UnGrabKey();
		//		app.GetPopupManager()->UnGrabMouse();
		//		app.GetPopupManager()->SetPastKeyWindow(nullptr);
		//		app.GetPopupManager()->SetPastWindow(nullptr);
		//		app.GetPopupManager()->SetScrollCaptureWindow(nullptr);
		//		app.GetPopupManager()->GetWindowTree()->GetNodeVector().clear();

		_widget_handler.OnDuplicateSelectedWidget(msg);
	}

	void MainWindow::OnSnapToGridWidgetFromRightClickMenu(const ax::event::EmptyMsg& msg)
	{
		ax::App::GetInstance().GetPopupManager()->Clear();

		if (!_selected_windows.size()) {
			return;
		}

		for (auto& n : _selected_windows) {
			ax::Window* wwin = n;
			GridSnapProxy gsp = GetGridSnapProxy();

			const ax::Rect gw_abs_rect(_gridWindow->GetWindow()->dimension.GetAbsoluteRect());
			ax::Point pos
				= gsp.FindClosestPosition(wwin->dimension.GetAbsoluteRect().position - gw_abs_rect.position);
			pos += gw_abs_rect.position;
			wwin->dimension.SetPosition(pos - wwin->node.GetParent()->dimension.GetAbsoluteRect().position);
		}
	}

	void MainWindow::OnHelpBar(const ax::event::StringMsg& msg)
	{
		_help_bar_str = msg.GetMsg();
		win->Update();
	}

	void MainWindow::OnReloadScript(const ax::event::SimpleMsg<int>& msg)
	{
		ax::console::Print("Reload script");

		/// @todo Do this in another thread and add a feedback to user somehow.
		//----------------------------------------------------------------------
		//		_codeEditor->SaveFile(_codeEditor->GetScriptPath());
		_bottom_section->SaveFile(_bottom_section->GetScriptPath());
		PyoAudio::GetInstance()->ReloadScript(_bottom_section->GetScriptPath());
		//----------------------------------------------------------------------
	}

	void MainWindow::OnStopScript(const ax::event::SimpleMsg<int>& msg)
	{
		PyoAudio::GetInstance()->StopServer();
	}

	void MainWindow::OnGlobalKey(const char& c)
	{
		ax::console::Print("MainWindow global key");

		if (!ax::App::GetInstance().GetWindowManager()->IsCmdDown()) {
			return;
		}

		if (c == 's' || c == 'S') {
			// Save current project.
			_project_handler.SaveCurrentProject();
		}
	}

	void MainWindow::OnPaintOverChildren(ax::GC gc)
	{
		if (_need_to_save_widget_img_on_paint == false) {
			return;
		}

		_need_to_save_widget_img_on_paint = false;

		if (_selected_windows.size() != 1) {
			return;
		}
		ax::Window* w = _selected_windows[0];

		if (!w->component.Has("Widget")) {
			return;
		}

		unsigned char* pdata = nullptr;
		ax::Rect rect = w->GetWindowPixelData(pdata);
		ax::Image img((void*)pdata, rect.size);

		time_t rawtime;
		struct tm* timeinfo;
		char buffer[80];
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(buffer, 80, "%Y_%m_%d_%H_%M_%S", timeinfo);
		const std::string time_str(buffer, strlen(buffer));

		const std::string img_path("custom_widgets_menu_images/" + time_str + ".png");
		img.SaveImage(img_path);
		delete[] pdata;

		// Callback for saving widget with child widgets in them.
		std::function<void(ax::Xml&, ax::Xml::Node&, ax::Window*)> panel_save_child
			= [&](ax::Xml& xml, ax::Xml::Node& node, ax::Window* child_win) {

				  ax::widget::Component::Ptr opt = child_win->component.Get<ax::widget::Component>("Widget");

				  if (opt) {
					  if (child_win->property.HasProperty("AcceptWidget")) {
						  opt->SetSaveChildCallback(panel_save_child);
					  }
					  // Save ax::Object.
					  opt->Save(xml, node);
				  }
			  };

		ax::widget::Component::Ptr wcomp = w->component.Get<ax::widget::Component>("Widget");
		if (w->property.HasProperty("AcceptWidget")) {
			wcomp->SetSaveChildCallback(panel_save_child);
		}

		ax::Xml xml;
		ax::Xml::Node custom_widget_node = xml.CreateNode("CustomWidget");
		xml.AddMainNode(custom_widget_node);

		// Add size attribute.
		const ax::Size wsize = w->dimension.GetSize();
		custom_widget_node.AddAttribute(
			"size", "(" + std::to_string(wsize.w) + " x " + std::to_string(wsize.h) + ")");

		// Add menu img path.
		custom_widget_node.AddAttribute("img", img_path);
		custom_widget_node.AddAttribute("name", "No name");
		custom_widget_node.AddAttribute("description", "No description");

		ax::Xml::Node wnode = wcomp->Save(xml, custom_widget_node);
		wnode.RemoveChildNode("position");
		xml.Save("custom_widgets/" + time_str + ".xml");
	}

	void MainWindow::OnPaint(ax::GC gc)
	{
		const ax::Rect rect(ax::Point(0, 0), win->dimension.GetSize());

		gc.SetColor(ax::Color(0.3));
		gc.DrawRectangle(rect);
		gc.DrawRectangleContour(rect);

		gc.SetColor(ax::Color(1.0));
		gc.DrawString(_font, _help_bar_str, ax::Point(5, rect.size.h - 16));
	}
}
}
