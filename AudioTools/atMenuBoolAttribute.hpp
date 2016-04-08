//
//  atMenuBoolAttribute.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-07.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef atMenuBoolAttribute_hpp
#define atMenuBoolAttribute_hpp

#include <OpenAX/OpenAX.h>

namespace at {
namespace inspector {
	class BoolAttribute : public ax::Window::Backbone {
	public:
		enum Events : ax::Event::Id { ASSIGN_VALUE };

		BoolAttribute(
			const ax::Rect& rect, const std::string& name, const std::string& value, ax::Event::Function fct);

	private:
		std::string _name;
		void OnPaint(ax::GC gc);
	};
}
}

#endif /* atMenuBoolAttribute_hpp */
