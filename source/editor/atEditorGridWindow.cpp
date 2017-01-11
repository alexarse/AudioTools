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

#include "editor/atEditorGridWindow.hpp"
#include <axlib/DropMenu.hpp>
#include <axlib/NodeVisitor.hpp>
#include <axlib/WindowManager.hpp>
#include <axlib/Xml.hpp>
#include <fstream>

#include "PyoAudio.h"
#include "atSkin.hpp"
#include "atUniqueNameComponent.h"
#include "editor/atEditor.hpp"
#include "editor/atEditorLoader.hpp"
#include "editor/atEditorMainWindow.hpp"
#include "python/PyoComponent.hpp"

// Widgets.
#include <axlib/Button.hpp>
#include <axlib/Knob.hpp>
#include <axlib/Label.hpp>
#include <axlib/NumberBox.hpp>
#include <axlib/Panel.hpp>
#include <axlib/Slider.hpp>
#include <axlib/Sprite.hpp>
#include <axlib/Toggle.hpp>
#include <axlib/WidgetLoader.hpp>

namespace at {
namespace editor {
	GridWindow::GridWindow(const ax::Rect& rect)
		: _grid_space(10)
		, _selection(false, ax::Rect(0, 0, 0, 0))
		, _bg_color(at::Skin::GetInstance()->data.grid_window_bg)
		, _right_click_menu(false)
		, _draw_grid_over_children(false)
	{
		// Create window.
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &GridWindow::OnPaint);
		win->event.OnPaintOverChildren = ax::WBind<ax::GC>(this, &GridWindow::OnPaintOverChildren);

		win->event.OnMouseLeftDown = ax::WBind<ax::Point>(this, &GridWindow::OnMouseLeftDown);
		win->event.OnMouseLeftDragging = ax::WBind<ax::Point>(this, &GridWindow::OnMouseLeftDragging);
		win->event.OnMouseLeftUp = ax::WBind<ax::Point>(this, &GridWindow::OnMouseLeftUp);
		win->event.OnBackSpaceDown = ax::WBind<char>(this, &GridWindow::OnBackSpaceDown);
		win->event.OnKeyDown = ax::WBind<char>(this, &GridWindow::OnKeyDown);

		win->event.OnLeftArrowDown = ax::WBind<char>(this, &GridWindow::OnLeftArrowDown);
		win->event.OnRightArrowDown = ax::WBind<char>(this, &GridWindow::OnRightArrowDown);
		win->event.OnUpArrowDown = ax::WBind<char>(this, &GridWindow::OnUpArrowDown);
		win->event.OnDownArrowDown = ax::WBind<char>(this, &GridWindow::OnDownArrowDown);

		win->event.OnResize = ax::WBind<ax::Size>(this, &GridWindow::OnResize);

		win->event.OnGlobalClick
			= ax::WBind<ax::Window::Event::GlobalClick>(this, &GridWindow::OnGlobalClick);

		win->AddConnection(DROP_WIDGET_MENU, GetOnDropWidgetMenu());
		win->AddConnection(BEGIN_DRAGGING_WIDGET, GetOnWidgetIsDragging());
		win->AddConnection(DONE_DRAGGING_WIDGET, GetOnWidgetDoneDragging());

		win->event.OnAssignToWindowManager = ax::WBind<int>(this, &GridWindow::OnAssignToWindowManager);

		win->property.AddProperty("BlockDrawing");
		win->property.AddProperty("AcceptWidget");

		/// @todo Move this somewhere else.
		ax::widget::Loader* loader = ax::widget::Loader::GetInstance();
		loader->AddBuilder("Button", new ax::Button::Builder());
		loader->AddBuilder("Toggle", new ax::Toggle::Builder());
		loader->AddBuilder("Knob", new ax::Knob::Builder());
		loader->AddBuilder("Label", new ax::Label::Builder());
		loader->AddBuilder("NumberBox", new ax::NumberBox::Builder());
		loader->AddBuilder("Panel", new ax::Panel::Builder());
		loader->AddBuilder("Slider", new ax::Slider::Builder());
		loader->AddBuilder("Sprite", new ax::Sprite::Builder());

		ax::Rect d_rect(win->dimension.GetDrawingRect());
		_horizontal_lines_array.reserve((d_rect.size.h / _grid_space) * 2);
		_vertical_lines_array.reserve((d_rect.size.w / _grid_space) * 2);

		// Vertical lines.
		for (int x = _grid_space; x < d_rect.size.w; x += _grid_space) {
			_vertical_lines_array.push_back(ax::FPoint(x, 0));
			_vertical_lines_array.push_back(ax::FPoint(x, d_rect.size.h));
		}

