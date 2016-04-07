//
//  atMenuSizeAttribute.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-07.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef atMenuSizeAttribute_hpp
#define atMenuSizeAttribute_hpp

#include <OpenAX/OpenAX.h>

namespace at {
namespace inspector {
	class SizeAttribute : public ax::Window::Backbone {
	public:
		enum Events : ax::Event::Id { ASSIGN_VALUE };

		SizeAttribute(
			const ax::Rect& rect, const std::string& name, const std::string& value, ax::Event::Function fct);

	private:
		std::string _name;
		//		ax::Color _color;
		ax::Font _font;

		//		axEVENT_DECLARATION(ax::ColorPicker::Msg, OnColorSelect);
		//		axEVENT_DECLARATION(ax::ColorPicker::Msg, OnColorCancel);

		//		void OnMouseLeftDown(const ax::Point& pos);
		void OnPaint(ax::GC gc);
	};
}
}

#endif /* atMenuSizeAttribute_hpp */
