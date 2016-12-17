//
//  atMenuPathAttribute.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-28.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef atMenuPathAttribute_hpp
#define atMenuPathAttribute_hpp

#include <axlib/Button.hpp>
#include <axlib/axlib.hpp>

namespace at {
namespace inspector {
	class PathAttribute : public ax::Window::Backbone {
	public:
		enum Events : ax::event::Id { ASSIGN_VALUE };

		PathAttribute(
			const ax::Rect& rect, const std::string& name, const std::string& value, ax::event::Function fct);

	private:
		std::string _name;
		std::string _value;
		ax::Font _font;

		axEVENT_DECLARATION(ax::Button::Msg, OnOpenPath);
		void OnPaint(ax::GC gc);
	};
}
}

#endif /* atMenuPathAttribute_hpp */
