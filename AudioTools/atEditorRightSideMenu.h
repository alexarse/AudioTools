//
//  atEditorRightSideMenu.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-19.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef atEditorRightSideMenu_hpp
#define atEditorRightSideMenu_hpp

#include <OpenAX/Button.h>
#include <OpenAX/OpenAX.h>

#include "atColorButton.h"
#include "atEditorAccount.h"
#include "atEditorInspectorMenu.h"
#include "atEditorProjectInfo.h"
#include "atEditorPyDoc.h"

namespace at {
namespace editor {
	/*
	 * RigthSideMenu.
	 */
	class RightSideMenu : public ax::Window::Backbone {
	public:
		RightSideMenu(const ax::Rect& rect);

		void SetInspectorHandle(ax::Window* handle);
		void RemoveInspectorHandle();

	private:
		static const int TOP_BAR_HEIGHT = 25;

		InspectorMenu* _inspector;
		ProjectInfo* _project_info;
		PyDoc* _pydoc;
		Account* _account;

		std::vector<at::ColorButton*> _btns;

		axEVENT_DECLARATION(ax::Button::Msg, OnInspectorButton);
		axEVENT_DECLARATION(ax::Button::Msg, OnProjectInfoButton);
		axEVENT_DECLARATION(ax::Button::Msg, OnPyDocButton);
		axEVENT_DECLARATION(ax::Button::Msg, OnAccountButton);

		ax::Point AddButton(const ax::Point& pos, ax::Window* win, const ax::Button::Events& evts,
			const ax::Button::Info& info, const std::string& img, const std::string& description);

		void OnResize(const ax::Size& size);
		void OnPaint(ax::GC gc);
	};
}
}

#endif /* atEditorRightSideMenu_hpp */
