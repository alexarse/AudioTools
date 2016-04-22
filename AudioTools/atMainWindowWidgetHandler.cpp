//
//  atMainWindowWidgetHandler.cpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-22.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#include "atEditorLoader.h"
#include "atEditorMainWindow.h"
#include "atHelpBar.h"
#include "atMainWindowWidgetHandler.h"

#include <OpenAX/Panel.h>
#include <OpenAX/WidgetLoader.h>

namespace at {
namespace editor {
	MainWindowWidgetHandler::MainWindowWidgetHandler(MainWindow* main_window)
		: _main_window(main_window)
		, _has_tmp_widget(false)
	{
	}

	void MainWindowWidgetHandler::DeleteCurrentWidgets()
	{
		ax::Window* win = _main_window->GetWindow();

		// @todo Remove multiple widgets.
		if (_main_window->_selected_windows.size()) {

			auto& children = _main_window->_selected_windows[0]->node.GetParent()->node.GetChildren();
			ax::Window::Ptr current_win;

			int index = -1;

			for (int i = 0; i < children.size(); i++) {
				if (children[i]->GetId() == _main_window->_selected_windows[0]->GetId()) {
					current_win = children[i];
					index = i;
					break;
				}
			}

			if (current_win && index != -1) {
				win->event.UnGrabMouse();
				ax::App::GetInstance().GetWindowManager()->ReleaseMouseHover();
				children.erase(children.begin() + index);
			}
		}

		_main_window->_selected_windows.clear();
		_main_window->_right_menu->RemoveInspectorHandle();

		if (_main_window->_gridWindow->GetMainWindow() == nullptr) {
			_main_window->_left_menu->SetOnlyMainWindowWidgetSelectable();
		}
	}

	void MainWindowWidgetHandler::OnSelectWidget(const ax::Event::SimpleMsg<ax::Window*>& msg)
	{
		ax::Window* selected_win = msg.GetMsg();
		_main_window->_selected_windows.clear();

		_main_window->_gridWindow->UnSelectAllWidgets();

		if (selected_win != nullptr) {
			_main_window->_selected_windows.push_back(selected_win);
			selected_win->property.AddProperty("current_editing_widget");
			selected_win->Update();
			_main_window->_right_menu->SetInspectorHandle(selected_win);
		}
		else {
			_main_window->_right_menu->SetInspectorHandle(selected_win);
		}

		if (_main_window->_gridWindow->GetMainWindow() == nullptr) {
			_main_window->_left_menu->SetOnlyMainWindowWidgetSelectable();
		}
	}

	void MainWindowWidgetHandler::OnUnSelectAllWidget(const ax::Event::SimpleMsg<int>& msg)
	{
		_main_window->_selected_windows.clear();
		_main_window->_right_menu->RemoveInspectorHandle();
	}

	void MainWindowWidgetHandler::OnCreateDraggingWidget(const ax::Event::SimpleMsg<ObjMsg>& msg)
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

	void MainWindowWidgetHandler::OnDraggingWidget(const ax::Event::SimpleMsg<ax::Point>& msg)
	{
		if (_has_tmp_widget) {
			ax::Point pos(msg.GetMsg());
			ax::Window::Ptr wobj = ax::App::GetInstance().GetPopupManager()->GetWindowTree()->GetTopLevel();

			if (wobj) {
				wobj->dimension.SetPosition(pos);
			}
		}
	}

	void MainWindowWidgetHandler::OnReleaseObjWidget(const ax::Event::SimpleMsg<ax::Point>& msg)
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
			ax::Window* main_window = _main_window->_gridWindow->GetMainWindow();

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

			bool inside_grid(
				_main_window->_gridWindow->GetWindow()->dimension.GetAbsoluteRect().IsPointInside(pos));

			// Not dragging above grid window then do nothing.
			if (!inside_grid) {
				// The temporary widget will be deleted.
				return;
			}

			_main_window->_left_menu->SetAllSelectable();

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
				Loader loader(_main_window->_gridWindow->GetWindow());
				loader.SetupExistingWidget(widget_win.get(), _tmp_widget_builder_name);

				_main_window->_selected_windows.clear();
				_main_window->_gridWindow->UnSelectAllWidgets();

				if (widget_win != nullptr) {
					widget_win->property.AddProperty("current_editing_widget");
					widget_win->Update();
					_main_window->_selected_windows.push_back(widget_win.get());
					//					_inspectorMenu->SetWidgetHandle(widget_win.get());
					_main_window->_right_menu->SetInspectorHandle(widget_win.get());
				}
			}

			else {
				// Reparent.
				widget_win->node.SetParent(_main_window->_gridWindow->GetWindow());
				_main_window->_gridWindow->GetWindow()->node.GetChildren().push_back(widget_win);

				widget_win->dimension.SetPosition(
					pos - _main_window->_gridWindow->GetWindow()->dimension.GetAbsoluteRect().position);

				// Setup widget.
				Loader loader(_main_window->_gridWindow->GetWindow());
				loader.SetupExistingWidget(widget_win.get(), _tmp_widget_builder_name);

				_main_window->_gridWindow->UnSelectAllWidgets();
				_main_window->_selected_windows.clear();

				if (widget_win != nullptr) {
					widget_win->property.AddProperty("current_editing_widget");
					widget_win->Update();
					_main_window->_selected_windows.push_back(widget_win.get());
					//					_inspectorMenu->SetWidgetHandle(widget_win.get());
					_main_window->_right_menu->SetInspectorHandle(widget_win.get());
				}
			}
		}
	}

	void MainWindowWidgetHandler::OnDeleteSelectedWidget(const ax::Event::EmptyMsg& msg)
	{
		if (_main_window->_selected_windows.size()) {
			ax::Print("Should delete widget.");
			DeleteCurrentWidgets();
		}
	}

	void MainWindowWidgetHandler::OnDuplicateSelectedWidget(const ax::Event::EmptyMsg& msg)
	{
//		MainWindow* main_win = at::editor::App::GetInstance()->GetMainWindow();

		std::vector<ax::Window*> sel_wins = _main_window->GetSelectedWindows();

		if (sel_wins.size()) {
			// Copy selected widgets.
			std::shared_ptr<ax::Window::Backbone> bck_bone(sel_wins[0]->backbone->GetCopy());

			if (bck_bone == nullptr) {
				return;
			}

			const ax::Rect rect(sel_wins[0]->dimension.GetRect());

			bck_bone->GetWindow()->dimension.SetPosition(rect.position + ax::Point(rect.size.x + 2, 0));

			at::editor::Loader loader(_main_window->_gridWindow->GetWindow());
			ax::widget::Component* widget
				= static_cast<ax::widget::Component*>(bck_bone->GetWindow()->component.Get("Widget").get());

			ax::Window* parent = sel_wins[0]->node.GetParent();

			if (parent == nullptr) {
				return;
			}
			
			// Can't duplicate main panel widget.
			if (parent->GetId() == _main_window->_gridWindow->GetWindow()->GetId()) {
				return;
			}

			parent->node.Add(bck_bone);
			loader.SetupExistingWidget(bck_bone->GetWindow(), widget->GetBuilderName());
		}
	}
}
}