//
//  atMenuIntegerAttribute.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-08.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef atMenuIntegerAttribute_hpp
#define atMenuIntegerAttribute_hpp

#include <OpenAX/NumberScroll.h>
#include <OpenAX/OpenAX.h>

namespace at {
namespace inspector {
	class IntegerAttribute : public ax::Window::Backbone {
	public:
		enum Events : ax::Event::Id { ASSIGN_VALUE };

		IntegerAttribute(
			const ax::Rect& rect, const std::string& name, const std::string& value, ax::Event::Function fct);

	private:
		std::string _name;
//		ax::Font _font;
//		ax::NumberScroll* _width_scroll;
//		ax::NumberScroll* _height_scroll;

		axEVENT_DECLARATION(ax::NumberScroll::Msg, OnValueChange);
//		axEVENT_DECLARATION(ax::NumberScroll::Msg, OnHeightChange);

		void OnPaint(ax::GC gc);
	};
}
}

#endif /* atMenuIntegerAttribute_hpp */
