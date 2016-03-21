//
//  atInspectorMenuAttribute.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-03-12.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef atInspectorMenuAttribute_hpp
#define atInspectorMenuAttribute_hpp

#include <OpenAX/OpenAX.h>

namespace at {
namespace inspector {
	class MenuAttribute : public ax::Window::Backbone {
	public:
		
		enum Events : ax::Event::Id {
			ASSIGN_VALUE
		};
	
		MenuAttribute(
			const ax::Rect& rect, const std::string& name, const std::string& value, ax::Event::Function fct);

	private:
		std::string _name;
		std::string _value;
		ax::Font _font;

		void OnPaint(ax::GC gc);
	};
}
}

#endif /* atInspectorMenuAttribute_hpp */
