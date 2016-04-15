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

#include "atEditorGridWindow.h"
#include <OpenAX/WindowManager.h>
#include <OpenAX/rapidxml.hpp>
#include <OpenAX/rapidxml_print.hpp>
#include <fstream>

#include "PyoAudio.h"
#include "PyoComponent.h"
#include "atCommon.h"
#include "atEditor.h"
#include "atEditorLoader.h"
#include "atEditorMainWindow.h"
#include "atSkin.hpp"

#include <OpenAX/Button.h>
#include <OpenAX/Knob.h>
#include <OpenAX/Label.h>
#include <OpenAX/Panel.h>
#include <OpenAX/Slider.h>
#include <OpenAX/Toggle.h>
#include <OpenAX/WidgetLoader.h>

namespace at {
namespace editor {
	GridWindow::GridWindow(const ax::Rect& rect)
		: _grid_space(10)
		, _selection(false, ax::Rect(0, 0, 0, 0))
		, _bg_color(at::Skin::GetInstance()->data.grid_window_bg)
	{
		// Create window.
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &GridWindow::OnPaint);
		win->event.OnMouseLeftDown = ax::WBind<ax::Point>(this, &GridWindow::OnMouseLeftDown);
		win->event.OnMouseLeftDragging = ax::WBind<ax::Point>(this, &GridWindow::OnMouseLeftDragging);
		win->event.OnMouseLeftUp = ax::WBind<ax::Point>(this, &GridWindow::OnMouseLeftUp);
		win->event.OnBackSpaceDown = ax::WBind<char>(this, &GridWindow::OnBackSpaceDown);
		win->event.OnKeyDown = ax::WBind<char>(this, &GridWindow::OnKeyDown);

		ax::App::GetInstance().GetWindowManager()->AddGlobalGrabedWindow(win);

		win->property.AddProperty("BlockDrawing");
		win->property.AddProperty("AcceptWidget");

		ax::widget::Loader* loader = ax::widget::Loader::GetInstance();
		loader->AddBuilder("Button", new ax::Button::Builder());
		loader->AddBuilder("Toggle", new ax::Toggle::Builder());
		loader->AddBuilder("Knob", new ax::Knob::Builder());
		loader->AddBuilder("Label", new ax::Label::Builder());
		loader->AddBuilder("Panel", new ax::Panel::Builder());
		loader->AddBuilder("Slider", new ax::Slider::Builder());

		OpenLayout("layouts/default.xml");
	}

	void GridWindow::SetGridSpace(const int& space)
	{
		if (space > 0 && _grid_space != space) {
			_grid_space = space;
			win->Update();
		}
	}

	ax::Window* GridWindow::GetMainWindow()
	{
		std::vector<ax::Window::Ptr>& children = win->node.GetChildren();
		for (auto& n : children) {
			if (n->property.HasProperty("MainWindow")) {
				return n.get();
			}
		}

		return nullptr;
	}

	void GridWindow::SaveLayout(const std::string& path, const std::string& script_path)
	{
		std::vector<ax::Window::Ptr>& children = win->node.GetChildren();

		ax::Xml xml;
		ax::Xml::Node layout = xml.CreateNode("Layout");
		xml.AddMainNode(layout);
		layout.AddAttribute("script", script_path);

		// Callback for saving widget with child widgets in them.
		std::function<void(ax::Xml&, ax::Xml::Node&, ax::Window*)> panel_save_child
			= [&](ax::Xml& xml, ax::Xml::Node& node, ax::Window* child_win) {

				  ax::widget::Component::Ptr opt = child_win->component.Get<ax::widget::Component>("Widget");

				  if (opt) {
					  if (child_win->property.HasProperty("AcceptWidget")) {
						  opt->SetSaveChildCallback(panel_save_child);
					  }
					  // Save ax::Object.
					  ax::Xml::Node child_node = opt->Save(xml, node);

					  if (child_win->component.Has("pyo")) {
						  pyo::Component::Ptr comp = child_win->component.Get<pyo::Component>("pyo");

						  std::string fct_name = comp->GetFunctionName();

						  ax::Xml::Node pyo_node = xml.CreateNode("pyo", fct_name);
						  child_node.AddNode(pyo_node);
					  }
				  }
			  };

		for (auto& n : children) {
			ax::widget::Component::Ptr opt = n->component.Get<ax::widget::Component>("Widget");

			if (opt) {
				if (n->property.HasProperty("AcceptWidget")) {
					opt->SetSaveChildCallback(panel_save_child);
				}

				// Save ax::Object.
				ax::Xml::Node node = opt->Save(xml, layout);

				if (n->component.Has("pyo")) {
					ax::Print("HAS PYO");
					pyo::Component::Ptr comp = n->component.Get<pyo::Component>("pyo");
					std::string fct_name = comp->GetFunctionName();
					ax::Xml::Node pyo_node = xml.CreateNode("pyo", fct_name);
					node.AddNode(pyo_node);
				}
			}
		}

		xml.Save(path);
	}

	std::string GridWindow::OpenLayout(const std::string& path)
	{
		at::editor::Loader loader(win);
		return loader.OpenLayout(path, true);
	}

	void GridWindow::SetBackgroundColor(const ax::Color& color)
	{
		_bg_color = color;
		win->Update();
	}
	
