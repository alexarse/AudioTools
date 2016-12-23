//
//  atMainWindowViewHandler.cpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-22.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#include "atMainWindowViewHandler.h"
#include "atHelpBar.h"
#include "editor/atEditorMainWindow.hpp"

namespace at {
namespace editor {
	MainWindowViewHandler::MainWindowViewHandler(MainWindow* main_window)
		: _main_window(main_window)
	{
	}

	void MainWindowViewHandler::OnViewLayout(const ax::event::SimpleMsg<int>& msg)
	{
		//		ax::Window* win = _main_window->GetWindow();

		ax::console::Print("Main window view.");
		ax::Window* main_win = _main_window->_gridWindow->GetMainWindow();

		if (main_win == nullptr) {
			ax::console::Print("No main window setup.");
			/// @todo Add a message box to tell user.
			return;
		}

		ax::Rect rect(main_win->dimension.GetRect());

		_view_info.old_frame_size = ax::App::GetInstance().GetFrameSize();
		_view_info.old_main_window_position = rect.position;
		_view_info.left_menu_shown = _main_window->_left_menu->GetWindow()->IsShown();
		_view_info.right_menu_shown = _main_window->_right_menu->GetWindow()->IsShown();
		_view_info.editor_shown = _main_window->_bottom_section->GetWindow()->IsShown();

		main_win->dimension.SetPosition(ax::Point(0, 0));
		_main_window->_left_menu->GetWindow()->Hide();
		_main_window->_right_menu->GetWindow()->Hide();
		_main_window->_bottom_section->GetWindow()->Hide();
		_main_window->_statusBar->GetWindow()->Hide();

		_main_window->_gridWindow->GetWindow()->dimension.SetPosition(ax::Point(0, 0));
		_main_window->_gridWindow->GetWindow()->dimension.SetSize(main_win->dimension.GetSize());
		_view_mode = true;

		ax::App::GetInstance().SetFrameSize(rect.size);

		ax::Button::Info btn_info;
		btn_info.normal = ax::Color(0.0, 0.0);
		btn_info.hover = ax::Color(0.0, 0.0);
		btn_info.clicking = ax::Color(0.0, 0.0);
		btn_info.selected = ax::Color(0.0, 0.0);
		btn_info.contour = ax::Color(0.0, 0.0);
		btn_info.font_color = ax::Color(0.0, 0.0);

		ax::Point back_btn_pos((rect.size - ax::Size(25, 25)).ToPair());

		auto tmp_back_btn = ax::shared<ax::Button>(ax::Rect(back_btn_pos, ax::Size(25, 25)),
			GetOnBackToEditor(), btn_info, "resources/back.png", "", ax::Button::Flags::SINGLE_IMG);

		tmp_back_btn->GetWindow()->property.AddProperty("TemporaryBackButton");

		main_win->node.Add(tmp_back_btn);

		_main_window->_selected_windows.clear();
		_main_window->_gridWindow->UnSelectAllWidgets();
		_main_window->_right_menu->SetInspectorHandle(nullptr);

		ax::App::GetInstance().SetResizable(false);
		ax::App::GetInstance().SetFocusAndCenter();
	}

	void MainWindowViewHandler::OnBackToEditor(const ax::Button::Msg& msg)
	{
		_view_mode = false;

		_main_window->_statusBar->GetWindow()->Show();

		if (_view_info.left_menu_shown) {
			_main_window->_left_menu->GetWindow()->Show();
		}

		if (_view_info.right_menu_shown) {
			//			_inspectorMenu->GetWindow()->Show();
			_main_window->_right_menu->GetWindow()->Show();
		}

		if (_view_info.editor_shown) {
			//			_codeEditor->GetWindow()->Show();
			_main_window->_bottom_section->GetWindow()->Show();
		}

		ax::Window* main_win = _main_window->_gridWindow->GetMainWindow();
		main_win->dimension.SetPosition(_view_info.old_main_window_position);

		_main_window->_gridWindow->GetWindow()->dimension.SetPosition(
			ax::Point(_main_window->WIDGET_MENU_WIDTH, _main_window->STATUS_BAR_HEIGHT));

		// Find tmp back button and delete it.
		ax::Window* main_window = _main_window->_gridWindow->GetMainWindow();

		if (main_window == nullptr) {
			ax::console::Error("Main window doesn't exist when comming back from view mode.");
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
			ax::console::Error("Back to editor button wasn't found when comming back "
							   "from view mode.");
		}

		ax::App::GetInstance().SetResizable(true);
		ax::App::GetInstance().SetFrameSize(_view_info.old_frame_size);
		ax::App::GetInstance().SetFocusAndCenter();
	}

