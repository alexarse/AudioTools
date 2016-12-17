//
//  atEditorPyDocElement.cpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-19.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#include "editor/atEditorPyDocElement.hpp"
#include "atSkin.hpp"

namespace at {
namespace editor {
	PyDocElement::PyDocElement(const ax::Rect& rect, const std::string& name, const std::string& description)
		: _font("fonts/Lato.ttf")
		, _font_normal("fonts/LatoLight.ttf")
		, _name(name)
		, _description(description)
	{
		// Create window.
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &PyDocElement::OnPaint);

		int n_char_per_line = 40;
		int str_size = (int)description.size();
		int n_separation = ceil(str_size / (double)n_char_per_line);

		for (int i = 0; i < n_separation; i++) {
			_desc_content.push_back(description.substr(i * n_char_per_line, (i + 1) * n_char_per_line));
		}

		win->dimension.SetSize(ax::Size(rect.size.w, 14 + n_separation * 12 + 5));
	}

	void PyDocElement::OnPaint(ax::GC gc)
	{
		const ax::Rect rect(win->dimension.GetDrawingRect());

		//		gc.SetColor(ax::Color(1.0));
		//		gc.DrawRectangle(rect);
		gc.DrawRectangleColorFade(rect, at::Skin::GetInstance()->data.w_menu_obj_bg_0,
			at::Skin::GetInstance()->data.w_menu_obj_bg_1);

		gc.SetColor(ax::Color(0.3));
		gc.DrawString(_font, _name, ax::Point(5, 2));

		gc.SetColor(ax::Color(0.3));

		for (int i = 0; i < _desc_content.size(); i++) {
			gc.DrawString(_font_normal, _desc_content[i], ax::Point(5, 14 + i * 12));
		}

		gc.SetColor(ax::Color(0.7));
		gc.DrawRectangleContour(rect);
	}
}
}