	ax::Window* GridWindow::GetWidgetByName(const std::string& name)
	{
		auto& children = win->node.GetChildren();
		
		if(children.empty()) {
			return nullptr;
		}
		
		return children[0].get();
	}

	void GridWindow::OnBackSpaceDown(const char& c)
	{
		// Delete current selected widget.

		ax::App& app = ax::App::GetInstance();

		// If command is down and mouse is still inside window.
		if (app.GetWindowManager()->IsCmdDown() && app.GetWindowManager()->IsMouseStillInChildWindow(win)) {
			MainWindow* main_win = at::editor::App::GetInstance()->GetMainWindow();

			std::vector<ax::Window*> sel_wins = main_win->GetSelectedWindows();

			if (sel_wins.size()) {
				ax::Print("Should delete widget.");
				main_win->DeleteCurrentWidgets();
			}
		}
	}

	void GridWindow::OnKeyDown(const char& c)
	{
		ax::App& app = ax::App::GetInstance();

		// If command is down and mouse is still inside window.
		if (app.GetWindowManager()->IsCmdDown() && app.GetWindowManager()->IsMouseStillInChildWindow(win)) {

			if (c == 'd' || c == 'D') {

				MainWindow* main_win = at::editor::App::GetInstance()->GetMainWindow();

				std::vector<ax::Window*> sel_wins = main_win->GetSelectedWindows();

				if (sel_wins.size()) {
					// Copy selected widgets.
					std::shared_ptr<ax::Window::Backbone> bck_bone(sel_wins[0]->backbone->GetCopy());
					
					if(bck_bone == nullptr) {
						return;
					}
					
					const ax::Rect rect(sel_wins[0]->dimension.GetRect());
					
					bck_bone->GetWindow()->dimension.SetPosition(
						rect.position + ax::Point(rect.size.x + 2, 0));
					
					at::editor::Loader loader(win);
					ax::widget::Component* widget = static_cast<ax::widget::Component*>(bck_bone->GetWindow()->component.Get("Widget").get());

					ax::Window* parent = sel_wins[0]->node.GetParent();
					
					if(parent == nullptr) {
						return;
					}
					
					if(parent->GetId() == win->GetId()) {
						return;
					}
					
					parent->node.Add(bck_bone);
					loader.SetupExistingWidget(bck_bone->GetWindow(), widget->GetBuilderName());
				}
			}
		}
	}

	void GridWindow::OnMouseLeftDown(const ax::Point& pos)
	{
		bool cmd_down = ax::App::GetInstance().GetWindowManager()->IsCmdDown();

		// Clear menu.
		if (cmd_down) {
			UnSelectAllWidgets();
			win->PushEvent(UNSELECT_ALL, new ax::Event::SimpleMsg<int>(0));
		}
		// Start multi widget selection.
		else {
			ax::Point m_pos(pos - win->dimension.GetAbsoluteRect().position);
			_selection.first = true;
			_selection.second.position = m_pos;
			_selection.second.size = ax::Size(1, 1);

			win->event.GrabMouse();
			win->Update();
		}
	}

	void UnselectAllChildWidget(ax::Window::Ptr window)
	{
		window->property.RemoveProperty("current_editing_widget");

		if (window->property.HasProperty("AcceptWidget")) {
			std::vector<ax::Window::Ptr>& children = window->node.GetChildren();

			for (auto& n : children) {
				UnselectAllChildWidget(n);
			}
		}
	}

	void GridWindow::UnSelectAllWidgets()
	{
		std::vector<ax::Window::Ptr>& children = win->node.GetChildren();
		for (auto& n : children) {
			UnselectAllChildWidget(n);
		}
		win->Update();
	}

	void GridWindow::OnMouseLeftDragging(const ax::Point& pos)
	{
		ax::Point m_pos(pos - win->dimension.GetAbsoluteRect().position);
		_selection.second.size = m_pos - _selection.second.position;
		win->Update();
	}

	void GridWindow::OnMouseLeftUp(const ax::Point& pos)
	{
		if (win->event.IsGrabbed()) {
			win->event.UnGrabMouse();
			_selection.first = false;
			win->Update();
		}
	}

	void GridWindow::OnPaint(ax::GC gc)
	{
		ax::Rect rect(win->dimension.GetDrawingRect());

		// Background.
		gc.SetColor(_bg_color);
		gc.DrawRectangle(rect);

		gc.SetColor(at::Skin::GetInstance()->data.grid_window_lines);

		// Vertical lines.
		for (int x = _grid_space; x < rect.size.x; x += _grid_space) {
			gc.DrawLineStripple(ax::Point(x, 0), ax::Point(x, rect.size.y));
		}

		// Horizontal lines.
		for (int y = _grid_space; y < rect.size.y; y += _grid_space) {
			gc.DrawLineStripple(ax::Point(0, y), ax::Point(rect.size.x, y));
		}

		// Selection rectangle.
		if (_selection.first) {
			gc.SetColor(ax::Color(0.7, 0.5));
			gc.DrawRectangle(_selection.second);
			gc.SetColor(ax::Color(0.7, 0.8));
			gc.DrawRectangleContour(_selection.second);
		}

		// Grid contour.
		gc.SetColor(at::Skin::GetInstance()->data.grid_window_contour);
		gc.DrawRectangleContour(rect);
	}
}
}