	void MainWindowViewHandler::OnResizeCodeEditor(const ax::event::SimpleMsg<int>& msg)
	{
		ax::Window* win = _main_window->GetWindow();
		win->event.OnResize(win->dimension.GetSize());
	}

	void MainWindowViewHandler::OnToggleLeftPanel(const ax::Toggle::Msg& msg)
	{
		ax::Window* win = _main_window->GetWindow();
		ax::Window* w = _main_window->_left_menu->GetWindow();

		if (w->IsShown()) {
			w->Hide();
		}
		else {
			w->Show();
		}

		win->event.OnResize(win->dimension.GetSize());
	}

	void MainWindowViewHandler::OnToggleBottomPanel(const ax::Toggle::Msg& msg)
	{
		ax::Window* win = _main_window->GetWindow();
		ax::Window* w = _main_window->_bottom_section->GetWindow();

		if (w->IsShown()) {
			w->Hide();
		}
		else {
			w->Show();
		}

		win->event.OnResize(win->dimension.GetSize());
	}

	void MainWindowViewHandler::OnToggleRightPanel(const ax::Toggle::Msg& msg)
	{
		ax::Window* win = _main_window->GetWindow();
		ax::Window* w = _main_window->_right_menu->GetWindow();

		if (w->IsShown()) {
			w->Hide();
		}
		else {
			w->Show();
		}

		win->event.OnResize(win->dimension.GetSize());
	}

	void MainWindowViewHandler::OnSmallerLeftMenu(const ax::Button::Msg& msg)
	{
		ax::Window* win = _main_window->GetWindow();
		win->event.OnResize(win->dimension.GetSize());
	}

