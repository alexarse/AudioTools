//
//  atEditorRightSideMenu.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-19.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef atEditorRightSideMenu_hpp
#define atEditorRightSideMenu_hpp

#include <OpenAX/OpenAX.h>

#include "atEditorInspectorMenu.h"

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
	
		void OnResize(const ax::Size& size);
		void OnPaint(ax::GC gc);
	};
}
}

#endif /* atEditorRightSideMenu_hpp */
