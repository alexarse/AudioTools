//
//  atColorButton.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-21.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef atColorButton_hpp
#define atColorButton_hpp

#include <OpenAX/Button.h>

namespace at {
	class ColorButton : public ax::Button {
	public:
		ColorButton(const ax::Rect& rect, const ax::Button::Events& events, const ax::Button::Info& info,
					std::string img_path = "", std::string label = "", ax::Flag flags = 0, std::string msg = "");
		
		inline void SetSelected(bool sel)
		{
			if(_selected != sel) {
				_selected = sel;
				win->Update();
			}
		}
		
		inline void SetColor(const ax::Color& color)
		{
			_color = color;
			win->Update();
		}
		
	private:
		bool _selected = false;
		ax::Color _color = ax::Color(250, 172, 0);
	
		void OnCustomPaint(ax::GC gc);
	};
}

#endif /* atColorButton_hpp */
