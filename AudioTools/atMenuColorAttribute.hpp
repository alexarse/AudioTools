//
//  atMenuColorAttribute.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-06.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef atMenuColorAttribute_hpp
#define atMenuColorAttribute_hpp

#include <OpenAX/OpenAX.h>
#include <OpenAX/ColorPicker.h>

namespace at {
namespace inspector {
	class ColorAttribute : public ax::Window::Backbone {
	public:
		enum Events : ax::Event::Id { ASSIGN_VALUE };

		ColorAttribute(
			const ax::Rect& rect, const std::string& name, const std::string& value, ax::Event::Function fct);
		

	private:
		std::string _name;
		ax::Color _color;
		ax::Font _font;
		
		axEVENT_DECLARATION(ax::ColorPicker::Msg, OnColorSelect);
		axEVENT_DECLARATION(ax::ColorPicker::Msg, OnColorCancel);

		void OnMouseLeftDown(const ax::Point& pos);
		void OnPaint(ax::GC gc);
	};
}
}

#endif /* atMenuColorAttribute_hpp */
