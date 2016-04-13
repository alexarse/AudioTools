/*
 * Copyright (c) 2016 AudioTools - All Rights Reserved
 *
 * This Software may not be distributed in parts or its entirety
 * without prior written agreement by AutioTools.
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
 
#include "atEditorMainWindow.h"

#include <OpenAX/Knob.h>
#include <OpenAX/Label.h>
#include <OpenAX/Panel.h>
#include <OpenAX/ScrollBar.h>
#include <OpenAX/Slider.h>
#include <OpenAX/WidgetLoader.h>
#include <OpenAX/WindowManager.h>
#include <OpenAX/WindowTree.h>

#include "CodeEditor.h"
#include "PyoAudio.h"
#include "atCommon.h"
#include "atEditorLoader.h"

namespace at {
namespace editor {
	MainWindow::MainWindow(const ax::Rect& rect)
		: _has_tmp_widget(false)
	{
		// Create window.
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &MainWindow::OnPaint);
		win->event.OnResize = ax::WBind<ax::Size>(this, &MainWindow::OnResize);

		// Create top menu.
		ax::Rect top_menu_rect(0, 0, rect.size.x, 30);
		_statusBar = new StatusBar(top_menu_rect);
		win->node.Add(std::shared_ptr<ax::Window::Backbone>(_statusBar));

		_statusBar->SetLayoutFilePath("default.xml");

		ax::Window* sb_win = _statusBar->GetWindow();
		sb_win->AddConnection(StatusBar::SAVE_LAYOUT, GetOnSaveLayout());
		sb_win->AddConnection(StatusBar::OPEN_LAYOUT, GetOnOpenLayout());
		sb_win->AddConnection(StatusBar::RELOAD_SCRIPT, GetOnReloadScript());

		sb_win->AddConnection(StatusBar::TOGGLE_LEFT_PANEL, GetOnToggleLeftPanel());
		sb_win->AddConnection(StatusBar::TOGGLE_BOTTOM_PANEL, GetOnToggleBottomPanel());
		sb_win->AddConnection(StatusBar::TOGGLE_RIGHT_PANEL, GetOnToggleRightPanel());

		sb_win->AddConnection(StatusBar::VIEW_LAYOUT, GetOnViewLayout());

		// Create grid window.
		ax::Rect grid_rect(WIDGET_MENU_WIDTH, 30, rect.size.x - WIDGET_MENU_WIDTH - INSPECTOR_MENU_WIDTH,
			rect.size.y - 30 - 200 - 18);
		win->node.Add(_gridWindow = ax::shared<GridWindow>(grid_rect));

		_gridWindow->GetWindow()->AddConnection(1234, GetOnSelectWidget());
		_gridWindow->GetWindow()->AddConnection(GridWindow::UNSELECT_ALL, GetOnUnSelectAllWidget());

		// Create widget menu. // 75
		ax::Rect widget_menu_rect(0, 30, WIDGET_MENU_WIDTH, rect.size.y - 30 - 18);
		_widgetMenu = ax::shared<WidgetMenu>(widget_menu_rect);
		win->node.Add(_widgetMenu);

		_widgetMenu->GetWindow()->AddConnection(WidgetMenu::SMALLER_MENU, GetOnSmallerLeftMenu());

		// Create info menu.
		ax::Rect info_rect(
			rect.size.x - INSPECTOR_MENU_WIDTH, 30, INSPECTOR_MENU_WIDTH, rect.size.y - 30 - 18);
		win->node.Add(_inspectorMenu = ax::shared<InspectorMenu>(info_rect));

		// Create code editor.
		TextEditor::Info txt_info;
		txt_info.bg_color = ax::Color(1.0);
		txt_info.cursor_color = ax::Color(0.0);
		txt_info.line_number_bg_color = ax::Color(0.95);
		txt_info.line_number_color = ax::Color(0.4);
		txt_info.text_color = ax::Color(0.0);

		ax::Rect bottom_rect(WIDGET_MENU_WIDTH + 1, rect.size.y - 200 - 18,
			rect.size.x - WIDGET_MENU_WIDTH - INSPECTOR_MENU_WIDTH, 200);

		auto b_section = ax::shared<BottomSection>(bottom_rect);
		win->node.Add(b_section);
		_bottom_section = b_section.get();
		_bottom_section->GetWindow()->AddConnection(BottomSection::RESIZE, GetOnResizeCodeEditor());
		//		win->node.Add(_codeEditor = ax::shared<CodeEditor>(bottom_rect));

		_bottom_section->GetWindow()->AddConnection(10020, ax::Event::Function([&](ax::Event::Msg* msg) {
														ax::Print("Save");
														std::vector<ax::Window::Ptr>& children
															= _gridWindow->GetWindow()->node.GetChildren();

														for (auto& n : children) {
															n->Update();
														}
													}));

		/// @todo Add enum for events.
		win->AddConnection(8000, GetOnCreateDraggingWidget());
		win->AddConnection(8001, GetOnDraggingWidget());
		win->AddConnection(8002, GetOnReleaseObjWidget());

		//		_codeEditor->GetWindow()->AddConnection(CodeEditor::RESIZE, GetOnResizeCodeEditor());
		//
		//		/// @todo Add enum for events.
		//		_codeEditor->GetWindow()->AddConnection(10020, ax::Event::Function([&](ax::Event::Msg* msg) {
		//													ax::Print("Save");
		//													std::vector<ax::Window::Ptr>& children
		//														=
		//_gridWindow->GetWindow()->node.GetChildren();
		//
		//													for (auto& n : children) {
		//														n->Update();
		//													}
		//												}));
	}

	void MainWindow::OnSmallerLeftMenu(const ax::Button::Msg& msg)
	{
		ax::Print("OnSmallerLeftMenu");
		win->event.OnResize(win->dimension.GetSize());
	}

	void MainWindow::OnSelectWidget(const ax::Event::SimpleMsg<ax::Window*>& msg)
	{
		ax::Window* selected_win = msg.GetMsg();

		_gridWindow->UnSelectAllWidgets();

		if (selected_win != nullptr) {
			selected_win->property.AddProperty("current_editing_widget");
			selected_win->Update();
			_inspectorMenu->SetWidgetHandle(selected_win);
		}
		else {
			_inspectorMenu->SetWidgetHandle(nullptr);
		}

		if (_gridWindow->GetMainWindow() == nullptr) {
			_widgetMenu->SetOnlyMainWindowWidgetSelectable();
		}
	}

	void MainWindow::OnUnSelectAllWidget(const ax::Event::SimpleMsg<int>& msg)
	{
		ax::Print("REMOVE ALL");
		_inspectorMenu->RemoveHandle();
	}

	void MainWindow::OnResizeCodeEditor(const ax::Event::SimpleMsg<int>& msg)
	{
		win->event.OnResize(win->dimension.GetSize());
	}

	void MainWindow::OnReloadScript(const ax::Event::SimpleMsg<int>& msg)
	{
		ax::Print("Reload script");

		/// @todo Do this in another thread and add a feedback to user somehow.
		//----------------------------------------------------------------------
		//		_codeEditor->SaveFile(_codeEditor->GetScriptPath());
		_bottom_section->SaveFile(_bottom_section->GetScriptPath());
		PyoAudio::GetInstance()->ReloadScript(_bottom_section->GetScriptPath());
		//----------------------------------------------------------------------
	}

	void MainWindow::OnToggleLeftPanel(const ax::Toggle::Msg& msg)
	{
		ax::Window* w = _widgetMenu->GetWindow();

		if (w->IsShown()) {
			w->Hide();
		}
		else {
			w->Show();
		}

		win->event.OnResize(win->dimension.GetSize());
	}

	void MainWindow::OnToggleBottomPanel(const ax::Toggle::Msg& msg)
	{
		//		ax::Window* w = _codeEditor->GetWindow();
		ax::Window* w = _bottom_section->GetWindow();

		if (w->IsShown()) {
			w->Hide();
		}
		else {
			w->Show();
		}

		win->event.OnResize(win->dimension.GetSize());
	}

	void MainWindow::OnToggleRightPanel(const ax::Toggle::Msg& msg)
	{
		ax::Window* w = _inspectorMenu->GetWindow();

		if (w->IsShown()) {
			w->Hide();
		}
		else {
			w->Show();
		}

		win->event.OnResize(win->dimension.GetSize());
	}

	void MainWindow::OnSaveLayout(const ax::Event::StringMsg& msg)
	{
		//----------------------------------------------------------------------
		//		_gridWindow->SaveLayout("layouts/" + msg.GetMsg(), _codeEditor->GetScriptPath());
		_gridWindow->SaveLayout("layouts/" + msg.GetMsg(), _bottom_section->GetScriptPath());
		//----------------------------------------------------------------------
	}

	void MainWindow::OnOpenLayout(const ax::Event::StringMsg& msg)
	{
		if (!msg.GetMsg().empty()) {
			std::string script_path = _gridWindow->OpenLayout("layouts/" + msg.GetMsg());

			if (!script_path.empty()) {
				ax::Print("Loading script :", script_path);
				_statusBar->SetLayoutFilePath(msg.GetMsg());
				//----------------------------------------------------------------------
				//				_codeEditor->OpenFile(script_path);
				_bottom_section->OpenFile(script_path);

				PyoAudio::GetInstance()->ReloadScript(script_path);
				//----------------------------------------------------------------------
			}

			if (_gridWindow->GetMainWindow() == nullptr) {
				_widgetMenu->SetOnlyMainWindowWidgetSelectable();
			}
			else {
				_widgetMenu->SetAllSelectable();
			}
		}
	}

	void MainWindow::OnViewLayout(const ax::Event::SimpleMsg<int>& msg)
	{
		ax::Print("Main window view.");
		ax::Window* main_win = _gridWindow->GetMainWindow();

		if (main_win == nullptr) {
			ax::Print("No main window setup.");
			/// @todo Add a message box to tell user.
			return;
		}

		ax::Rect rect(main_win->dimension.GetRect());

		_view_info.old_frame_size = ax::App::GetInstance().GetFrameSize();
		_view_info.old_main_window_position = rect.position;
		_view_info.left_menu_shown = _widgetMenu->GetWindow()->IsShown();
		_view_info.right_menu_shown = _inspectorMenu->GetWindow()->IsShown();
		//		_view_info.editor_shown = _codeEditor->GetWindow()->IsShown();
		_view_info.editor_shown = _bottom_section->GetWindow()->IsShown();

		main_win->dimension.SetPosition(ax::Point(0, 0));
		_widgetMenu->GetWindow()->Hide();
		_inspectorMenu->GetWindow()->Hide();
		//		_codeEditor->GetWindow()->Hide();
		_bottom_section->GetWindow()->Hide();
		_statusBar->GetWindow()->Hide();

		_gridWindow->GetWindow()->dimension.SetPosition(ax::Point(0, 0));
		_gridWindow->GetWindow()->dimension.SetSize(main_win->dimension.GetSize());
		_view_mode = true;

		ax::App::GetInstance().SetFrameSize(rect.size);

		ax::Button::Info btn_info;
		btn_info.normal = ax::Color(0.0, 0.0);
		btn_info.hover = ax::Color(0.0, 0.0);
		btn_info.clicking = ax::Color(0.0, 0.0);
		btn_info.selected = ax::Color(0.0, 0.0);
		btn_info.contour = ax::Color(0.0, 0.0);
		btn_info.font_color = ax::Color(0.0, 0.0);

		ax::Point back_btn_pos(rect.size - ax::Point(25, 25));

		auto tmp_back_btn = ax::shared<ax::Button>(ax::Rect(back_btn_pos, ax::Size(25, 25)),
			GetOnBackToEditor(), btn_info, "resources/back.png", "", ax::Button::Flags::SINGLE_IMG);

		tmp_back_btn->GetWindow()->property.AddProperty("TemporaryBackButton");

		main_win->node.Add(tmp_back_btn);

		_gridWindow->UnSelectAllWidgets();
		_inspectorMenu->SetWidgetHandle(nullptr);

		ax::App::GetInstance().SetResizable(false);
		ax::App::GetInstance().SetFocusAndCenter();
	}

	void MainWindow::OnBackToEditor(const ax::Button::Msg& msg)
	{
		_view_mode = false;

		_statusBar->GetWindow()->Show();

		if (_view_info.left_menu_shown) {
			_widgetMenu->GetWindow()->Show();
		}

		if (_view_info.right_menu_shown) {
			_inspectorMenu->GetWindow()->Show();
		}

		if (_view_info.editor_shown) {
			//			_codeEditor->GetWindow()->Show();
			_bottom_section->GetWindow()->Show();
		}

		ax::Window* main_win = _gridWindow->GetMainWindow();
		main_win->dimension.SetPosition(_view_info.old_main_window_position);

		_gridWindow->GetWindow()->dimension.SetPosition(ax::Point(WIDGET_MENU_WIDTH, 30));

		// Find tmp back button and delete it.
		ax::Window* main_window = _gridWindow->GetMainWindow();

		if (main_window == nullptr) {
			ax::Error("Main window doesn't exist when comming back from view mode.");
			ax::App::GetInstance().SetFrameSize(_view_info.old_frame_size);
			return;
		}

		auto& children = main_window->node.GetChildren();

		ax::Window* back_btn = nullptr;
		int index = -1;

		for (int i = 0; i < children.size(); i++) {
			if (children[i]->property.HasProperty("TemporaryBackButton")) {
				back_btn = children[i].get();
				index = i;
				break;
			}
		}

		if (back_btn != nullptr && index != -1) {
			back_btn->event.UnGrabMouse();
			ax::App::GetInstance().GetWindowManager()->ReleaseMouseHover();
			children.erase(children.begin() + index);
		}
		else {
			ax::Error("Back to editor button wasn't found when comming back "
					  "from view mode.");
		}

		ax::App::GetInstance().SetResizable(true);
		ax::App::GetInstance().SetFrameSize(_view_info.old_frame_size);
		ax::App::GetInstance().SetFocusAndCenter();
	}

	void MainWindow::OnCreateDraggingWidget(const ax::Event::SimpleMsg<ObjMsg>& msg)
	{
		ax::StringPair obj_info = msg.GetMsg().first;
		std::string builder_name = obj_info.first;
		std::string file_path = obj_info.second;
		ax::Point pos(msg.GetMsg().second);

		ax::widget::Loader* loader = ax::widget::Loader::GetInstance();
		ax::widget::Builder* builder = loader->GetBuilder(builder_name);

		if (builder == nullptr) {
			ax::Error("Builder", builder_name, "doesn't exist.");
		}

		auto obj(builder->Create(pos, file_path));
		ax::App::GetInstance().GetPopupManager()->GetWindowTree()->AddTopLevel(
			ax::Window::Ptr(obj->GetWindow()));
		obj->GetWindow()->backbone = obj;
		_has_tmp_widget = true;
		_tmp_widget_builder_name = builder_name;
	}

	void MainWindow::OnDraggingWidget(const ax::Event::SimpleMsg<ax::Point>& msg)
	{
		if (_has_tmp_widget) {
			ax::Point pos(msg.GetMsg());
			ax::Window::Ptr wobj = ax::App::GetInstance().GetPopupManager()->GetWindowTree()->GetTopLevel();

			if (wobj) {
				wobj->dimension.SetPosition(pos);
			}
		}
	}

	void MainWindow::OnReleaseObjWidget(const ax::Event::SimpleMsg<ax::Point>& msg)
	{
		ax::Point pos(msg.GetMsg());

		if (_has_tmp_widget) {
			_has_tmp_widget = false;
			std::vector<ax::Window::Ptr>& nodes
				= ax::App::GetInstance().GetPopupManager()->GetWindowTree()->GetNodeVector();
			ax::Window::Ptr widget_win = nodes[0];

			// Remove all window from Popup manager window tree.
			nodes.clear();

			// Check if a MainWindow exist first.
			ax::Window* main_window = _gridWindow->GetMainWindow();

			if (main_window == nullptr) {
				// Assign MainWindow name to first ax::Panel added.
				if (_tmp_widget_builder_name == "Panel") {
					ax::Panel* panel = static_cast<ax::Panel*>(widget_win->backbone.get());
					panel->SetName("MainWindow");
					widget_win->property.AddProperty("MainWindow");
					main_window = widget_win.get();
				}
				else {
					ax::Print("A MainWindow Panel shall be created first to add widget.");
					return;
				}
			}

			bool inside_grid(_gridWindow->GetWindow()->dimension.GetAbsoluteRect().IsPointInside(pos));

			// Not dragging above grid window then do nothing.
			if (!inside_grid) {
				// The temporary widget will be deleted.
				return;
			}

			_widgetMenu->SetAllSelectable();

			if (widget_win->GetId() != main_window->GetId()) {
				bool inside_main_window = main_window->dimension.GetAbsoluteRect().IsPointInside(pos);

				if (!inside_main_window) {
					// The temporary widget will be deleted.
					ax::Print("Drag widget over the MainWindow.");
					return;
				}
			}

			// Is inside grid window.
			ax::Window* hover_window
				= ax::App::GetInstance().GetWindowManager()->GetWindowTree()->FindMousePosition(pos);

			/// @todo Make sure this doesn't loop for ever.
			while (!hover_window->property.HasProperty("AcceptWidget")) {
				hover_window = hover_window->node.GetParent();
			}

			if (hover_window) {
				ax::Print("FOUND WINDOW");
				// Reparent.
				widget_win->node.SetParent(hover_window);
				hover_window->node.GetChildren().push_back(widget_win);
				widget_win->dimension.SetPosition(pos - hover_window->dimension.GetAbsoluteRect().position);

				// Setup widget.
				Loader loader(_gridWindow->GetWindow());
				loader.SetupExistingWidget(widget_win.get(), _tmp_widget_builder_name);

				_gridWindow->UnSelectAllWidgets();

				if (widget_win != nullptr) {
					widget_win->property.AddProperty("current_editing_widget");
					widget_win->Update();
					_inspectorMenu->SetWidgetHandle(widget_win.get());
				}
			}

			else {
				// Reparent.
				widget_win->node.SetParent(_gridWindow->GetWindow());
				_gridWindow->GetWindow()->node.GetChildren().push_back(widget_win);

				widget_win->dimension.SetPosition(
					pos - _gridWindow->GetWindow()->dimension.GetAbsoluteRect().position);

				// Setup widget.
				Loader loader(_gridWindow->GetWindow());
				loader.SetupExistingWidget(widget_win.get(), _tmp_widget_builder_name);

				_gridWindow->UnSelectAllWidgets();

				if (widget_win != nullptr) {
					widget_win->property.AddProperty("current_editing_widget");
					widget_win->Update();
					_inspectorMenu->SetWidgetHandle(widget_win.get());
				}
			}
		}
	}

	void MainWindow::OnResize(const ax::Size& size)
	{
		if (_view_mode) {
			return;
		}

		// Resize status bar.
		ax::Size top_menu_size(_statusBar->GetWindow()->dimension.GetSize());
		top_menu_size.x = size.x;
		_statusBar->GetWindow()->dimension.SetSize(top_menu_size);

		bool widget_menu = _widgetMenu->GetWindow()->IsShown();
		bool inspector = _inspectorMenu->GetWindow()->IsShown();
		//		bool code_editor = _codeEditor->GetWindow()->IsShown();
		bool code_editor = _bottom_section->GetWindow()->IsShown();

		int editor_height = 0;

		if (code_editor) {
			//			editor_height = _codeEditor->GetWindow()->dimension.GetSize().y;
			editor_height = _bottom_section->GetWindow()->dimension.GetSize().y;
			if (editor_height > size.y - STATUS_BAR_HEIGHT) {
				editor_height = size.y - STATUS_BAR_HEIGHT;
			}
		}

		int grid_height = size.y - STATUS_BAR_HEIGHT - editor_height - BOTTOM_BAR_HEIGHT;

		int widget_menu_width = _widgetMenu->GetWindow()->dimension.GetRect().size.x;

		if (widget_menu && inspector) {
			ax::Size widget_menu_size(widget_menu_width, size.y - STATUS_BAR_HEIGHT - BOTTOM_BAR_HEIGHT);
			_widgetMenu->GetWindow()->dimension.SetSize(widget_menu_size);

			ax::Rect grid_rect(widget_menu_width, STATUS_BAR_HEIGHT,
				size.x - widget_menu_width - INSPECTOR_MENU_WIDTH, grid_height);
			_gridWindow->GetWindow()->dimension.SetRect(grid_rect);

			ax::Rect info_rect(size.x - INSPECTOR_MENU_WIDTH, STATUS_BAR_HEIGHT, INSPECTOR_MENU_WIDTH,
				size.y - STATUS_BAR_HEIGHT - BOTTOM_BAR_HEIGHT);
			_inspectorMenu->GetWindow()->dimension.SetRect(info_rect);

			if (code_editor) {
				ax::Rect editor_rect(widget_menu_width + 1, size.y - editor_height - BOTTOM_BAR_HEIGHT,
					size.x - widget_menu_width - INSPECTOR_MENU_WIDTH, editor_height);
				//				_codeEditor->GetWindow()->dimension.SetRect(editor_rect);
				_bottom_section->GetWindow()->dimension.SetRect(editor_rect);
			}
		}
		else if (widget_menu) {
			ax::Size widget_menu_size(widget_menu_width, size.y - STATUS_BAR_HEIGHT - BOTTOM_BAR_HEIGHT);
			_widgetMenu->GetWindow()->dimension.SetSize(widget_menu_size);

			ax::Rect grid_rect(widget_menu_width, STATUS_BAR_HEIGHT, size.x - widget_menu_width, grid_height);
			_gridWindow->GetWindow()->dimension.SetRect(grid_rect);

			if (code_editor) {
				ax::Rect editor_rect(widget_menu_width + 1, size.y - editor_height - BOTTOM_BAR_HEIGHT,
					size.x - widget_menu_width, editor_height);
				//				_codeEditor->GetWindow()->dimension.SetRect(editor_rect);
				_bottom_section->GetWindow()->dimension.SetRect(editor_rect);
			}
		}
		else if (inspector) {
			ax::Rect grid_rect(0, STATUS_BAR_HEIGHT, size.x - INSPECTOR_MENU_WIDTH, grid_height);
			_gridWindow->GetWindow()->dimension.SetRect(grid_rect);

			ax::Rect info_rect(size.x - INSPECTOR_MENU_WIDTH, STATUS_BAR_HEIGHT, INSPECTOR_MENU_WIDTH,
				size.y - STATUS_BAR_HEIGHT - BOTTOM_BAR_HEIGHT);
			_inspectorMenu->GetWindow()->dimension.SetRect(info_rect);

			if (code_editor) {
				ax::Rect editor_rect(1, size.y - editor_height - BOTTOM_BAR_HEIGHT,
					size.x - INSPECTOR_MENU_WIDTH, editor_height);
				//				_codeEditor->GetWindow()->dimension.SetRect(editor_rect);
				_bottom_section->GetWindow()->dimension.SetRect(editor_rect);
			}
		}
		else {
			ax::Rect grid_rect(0, STATUS_BAR_HEIGHT, size.x, grid_height);
			_gridWindow->GetWindow()->dimension.SetRect(grid_rect);

			if (code_editor) {
				ax::Rect editor_rect(1, size.y - editor_height - BOTTOM_BAR_HEIGHT, size.x, editor_height);
				//				_codeEditor->GetWindow()->dimension.SetRect(editor_rect);
				_bottom_section->GetWindow()->dimension.SetRect(editor_rect);
			}
		}
	}

	void MainWindow::OnPaint(ax::GC gc)
	{
		ax::Rect rect(ax::Point(0, 0), win->dimension.GetSize());

		gc.SetColor(ax::Color(0.3));
		gc.DrawRectangle(rect);
		gc.DrawRectangleContour(rect);

		gc.SetColor(ax::Color(0.30));
		gc.DrawRectangle(ax::Rect(0, rect.size.y - 18, rect.size.x, 18));
	}
}
}