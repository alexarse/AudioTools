//
//  atEditorWidgetMenuSeparator.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-21.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef atEditorWidgetMenuSeparator_hpp
#define atEditorWidgetMenuSeparator_hpp

#include <axlib/axlib.hpp>

namespace at {
namespace editor {
	class WidgetMenuSeparator : public ax::Window::Backbone {
	public:
		WidgetMenuSeparator(const ax::Rect& rect, const std::string& name);

	private:
		ax::Font _font;
		std::string _name;

		void OnMouseLeftDown(const ax::Point& pos);
		void OnMouseLeftDragging(const ax::Point& pos);
		void OnMouseLeftUp(const ax::Point& pos);
		void OnPaint(ax::GC gc);
	};
}
}

#endif /* atEditorWidgetMenuSeparator_hpp */