		// Horizontal lines.
		for (int y = _grid_space; y < d_rect.size.h; y += _grid_space) {
			_horizontal_lines_array.push_back(ax::FPoint(0, y));
			_horizontal_lines_array.push_back(ax::FPoint(d_rect.size.w, y));
		}
	}

	void GridWindow::OnAssignToWindowManager(const int& v)
	{
		win->event.GrabGlobalMouse();
		win->event.GrabGlobalKey();
	}

	ax::Window* GridWindow::GetMainWindow()
	{
		std::vector<std::shared_ptr<ax::Window>>& children = win->node.GetChildren();

		for (auto& n : children) {
			if (n->property.HasProperty("MainWindow")) {
				return n.get();
			}
		}

		return nullptr;
	}

	void GridWindow::SaveLayout(const std::string& path, const std::string& script_path)
	{
		std::vector<std::shared_ptr<ax::Window>>& children = win->node.GetChildren();

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

					  if (child_win->component.Has("unique_name")) {
						  at::UniqueNameComponent::Ptr comp
							  = child_win->component.Get<at::UniqueNameComponent>("unique_name");

						  std::string name = comp->GetName();
						  ax::Xml::Node unique_name_node = xml.CreateNode("unique_name", name);
						  child_node.AddNode(unique_name_node);
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
					ax::console::Print("HAS PYO");
					pyo::Component::Ptr comp = n->component.Get<pyo::Component>("pyo");
					std::string fct_name = comp->GetFunctionName();
					ax::Xml::Node pyo_node = xml.CreateNode("pyo", fct_name);
					node.AddNode(pyo_node);
				}

				if (n->component.Has("unique_name")) {
					at::UniqueNameComponent::Ptr comp
						= n->component.Get<at::UniqueNameComponent>("unique_name");

					std::string name = comp->GetName();
					ax::Xml::Node unique_name_node = xml.CreateNode("unique_name", name);
					node.AddNode(unique_name_node);
				}
			}
		}

		ax::util::console::Print("Save path............", path);
		ax::util::console::Print(xml.GetString());
		xml.Save(path);
	}

	void GridWindow::OnDropWidgetMenu(const ax::event::SimpleMsg<std::pair<ax::Point, ax::Window*>>& msg)
	{
		ax::console::Print("Drop widget menu.");
		_right_click_menu = true;

		// Open menu.
		ax::DropMenu::Info menu_info;
		menu_info.normal = ax::Color(240, 240, 240);
		menu_info.hover = ax::Color(246, 246, 246);
		menu_info.font_color = ax::Color(0.0);
		menu_info.selected = ax::Color(41, 222, 255);
		menu_info.selected_hover = ax::Color(41, 226, 255);
		menu_info.selected_font_color = ax::Color(0.0);
		menu_info.contour = ax::Color(0.86);
		menu_info.separation = ax::Color(0.86);
		menu_info.up_down_arrow = ax::Color(0.35);
		menu_info.right_arrow = ax::Color(0.70);
		menu_info.item_height = 25;

		std::vector<std::string> menu_elems = { "Save as", "Remove", "Duplicate", "", "Snap to grid" };

		auto menu = ax::shared<ax::DropMenu>(
			ax::Rect(msg.GetMsg().first, ax::Size(100, 200)), GetOnMenuChoice(), menu_info, menu_elems);

		// Empty popup window tree.
		ax::App::GetInstance().GetPopupManager()->Clear();

		// Add to top level popup manager.
		ax::App::GetInstance().GetPopupManager()->GetWindowTree()->AddTopLevel(
			std::shared_ptr<ax::Window>(menu->GetWindow()));
		menu->GetWindow()->backbone = menu;
		ax::App::GetInstance().UpdateAll();
	}

	void GridWindow::OnMenuChoice(const ax::DropMenu::Msg& msg)
	{
		const std::string choice = msg.GetItem();

		if (choice == "Save as") {
			win->PushEvent(SAVE_PANEL_TO_WORKSPACE, new ax::event::EmptyMsg());
		}
		else if (choice == "Remove") {
			win->PushEvent(DELETE_SELECTED_WIDGET_FROM_RIGHT_CLICK, new ax::event::EmptyMsg());
		}
		else if (choice == "Duplicate") {
			win->PushEvent(DUPLICATE_SELECTED_WIDGET_FROM_RIGHT_CLICK, new ax::event::EmptyMsg());
		}
		else if (choice == "Snap to grid") {
			win->PushEvent(SNAP_WIDGET_TO_GRID_FROM_RIGHT_CLICK, new ax::event::EmptyMsg());
			ax::App::GetInstance().GetPopupManager()->Clear();
		}
	}

	void GridWindow::OnGlobalClick(const ax::Window::Event::GlobalClick& gclick)
	{
		if (_right_click_menu) {
			_right_click_menu = false;

			// Empty popup window tree.
			ax::App::GetInstance().GetPopupManager()->Clear();
			ax::App::GetInstance().UpdateAll();
		}
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

	ax::Window* GetWidgetByNameRecursive(ax::Window* window, const std::string& name)
	{
		if (window == nullptr) {
			return nullptr;
		}

		if (window->component.Has("unique_name")) {
			at::UniqueNameComponent::Ptr comp = window->component.Get<at::UniqueNameComponent>("unique_name");

			if (name == comp->GetName()) {
				return comp->GetWindow();
			}
		}

		if (window->property.HasProperty("AcceptWidget")) {
			std::vector<std::shared_ptr<ax::Window>>& children = window->node.GetChildren();

			for (auto& n : children) {
				ax::Window* tmp = GetWidgetByNameRecursive(n.get(), name);
				if (tmp != nullptr) {
					return tmp;
				}
			}
		}

		return nullptr;
	}

	ax::Window* GridWindow::GetWidgetByName(const std::string& name)
	{

		/// @todo Change this with ax::NodeVisitor.
		auto& children = win->node.GetChildren();

		if (children.empty()) {
			return nullptr;
		}

		for (auto& n : children) {
			ax::Window* tmp = GetWidgetByNameRecursive(n.get(), name);

			if (tmp != nullptr) {
				return tmp;
			}
		}

		return nullptr;
	}

	void GridWindow::SetGridSpace(int space)
	{
		space = ax::util::Clamp<int>(space, 5, 20);
		_grid_space = space;

		ax::Rect d_rect(win->dimension.GetDrawingRect());
		_horizontal_lines_array.clear();
		_vertical_lines_array.clear();
		_horizontal_lines_array.reserve((d_rect.size.h / _grid_space) * 2);
		_vertical_lines_array.reserve((d_rect.size.w / _grid_space) * 2);

		// Vertical lines.
		for (int x = _grid_space; x < d_rect.size.w; x += _grid_space) {
			_vertical_lines_array.push_back(ax::FPoint(x, 0));
			_vertical_lines_array.push_back(ax::FPoint(x, d_rect.size.h));
		}

		// Horizontal lines.
		for (int y = _grid_space; y < d_rect.size.h; y += _grid_space) {
			_horizontal_lines_array.push_back(ax::FPoint(0, y));
			_horizontal_lines_array.push_back(ax::FPoint(d_rect.size.w, y));
		}

		win->Update();
	}

	void GridWindow::OnBackSpaceDown(const char& c)
	{
		// Delete current selected widget.
		ax::App& app = ax::App::GetInstance();

		// If command is down and mouse is still inside window.
		if (app.GetWindowManager()->IsCmdDown() && app.GetWindowManager()->IsMouseStillInChildWindow(win)) {
			win->PushEvent(DELETE_SELECTED_WIDGET, new ax::event::EmptyMsg());
		}
	}

	void GridWindow::OnKeyDown(const char& c)
	{
		ax::App& app = ax::App::GetInstance();

		// If command is down and mouse is still inside window.
		if (app.GetWindowManager()->IsCmdDown() && app.GetWindowManager()->IsMouseStillInChildWindow(win)) {

			if (c == 'd' || c == 'D') {
				win->PushEvent(DUPLICATE_SELECTED_WIDGET, new ax::event::EmptyMsg());
			}
		}
	}

	void GridWindow::OnLeftArrowDown(const char& c)
	{
		win->PushEvent(ARROW_MOVE_SELECTED_WIDGET,
			new ax::event::SimpleMsg<ax::util::Direction>(ax::util::Direction::LEFT));
	}

	void GridWindow::OnRightArrowDown(const char& c)
	{
		win->PushEvent(ARROW_MOVE_SELECTED_WIDGET,
			new ax::event::SimpleMsg<ax::util::Direction>(ax::util::Direction::RIGHT));
	}

	void GridWindow::OnUpArrowDown(const char& c)
	{
		win->PushEvent(ARROW_MOVE_SELECTED_WIDGET,
			new ax::event::SimpleMsg<ax::util::Direction>(ax::util::Direction::UP));
	}

	void GridWindow::OnDownArrowDown(const char& c)
	{
		win->PushEvent(ARROW_MOVE_SELECTED_WIDGET,
			new ax::event::SimpleMsg<ax::util::Direction>(ax::util::Direction::DOWN));
	}

	void GridWindow::OnMouseLeftDown(const ax::Point& pos)
	{
		// Clear menu.
		if (ax::App::GetInstance().GetWindowManager()->IsCmdDown()) {
			UnSelectAllWidgets();
			win->PushEvent(UNSELECT_ALL, new ax::event::SimpleMsg<int>(0));
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

	void UnselectAllChildWidget(std::shared_ptr<ax::Window> window)
	{
		window->property.RemoveProperty("current_editing_widget");

		if (window->property.HasProperty("AcceptWidget")) {
			std::vector<std::shared_ptr<ax::Window>>& children = window->node.GetChildren();

			for (auto& n : children) {
				UnselectAllChildWidget(n);
			}
		}
	}

	void GridWindow::UnSelectAllWidgets()
	{
		std::vector<std::shared_ptr<ax::Window>>& children = win->node.GetChildren();
		for (auto& n : children) {
			UnselectAllChildWidget(n);
		}
		win->Update();
	}

	void GridWindow::OnWidgetIsDragging(const ax::event::EmptyMsg& msg)
	{
		if (_draw_grid_over_children == false) {
			_draw_grid_over_children = true;
			win->Update();
		}
	}

	void GridWindow::OnWidgetDoneDragging(const ax::event::EmptyMsg& msg)
	{
		_draw_grid_over_children = false;
		win->Update();
	}

	void GridWindow::OnMouseLeftDragging(const ax::Point& pos)
	{
		ax::Point m_pos(pos - win->dimension.GetAbsoluteRect().position);
		_selection.second.size = ax::Size((m_pos - _selection.second.position).ToPair());
		win->Update();
	}

	void GridWindow::OnMouseLeftUp(const ax::Point& pos)
	{
		if (win->event.IsGrabbed()) {
			win->event.UnGrabMouse();

			// Switch selection size when negative.
			if (_selection.second.size.w < 0) {
				_selection.second.position.x += _selection.second.size.w;
				_selection.second.size.w = -_selection.second.size.w;
			}

			if (_selection.second.size.h < 0) {
				_selection.second.position.y += _selection.second.size.h;
				_selection.second.size.h = -_selection.second.size.h;
			}

			// Look for selected widget.
			if (_selection.second.size.w > 0 && _selection.second.size.h > 0) {
				ax::Rect selection_rect = _selection.second;

				selection_rect.position += win->dimension.GetAbsoluteRect().position;

				std::vector<ax::Window*> selected
					= ax::NodeVisitor::AccumulateFromChild(win, [selection_rect](ax::Window* window) {
						  const ax::Rect abs_rect(window->dimension.GetAbsoluteRect());

						  if (selection_rect.IsPointInside(abs_rect.position)) {
							  return true;
						  }

						  return false;
					  });

				win->PushEvent(
					SELECT_MULTIPLE_WIDGET, new ax::event::SimpleMsg<std::vector<ax::Window*>>(selected));
			}

			_selection.first = false;
			win->Update();
		}
	}

	void GridWindow::OnResize(const ax::Size& size)
	{
		ax::Rect d_rect(win->dimension.GetDrawingRect());
		_horizontal_lines_array.clear();
		_vertical_lines_array.clear();
		_horizontal_lines_array.reserve((d_rect.size.h / _grid_space) * 2);
		_vertical_lines_array.reserve((d_rect.size.w / _grid_space) * 2);

		// Vertical lines.
		for (int x = _grid_space; x < d_rect.size.w; x += _grid_space) {
			_vertical_lines_array.push_back(ax::FPoint(x, 0));
			_vertical_lines_array.push_back(ax::FPoint(x, d_rect.size.h));
		}

		// Horizontal lines.
		for (int y = _grid_space; y < d_rect.size.h; y += _grid_space) {
			_horizontal_lines_array.push_back(ax::FPoint(0, y));
			_horizontal_lines_array.push_back(ax::FPoint(d_rect.size.w, y));
		}
	}

	void GridWindow::OnPaintOverChildren(ax::GC gc)
	{
		// Draw lines over children widgets.
		if (_draw_grid_over_children) {
			ax::Color line_color(at::Skin::GetInstance()->data.grid_window_lines);
			line_color.SetAlpha(0.1);
			gc.SetColor(line_color);
			gc.DrawLines(_horizontal_lines_array);
			gc.DrawLines(_vertical_lines_array);
		}

		// Selection rectangle.
		if (_selection.first) {
			gc.SetColor(ax::Color(0.8, 0.2));
			gc.DrawRectangle(_selection.second);
			gc.SetColor(ax::Color(0.8, 0.3));
			gc.DrawRectangleContour(_selection.second);
		}
	}

	void GridWindow::OnPaint(ax::GC gc)
	{
		const ax::Rect rect(win->dimension.GetDrawingRect());

		// Background.
		gc.SetColor(_bg_color);
		gc.DrawRectangle(rect);

		gc.SetColor(at::Skin::GetInstance()->data.grid_window_lines);
		gc.DrawLines(_horizontal_lines_array);
		gc.DrawLines(_vertical_lines_array);

		// Grid contour.
		gc.SetColor(at::Skin::GetInstance()->data.grid_window_contour);
		gc.DrawRectangleContour(rect);
	}
}
}
