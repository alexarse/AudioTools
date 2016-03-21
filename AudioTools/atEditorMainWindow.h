/*
 * Copyright (c) 2016 Alexandre Arsenault.
 *
 * This file is part of axFrameworks.
 *
 * axFrameworks is free or commercial software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 or any later version of the
 * License or use a commercial axFrameworks License.
 *
 * axFrameworks is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with axFrameworks. If not, see <http://www.gnu.org/licenses/>.
 *
 * To release a closed-source product which uses axFrameworks, commercial
 * licenses are available, email alx.arsenault@gmail.com for more information.
 */

#ifndef __AX_EDITOR_MAIN_WINDOW_H__
#define __AX_EDITOR_MAIN_WINDOW_H__

#include "atEditorWidgetMenu.h"
#include "atEditorGridWindow.h"
#include "atEditorWidgetMenu.h"
#include "atEditorStatusBar.h"
#include "atEditorInspectorMenu.h"

class CodeEditor;

namespace at {
	namespace editor {

class MainWindow : public ax::Window::Backbone {
public:
	MainWindow(const ax::Rect& rect);
	
private:
	//	std::shared_ptr<StatusBar> _statusBar;
	static const int STATUS_BAR_HEIGHT = 30;
	static const int INSPECTOR_MENU_WIDTH = 250;
	static const int WIDGET_MENU_DROPPED_WIDTH = 85;
	static const int WIDGET_MENU_WIDTH = 250;
	static const int BOTTOM_BAR_HEIGHT = 18;
	
	StatusBar* _statusBar;
	std::shared_ptr<WidgetMenu> _widgetMenu;
	std::shared_ptr<GridWindow> _gridWindow;
	std::shared_ptr<CodeEditor> _codeEditor;
	std::shared_ptr<InspectorMenu> _inspectorMenu;
	std::vector<ax::Window::Ptr> _selected_windows;
	
	bool _has_tmp_widget;
	std::string _tmp_widget_builder_name;
	
	typedef std::pair<ax::StringPair, ax::Point> ObjMsg;
	
	struct ViewInfo {
		ax::Point old_main_window_position;
		bool left_menu_shown;
		bool right_menu_shown;
		bool editor_shown;
		ax::Size old_frame_size;
	};
	
	ViewInfo _view_info;
	bool _view_mode = false;
	
	axEVENT_DECLARATION(ax::Event::StringMsg, OnSaveLayout);
//	axEVENT_DECLARATION(ax::Event::StringMsg, OnSaveAsLayout);
	axEVENT_DECLARATION(ax::Event::StringMsg, OnOpenLayout);
	axEVENT_DECLARATION(ax::Event::SimpleMsg<int>, OnViewLayout);
	axEVENT_DECLARATION(ax::Event::SimpleMsg<int>, OnReloadScript);
	axEVENT_DECLARATION(ax::Button::Msg, OnBackToEditor);
	
	axEVENT_DECLARATION(ax::Event::SimpleMsg<int>, OnResizeCodeEditor);
	axEVENT_DECLARATION(ax::Toggle::Msg, OnToggleLeftPanel);
	axEVENT_DECLARATION(ax::Toggle::Msg, OnToggleBottomPanel);
	axEVENT_DECLARATION(ax::Toggle::Msg, OnToggleRightPanel);
	
	axEVENT_ACCESSOR(ax::Event::SimpleMsg<ObjMsg>, OnCreateDraggingWidget);
	axEVENT_ACCESSOR(ax::Event::SimpleMsg<ax::Point>, OnDraggingWidget);
	axEVENT_ACCESSOR(ax::Event::SimpleMsg<ax::Point>, OnReleaseObjWidget);
	
	axEVENT_DECLARATION(ax::Event::SimpleMsg<ax::Window*>, OnSelectWidget);
	axEVENT_DECLARATION(ax::Event::SimpleMsg<int>, OnUnSelectAllWidget);
	
	axEVENT_DECLARATION(ax::Button::Msg, OnSmallerLeftMenu);
	
	void OnCreateDraggingWidget(const ax::Event::SimpleMsg<ObjMsg>& msg);
	void OnDraggingWidget(const ax::Event::SimpleMsg<ax::Point>& msg);
	void OnReleaseObjWidget(const ax::Event::SimpleMsg<ax::Point>& msg);
	
	void OnResize(const ax::Size& size);
	
	void OnPaint(ax::GC gc);
};
}
}

#endif // __AX_EDITOR_MAIN_WINDOW_H__