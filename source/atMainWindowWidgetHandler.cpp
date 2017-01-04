//
//  atMainWindowWidgetHandler.cpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-22.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#include "atMainWindowWidgetHandler.h"
#include "atHelpBar.h"
#include "editor/atEditorLoader.hpp"
#include "editor/atEditorMainWindow.hpp"

#include <axlib/Panel.hpp>
#include <axlib/WidgetLoader.hpp>
#include <fst/print.h>

#include "editor/atEditor.hpp"
#include "editor/atEditorGridWindow.hpp"
//#include "editor/atEditorMainWindow.hpp"

namespace at {
namespace editor {
	MainWindowWidgetHandler::MainWindowWidgetHandler(MainWindow* main_window)
		: _main_window(main_window)
		, _has_tmp_widget(false)
	{
	}

	void MainWindowWidgetHandler::DeleteCurrentWidgets()
	{
		// Remove all selected widgets.
		for (auto& n : _main_window->_selected_windows) {
			n->RemoveWindow();
		}

		// Clear selected widget vector.
		_main_window->_selected_windows.clear();

		_main_window->_right_menu->SetMultipleWidgetSelected(false);
		_main_window->_right_menu->RemoveInspectorHandle();

		if (_main_window->_gridWindow->GetMainWindow() == nullptr) {
			_main_window->_left_menu->SetOnlyMainWindowWidgetSelectable();
			_main_window->GetWindow()->PushEvent(
				MainWindow::HAS_WIDGET_ON_GRID, new ax::event::SimpleMsg<bool>(false));
		}
	}

	void MainWindowWidgetHandler::OnSelectWidget(const ax::event::SimpleMsg<ax::Window*>& msg)
	{
		ax::Window* selected_win = msg.GetMsg();
		_main_window->_selected_windows.clear();

		_main_window->_right_menu->SetMultipleWidgetSelected(false);
		_main_window->_gridWindow->UnSelectAllWidgets();

		if (selected_win != nullptr) {
			_main_window->_selected_windows.push_back(selected_win);
			selected_win->property.AddProperty("current_editing_widget");
			selected_win->Update();
			_main_window->_right_menu->SetInspectorHandle(selected_win);

			selected_win->event.GrabKey();
		}
		else {
			_main_window->_right_menu->SetInspectorHandle(selected_win);
		}

		if (_main_window->_gridWindow->GetMainWindow() == nullptr) {
			_main_window->_left_menu->SetOnlyMainWindowWidgetSelectable();
			_main_window->GetWindow()->PushEvent(
				MainWindow::HAS_WIDGET_ON_GRID, new ax::event::SimpleMsg<bool>(false));
		}
	}

	void MainWindowWidgetHandler::OnUnSelectAllWidget(const ax::event::SimpleMsg<int>& msg)
	{
		_main_window->_selected_windows.clear();
		_main_window->_right_menu->RemoveInspectorHandle();
	}

	void MainWindowWidgetHandler::OnCreateDraggingWidget(const ax::event::SimpleMsg<ObjMsg>& msg)
	{
		ax::console::Print("OnCreateDraggingWidget object.");

		std::pair<std::string, std::string> obj_info = msg.GetMsg().first;
		std::string builder_name = obj_info.first;
		std::string file_path = obj_info.second;
		ax::Point pos(msg.GetMsg().second);

		ax::widget::Loader* loader = ax::widget::Loader::GetInstance();
		ax::widget::Builder* builder = loader->GetBuilder(builder_name);

		fst::print(ptrace, "Builder name :", builder_name);

		if (builder == nullptr) {
			ax::console::Error("Builder", builder_name, "doesn't exist.");
			return;
		}

		ax::App& app(ax::App::GetInstance());
		app.GetPopupManager()->Clear();

		auto obj(builder->Create(pos, file_path));

		if (obj != nullptr) {

			app.AddPopupTopLevel(obj);

			obj->GetWindow()->property.RemoveProperty("Selectable");
			_has_tmp_widget = true;
			_tmp_widget_builder_name = builder_name;
		}
		//		ax::App::GetInstance().GetPopupManager()->GetWindowTree()->AddTopLevel(
		//			std::shared_ptr<ax::Window>(obj->GetWindow()));
		//		obj->GetWindow()->backbone = obj;

		//		_has_tmp_widget = true;
		//		_tmp_widget_builder_name = builder_name;
	}