	void MainWindowViewHandler::OnResize(const ax::Size& size)
	{
		if (_view_mode) {
			return;
		}

		int INSPECTOR_MENU_WIDTH = _main_window->INSPECTOR_MENU_WIDTH;
		int STATUS_BAR_HEIGHT = _main_window->STATUS_BAR_HEIGHT;
		int BOTTOM_BAR_HEIGHT = _main_window->BOTTOM_BAR_HEIGHT;

		// Resize status bar.
		ax::Size top_menu_size(_main_window->_statusBar->GetWindow()->dimension.GetSize());
		top_menu_size.w = size.w;
		_main_window->_statusBar->GetWindow()->dimension.SetSize(top_menu_size);

		bool widget_menu = _main_window->_left_menu->GetWindow()->IsShown();
		bool inspector = _main_window->_right_menu->GetWindow()->IsShown();
		bool code_editor = _main_window->_bottom_section->GetWindow()->IsShown();

		int editor_height = 0;

		if (code_editor) {
			editor_height = _main_window->_bottom_section->GetWindow()->dimension.GetSize().h;
			if (editor_height > size.h - _main_window->STATUS_BAR_HEIGHT) {
				editor_height = size.h - _main_window->STATUS_BAR_HEIGHT;
			}
		}

		int grid_height
			= size.h - _main_window->STATUS_BAR_HEIGHT - editor_height - _main_window->BOTTOM_BAR_HEIGHT;

		int widget_menu_width = _main_window->_left_menu->GetWindow()->dimension.GetRect().size.w;

		if (widget_menu && inspector) {
			ax::Size widget_menu_size(widget_menu_width,
				size.h - _main_window->STATUS_BAR_HEIGHT - _main_window->BOTTOM_BAR_HEIGHT);
			_main_window->_left_menu->GetWindow()->dimension.SetSize(widget_menu_size);

			ax::Rect grid_rect(widget_menu_width, _main_window->STATUS_BAR_HEIGHT,
				size.w - widget_menu_width - _main_window->INSPECTOR_MENU_WIDTH, grid_height);
			_main_window->_gridWindow->GetWindow()->dimension.SetRect(grid_rect);

			ax::Rect info_rect(size.w - _main_window->INSPECTOR_MENU_WIDTH, _main_window->STATUS_BAR_HEIGHT,
				_main_window->INSPECTOR_MENU_WIDTH,
				size.h - _main_window->STATUS_BAR_HEIGHT - _main_window->BOTTOM_BAR_HEIGHT);
			_main_window->_right_menu->GetWindow()->dimension.SetRect(info_rect);

			if (code_editor) {
				ax::Rect editor_rect(widget_menu_width + 1,
					size.h - editor_height - _main_window->BOTTOM_BAR_HEIGHT,
					size.w - widget_menu_width - _main_window->INSPECTOR_MENU_WIDTH, editor_height);
				_main_window->_bottom_section->GetWindow()->dimension.SetRect(editor_rect);
			}
		}
		else if (widget_menu) {
			ax::Size widget_menu_size(widget_menu_width, size.h - STATUS_BAR_HEIGHT - BOTTOM_BAR_HEIGHT);
			_main_window->_left_menu->GetWindow()->dimension.SetSize(widget_menu_size);

			ax::Rect grid_rect(
				widget_menu_width, _main_window->STATUS_BAR_HEIGHT, size.w - widget_menu_width, grid_height);
			_main_window->_gridWindow->GetWindow()->dimension.SetRect(grid_rect);

			if (code_editor) {
				ax::Rect editor_rect(widget_menu_width + 1, size.h - editor_height - BOTTOM_BAR_HEIGHT,
					size.w - widget_menu_width, editor_height);

				_main_window->_bottom_section->GetWindow()->dimension.SetRect(editor_rect);
			}
		}
		else if (inspector) {
			ax::Rect grid_rect(0, STATUS_BAR_HEIGHT, size.w - INSPECTOR_MENU_WIDTH, grid_height);
			_main_window->_gridWindow->GetWindow()->dimension.SetRect(grid_rect);

			ax::Rect info_rect(size.w - INSPECTOR_MENU_WIDTH, STATUS_BAR_HEIGHT, INSPECTOR_MENU_WIDTH,
				size.h - STATUS_BAR_HEIGHT - BOTTOM_BAR_HEIGHT);
			_main_window->_right_menu->GetWindow()->dimension.SetRect(info_rect);

			if (code_editor) {
				ax::Rect editor_rect(1, size.h - editor_height - BOTTOM_BAR_HEIGHT,
					size.w - INSPECTOR_MENU_WIDTH, editor_height);

				_main_window->_bottom_section->GetWindow()->dimension.SetRect(editor_rect);
			}
		}
		else {
			ax::Rect grid_rect(0, STATUS_BAR_HEIGHT, size.w, grid_height);
			_main_window->_gridWindow->GetWindow()->dimension.SetRect(grid_rect);

			if (code_editor) {
				ax::Rect editor_rect(1, size.h - editor_height - BOTTOM_BAR_HEIGHT, size.w, editor_height);

				_main_window->_bottom_section->GetWindow()->dimension.SetRect(editor_rect);
			}
		}

		// Midi feedback.
		_main_window->_midi_feedback->GetWindow()->dimension.SetPosition(ax::Point(size.w - 17, size.h - 15));

		AttachHelpInfo(_main_window->_midi_feedback->GetWindow(), "Midi input activity.");
	}
}
}
