//
//  atMenuSizeAttribute.cpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-07.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#include "atMenuSizeAttribute.hpp"
#include <OpenAX/Button.h>
#include <OpenAX/ColorPicker.h>
#include <OpenAX/Label.h>
#include <OpenAX/TextBox.h>
#include <OpenAX/Toggle.h>
#include <OpenAX/WindowManager.h>
#include <OpenAX/Xml.h>

namespace at {
namespace inspector {
	SizeAttribute::SizeAttribute(
		const ax::Rect& rect, const std::string& name, const std::string& value, ax::Event::Function fct)
		: _name(name)
		, _font("fonts/Lato.ttf")
	{
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &SizeAttribute::OnPaint);

		if (fct) {
			win->AddConnection(Events::ASSIGN_VALUE, fct);
		}

		ax::Label::Info labelInfo;
		labelInfo.normal = ax::Color(0.98);
		labelInfo.contour = ax::Color(0.88);
		labelInfo.font_color = ax::Color(0.0);
		labelInfo.font_size = 12;
		labelInfo.font_name = "fonts/Lato.ttf";
		labelInfo.alignement = ax::Utils::Alignement::axALIGN_LEFT;

		ax::Point pos(0, 0);
		win->node.Add(ax::shared<ax::Label>(ax::Rect(pos, ax::Size(90, 25)), labelInfo, _name));

		ax::TextBox::Info txtInfo;
		txtInfo.normal = ax::Color(1.0);
		txtInfo.hover = ax::Color(1.0);
		txtInfo.selected = ax::Color(1.0);
		txtInfo.highlight = ax::Color(0.4f, 0.4f, 0.6f, 0.2f);
		txtInfo.contour = ax::Color(0.88);
		txtInfo.cursor = ax::Color(1.0f, 0.0f, 0.0f);
		txtInfo.selected_shadow = ax::Color(0.8f, 0.8f, 0.8f);
		txtInfo.font_color = ax::Color(0.0);

		ax::NumberScroll::Info scroll_info;
		scroll_info.up_btn = "resources/drop_up.png";
		scroll_info.down_btn = "resources/drop_down.png";

		// Txt box.
		scroll_info.txt_info.normal = ax::Color(1.0);
		scroll_info.txt_info.hover = ax::Color(1.0);
		scroll_info.txt_info.selected = ax::Color(1.0);
		scroll_info.txt_info.highlight = ax::Color(0.4f, 0.4f, 0.6f, 0.2f);
		scroll_info.txt_info.contour = ax::Color(0.88);
		scroll_info.txt_info.cursor = ax::Color(1.0f, 0.0f, 0.0f);
		scroll_info.txt_info.selected_shadow = ax::Color(0.8f, 0.8f, 0.8f);
		scroll_info.txt_info.font_color = ax::Color(0.0);

		// Button.
		scroll_info.btn_info.normal = ax::Color(0.92);
		scroll_info.btn_info.hover = ax::Color(0.94);
		scroll_info.btn_info.clicking = ax::Color(0.90);
		scroll_info.btn_info.selected = scroll_info.btn_info.normal;
		scroll_info.btn_info.contour = ax::Color(0.88);
		scroll_info.btn_info.font_color = ax::Color(0.0, 0.0);

		auto size_values = ax::Utils::String::Split(value, ",");
		int w_value = std::stoi(size_values[0]);
		int h_value = std::stoi(size_values[1]);

		auto w_scroll
			= ax::shared<ax::NumberScroll>(ax::Rect(110, 0, 60, rect.size.y + 1), GetOnWidthChange(), scroll_info,
				w_value, ax::Utils::Control::Type::INTEGER, ax::Utils::Range<double>(1.0, 10000.0), 1.0);

		_width_scroll = w_scroll.get();
		win->node.Add(w_scroll);

		auto h_scroll
			= ax::shared<ax::NumberScroll>(ax::Rect(190, 0, 60, rect.size.y + 1), GetOnHeightChange(), scroll_info,
				h_value, ax::Utils::Control::Type::INTEGER, ax::Utils::Range<double>(1.0, 10000.0), 1.0);

		_height_scroll = h_scroll.get();
		win->node.Add(h_scroll);
	}

	void SizeAttribute::OnWidthChange(const ax::NumberScroll::Msg& msg)
	{
		std::string w_str = std::to_string((int)msg.GetValue());
		std::string h_str = std::to_string((int)_height_scroll->GetValue());
		std::string out_str(w_str + ", " + h_str);
	
		win->PushEvent(Events::ASSIGN_VALUE,
					   new ax::Event::SimpleMsg<ax::StringPair>(ax::StringPair(_name, out_str)));
	}

	void SizeAttribute::OnHeightChange(const ax::NumberScroll::Msg& msg)
	{
		std::string w_str = std::to_string((int)_width_scroll->GetValue());
		std::string h_str = std::to_string((int)msg.GetValue());
		std::string out_str(w_str + ", " + h_str);
		
		win->PushEvent(Events::ASSIGN_VALUE,
					   new ax::Event::SimpleMsg<ax::StringPair>(ax::StringPair(_name, out_str)));
	}

	void SizeAttribute::OnPaint(ax::GC gc)
	{
		const ax::Rect rect(win->dimension.GetDrawingRect());

		gc.SetColor(ax::Color(0.96));
		gc.DrawRectangle(rect);

		gc.SetColor(ax::Color(0.0));
		gc.DrawString(_font, "w :", ax::Point(93, 3));

		gc.DrawString(_font, "h :", ax::Point(175, 3));

		gc.SetColor(ax::Color(0.88));
		gc.DrawRectangleContour(ax::Rect(rect.position, ax::Size(rect.size.x, rect.size.y + 1)));
		
		gc.SetColor(ax::Color(0.88));
		gc.DrawLine(ax::Point(91, 0), ax::Point(91, rect.size.y + 1));
	}
}
}