	void MainWindowWidgetHandler::OnCreateCustomDraggingWidget(const ax::event::SimpleMsg<ObjMsg>& msg)
	{
		ax::console::Print("OnCreateCustomDraggingWidget object.");

		std::pair<std::string, std::string> obj_info = msg.GetMsg().first;
		//		std::string builder_name = obj_info.first;
		std::string file_path = obj_info.second;
		ax::Point pos(msg.GetMsg().second);

		ax::widget::Loader* loader = ax::widget::Loader::GetInstance();
		ax::Xml xml(file_path);

		if (xml.Parse() == false) {
			return;
		}

		ax::Xml::Node cwidget = xml.GetNode("CustomWidget");
		ax::Xml::Node wnode = cwidget.GetNode("Widget");
		wnode.AddNode(xml.CreateNode("position", pos.ToString()));
		std::string builder_name = wnode.GetAttribute("builder");
		ax::widget::Builder* builder = loader->GetBuilder(builder_name);

		fst::print(ptrace, "Builder name :", builder_name);

		if (builder == nullptr) {
			ax::console::Error("Builder", builder_name, "doesn't exist.");
			return;
		}

		ax::App& app(ax::App::GetInstance());
		app.GetPopupManager()->Clear();

		builder->SetCreateCallback([](ax::Window* wwin, ax::Xml::Node& node) {

			std::string builder_name = node.GetAttribute("builder");
			std::string pyo_fct_name;
			ax::console::Print("Create callback", builder_name);

			ax::Xml::Node pyo_node = node.GetNode("pyo");

			if (pyo_node.IsValid()) {
				pyo_fct_name = pyo_node.GetValue();
			}

			std::string unique_name;
			ax::Xml::Node unique_name_node = node.GetNode("unique_name");

			if (unique_name_node.IsValid()) {
				unique_name = unique_name_node.GetValue();
			}

			Loader loader(at::editor::App::GetInstance()->GetMainWindow()->GetGridWindow()->GetWindow());
			loader.SetupExistingWidget(wwin, builder_name, pyo_fct_name, unique_name);
			wwin->property.AddProperty("Selectable");
		});

		auto obj(builder->Create(wnode));

		if (obj != nullptr) {

			app.AddPopupTopLevel(obj);

			obj->GetWindow()->property.RemoveProperty("Selectable");
			_has_tmp_widget = true;
			_tmp_widget_builder_name = builder_name;
		}
		//		ax::App::GetInstance().GetPopupManager()->GetWindowTree()->AddTopLevel(
		//			std::shared_ptr<ax::Window>(obj->GetWindow()));
		//		obj->GetWindow()->backbone = obj;

		//		_has_tmp_widget = true;
		//		_tmp_widget_builder_name = builder_name;
	}

	void MainWindowWidgetHandler::OnDraggingWidget(const ax::event::SimpleMsg<ax::Point>& msg)
	{
		if (_has_tmp_widget) {
			ax::Point pos(msg.GetMsg());

			std::shared_ptr<ax::Window> wobj
				= ax::App::GetInstance().GetPopupManager()->GetWindowTree()->GetTopLevel();

			if (wobj) {

				at::editor::GridSnapProxy gsp
					= at::editor::App::GetInstance()->GetMainWindow()->GetGridSnapProxy();

				if (gsp.IsSnapActive()) {
					auto gw = at::editor::App::GetInstance()->GetMainWindow()->GetGridWindow()->GetWindow();
					const ax::Point gw_abs_pos(gw->dimension.GetAbsoluteRect().position);

					if (gw->dimension.GetAbsoluteRect().IsPointInside(pos)) {
						pos = gsp.FindClosestPosition(pos - gw_abs_pos) + gw_abs_pos;
					}
				}

				wobj->dimension.SetPosition(pos);
			}
		}
	}

