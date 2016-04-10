//
//  atSkin.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-08.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef atSkin_hpp
#define atSkin_hpp

#include <OpenAX/OpenAX.h>


///ax::Rect rect(win->dimension.GetDrawingRect());
//gc.DrawRectangleColorFade(rect, ax::Color(1.0), ax::Color(0.98));

//if (_show_text) {
//	gc.SetColor(ax::Color(0.1));
//	gc.DrawString(_font, _title, ax::Point(75, 6));
//	
//	gc.SetColor(ax::Color(0.0));
//	gc.DrawString(_font_normal, _info, ax::Point(75, 20));
//	gc.DrawString(_font_normal, _size_str, ax::Point(75, 32));
//}
//
//gc.SetColor(ax::Color(0.9));
//gc.DrawRectangleContour(rect);
//
//// If not selectable.
//if(!_selectable) {
//	gc.SetColor(ax::Color(0.5, 0.45));
//	gc.DrawRectangle(rect);
//}
namespace at {

struct SkinData {
	// Status bar.
	ax::Color status_bar_bg;
	ax::Color status_bar_text;
	
	// Grid window.
	ax::Color grid_window_bg;
	ax::Color grid_window_contour;
	ax::Color grid_window_lines;
	
	// Widget menu.
	ax::Color w_menu_separator_bg;
	ax::Color w_menu_separator_contour;
	ax::Color w_menu_separator_text;
	
	ax::Color w_menu_top_bar;
	ax::Color w_menu_obj_bg_0;
	ax::Color w_menu_obj_bg_1;
	ax::Color w_menu_obj_contour;
	ax::Color w_menu_title_txt;
	ax::Color w_menu_txt;
	
	// Text editor.
	ax::Color txt_edit_bar;
	ax::Color txt_edit_bar_contour;
	ax::Color txt_edit_bar_label;
};

class Skin {
public:
	inline static Skin* GetInstance()
	{
		if (_global_skin == nullptr) {
			_global_skin = new Skin();
		}
		
		return _global_skin;
	}
	
	void SetLightSkin()
	{
		data.status_bar_bg = ax::Color(0.30);
		data.status_bar_text = ax::Color(1.0);
		
		data.grid_window_bg = ax::Color(1.0);
		data.grid_window_contour = ax::Color(0.7);
		data.grid_window_lines = ax::Color(0.9);
		
		// Widget menu.
		data.w_menu_separator_bg = ax::Color(0.94);
		data.w_menu_separator_contour = ax::Color(0.94);
		data.w_menu_separator_text = ax::Color(0.3);
		
		data.w_menu_top_bar = ax::Color(0.45);
		data.w_menu_obj_bg_0 = ax::Color(1.0);
		data.w_menu_obj_bg_1 = ax::Color(0.98);
		data.w_menu_obj_contour = ax::Color(0.9);
		data.w_menu_title_txt = ax::Color(0.1);
		data.w_menu_txt = ax::Color(0.0);
		
		data.txt_edit_bar = ax::Color(0.45);
		data.txt_edit_bar_contour = ax::Color(0.45);
		data.txt_edit_bar_label = ax::Color(1.0);
	}
	
	void SetDarkSkin()
	{
		data.status_bar_bg = ax::Color(0.30);
		data.status_bar_text = ax::Color(1.0);
		
		data.grid_window_bg = ax::Color(0.4);
		data.grid_window_contour = ax::Color(0.3);
		data.grid_window_lines = ax::Color(0.45);
		
		// Widget menu.
		data.w_menu_separator_bg = ax::Color(0.24);
		data.w_menu_separator_contour = ax::Color(0.22);
		data.w_menu_separator_text = ax::Color(0.8);
		
		data.w_menu_top_bar = ax::Color(0.25);
		data.w_menu_obj_bg_0 = ax::Color(0.3);
		data.w_menu_obj_bg_1 = ax::Color(0.27);
		data.w_menu_obj_contour = ax::Color(0.2);
		data.w_menu_title_txt = ax::Color(0.7);
		data.w_menu_txt = ax::Color(0.9);
		
		data.txt_edit_bar = ax::Color(0.25);
		data.txt_edit_bar_contour = ax::Color(0.25);
		data.txt_edit_bar_label = ax::Color(0.8);
	}
	
	SkinData data;

private:
	static Skin* _global_skin;
	
	Skin()
	{
	}
};
}

#endif /* atSkin_hpp */
