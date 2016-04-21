//
//  atEditorLeftSideMenu.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-21.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef atEditorLeftSideMenu_hpp
#define atEditorLeftSideMenu_hpp

#include <OpenAX/OpenAX.h>

#include <OpenAX/Button.h>
#include <OpenAX/ScrollBar.h>

#include "atEditorWidgetMenu.h"
#include "atEditorWorkspace.h"
#include "atEditorProjectSpace.h"

namespace at {
namespace editor {

	class LeftSideMenu : public ax::Window::Backbone {
	public:
		LeftSideMenu(const ax::Rect& rect);
		
		enum : ax::Event::Id { SMALLER_MENU };
		
		void SetOnlyMainWindowWidgetSelectable();
		void SetAllSelectable();

	private:
		static const int TOP_BAR_HEIGHT = 25;
		bool _dropped_smaller = false;
		std::vector<ax::Button*> _menu_btns;
		
		ProjectSpace* _project_space;
		WidgetMenu* _widget_menu;
		Workspace* _workspace;
		
		
		axEVENT_DECLARATION(ax::Button::Msg, OnSmallerMenu);
		axEVENT_DECLARATION(ax::Button::Msg, OnWidgetList);
		axEVENT_DECLARATION(ax::Button::Msg, OnWorkspace);
		axEVENT_DECLARATION(ax::Button::Msg, OnProjectSpace);
		
		ax::Point AddButton(const ax::Point& pos, ax::Window* win, const ax::Button::Events& evts,
							const ax::Button::Info& info, const std::string& img, const std::string& description);
		
		void OnResize(const ax::Size& size);
		void OnPaint(ax::GC gc);
	};
}
}

#endif /* atEditorLeftSideMenu_hpp */