	void MainWindowWidgetHandler::OnReleaseObjWidget(const ax::event::SimpleMsg<ax::Point>& msg)
	{
		ax::Point pos(msg.GetMsg());

		at::editor::GridSnapProxy gsp = at::editor::App::GetInstance()->GetMainWindow()->GetGridSnapProxy();

		if (gsp.IsSnapActive()) {
			auto gw = at::editor::App::GetInstance()->GetMainWindow()->GetGridWindow()->GetWindow();
			const ax::Point gw_abs_pos(gw->dimension.GetAbsoluteRect().position);

			if (gw->dimension.GetAbsoluteRect().IsPointInside(pos)) {
				pos = gsp.FindClosestPosition(pos - gw_abs_pos) + gw_abs_pos;
			}
		}

		ax::console::Print("Release object.");

		if (_has_tmp_widget) {
			_has_tmp_widget = false;

			std::vector<std::shared_ptr<ax::Window>>& nodes
				= ax::App::GetInstance().GetPopupManager()->GetWindowTree()->GetNodeVector();

			std::shared_ptr<ax::Window> widget_win = nodes[0];
			widget_win = widget_win->RemoveWindow();

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
					ax::console::Print("A MainWindow Panel shall be created first to add widget.");
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
			_main_window->GetWindow()->PushEvent(
				MainWindow::HAS_WIDGET_ON_GRID, new ax::event::SimpleMsg<bool>(true));

			if (widget_win->GetId() != main_window->GetId()) {
				bool inside_main_window = main_window->dimension.GetAbsoluteRect().IsPointInside(pos);

				if (!inside_main_window) {
					// The temporary widget will be deleted.
					ax::console::Print("Drag widget over the MainWindow.");
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
				// Reparent.
				hover_window->node.Add(widget_win);
				widget_win->dimension.SetPosition(pos - hover_window->dimension.GetAbsoluteRect().position);
				//				widget_win->property.AddProperty("Selectable");
				//				widget_win->node.SetParent(hover_window);
				//				hover_window->node.GetChildren().push_back(widget_win);
				//				widget_win->dimension.SetPosition(pos -
				// hover_window->dimension.GetAbsoluteRect().position);

				// Setup widget.
				Loader loader(_main_window->_gridWindow->GetWindow());
				loader.SetupExistingWidget(widget_win.get(), _tmp_widget_builder_name);

				_main_window->_selected_windows.clear();
				_main_window->_gridWindow->UnSelectAllWidgets();

				if (widget_win != nullptr) {
					widget_win->property.AddProperty("current_editing_widget");
					widget_win->property.AddProperty("Selectable");
					widget_win->Update();
					_main_window->_selected_windows.push_back(widget_win.get());
					_main_window->_right_menu->SetInspectorHandle(widget_win.get());
				}
			}

			else {

				// Reparent.
				_main_window->_gridWindow->GetWindow()->node.Add(widget_win);

				widget_win->dimension.SetPosition(
					pos - _main_window->_gridWindow->GetWindow()->dimension.GetAbsoluteRect().position);

				//				widget_win->dimension.SetPosition(pos -
				// hover_window->dimension.GetAbsoluteRect().position);

				//				widget_win->node.SetParent(_main_window->_gridWindow->GetWindow());
				//				_main_window->_gridWindow->GetWindow()->node.GetChildren().push_back(widget_win);

				// Setup widget.
				Loader loader(_main_window->_gridWindow->GetWindow());
				loader.SetupExistingWidget(widget_win.get(), _tmp_widget_builder_name);

				_main_window->_gridWindow->UnSelectAllWidgets();
				_main_window->_selected_windows.clear();

				if (widget_win != nullptr) {
					widget_win->property.AddProperty("current_editing_widget");
					widget_win->property.AddProperty("Selectable");
					widget_win->Update();

					_main_window->_selected_windows.push_back(widget_win.get());
					_main_window->_right_menu->SetInspectorHandle(widget_win.get());
				}
			}
		}
	}

	void MainWindowWidgetHandler::OnDeleteSelectedWidget(const ax::event::EmptyMsg& msg)
	{
		if (_main_window->_selected_windows.size()) {

			ax::console::Print("Remove selected widget.");
			DeleteCurrentWidgets();
		}
	}

	void MainWindowWidgetHandler::OnDuplicateSelectedWidget(const ax::event::EmptyMsg& msg)
	{
		std::vector<ax::Window*> sel_wins = _main_window->GetSelectedWindows();

		if (sel_wins.size()) {
			// Copy selected widgets.
			std::shared_ptr<ax::Window::Backbone> bck_bone(sel_wins[0]->backbone->GetCopy());

			if (bck_bone == nullptr) {
				return;
			}

			const ax::Rect rect(sel_wins[0]->dimension.GetRect());

			bck_bone->GetWindow()->dimension.SetPosition(rect.position + ax::Point(rect.size.w + 2, 0));

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

			OnSelectWidget(ax::event::SimpleMsg<ax::Window*>(bck_bone->GetWindow()));
		}
	}

	void MainWindowWidgetHandler::OnSelectMultipleWidget(
		const ax::event::SimpleMsg<std::vector<ax::Window*>>& msg)
	{
		_main_window->_gridWindow->UnSelectAllWidgets();

		ax::console::Print("Select multiple widget.");

		std::vector<ax::Window*> selected = msg.GetMsg();

		for (auto& n : selected) {
			n->property.AddProperty("current_editing_widget");
			n->Update();
		}

		_main_window->_selected_windows = selected;

		if (selected.size() > 1) {
			_main_window->_right_menu->SetMultipleWidgetSelected(true);
		}
		else {
			_main_window->_right_menu->SetMultipleWidgetSelected(false);
		}
	}

	void MainWindowWidgetHandler::OnArrowMoveSelectedWidget(
		const ax::event::SimpleMsg<ax::util::Direction>& msg)
	{
		const ax::util::Direction dir = msg.GetMsg();

		for (auto& n : _main_window->_selected_windows) {
			const ax::Rect& w_rect = n->dimension.GetRect();

			if (dir == ax::util::Direction::LEFT) {
				n->dimension.SetPosition(w_rect.position - ax::Point(1, 0));
			}
			else if (dir == ax::util::Direction::RIGHT) {
				n->dimension.SetPosition(w_rect.position + ax::Point(1, 0));
			}
			else if (dir == ax::util::Direction::UP) {
				n->dimension.SetPosition(w_rect.position - ax::Point(0, 1));
			}
			// Down.
			else {
				n->dimension.SetPosition(w_rect.position + ax::Point(0, 1));
			}
		}
	}
}
}
