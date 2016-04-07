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

		ax::TextBox::Events txt_evts;

		txt_evts.enter_click = ax::Event::Function([&](ax::Event::Msg* msg) {
			ax::TextBox::Msg* tmsg = static_cast<ax::TextBox::Msg*>(msg);
			std::string msg_str = tmsg->GetMsg();

			win->PushEvent(Events::ASSIGN_VALUE,
				new ax::Event::SimpleMsg<ax::StringPair>(ax::StringPair(_name, msg_str)));
		});

		auto size_values = ax::Utils::String::Split(value, ",");
		win->node.Add(ax::shared<ax::TextBox>(
			ax::Rect(ax::Point(110, 0), ax::Size(60, 25)), txt_evts, txtInfo, "", size_values[0]));

		win->node.Add(ax::shared<ax::TextBox>(
			ax::Rect(ax::Point(210, 0), ax::Size(60, 25)), txt_evts, txtInfo, "", size_values[1]));

		auto btn_top = ax::shared<ax::Button>(ax::Rect(ax::Point(150, 0), ax::Size(20, rect.size.y / 2)),
			ax::Button::Events(), ax::Button::Info(), "", "");
		win->node.Add(btn_top);

		ax::Point btn_pos(btn_top->GetWindow()->dimension.GetRect().GetNextPosDown(0));

		auto btn_bottom = ax::shared<ax::Button>(ax::Rect(btn_pos, ax::Size(20, rect.size.y / 2)),
			ax::Button::Events(), ax::Button::Info(), "", "");
		win->node.Add(btn_bottom);
	}

	void SizeAttribute::OnPaint(ax::GC gc)
	{
		const ax::Rect rect(win->dimension.GetDrawingRect());

		gc.SetColor(ax::Color(0.94));
		gc.DrawRectangle(rect);

		gc.SetColor(ax::Color(1.0));
		gc.DrawRectangle(rect.GetInteriorRect(ax::Point(1, 1)));

		gc.SetColor(ax::Color(0.0));
		gc.DrawString(_font, "w :", ax::Point(92, 3));

		gc.DrawString(_font, "h :", ax::Point(180, 3));

		gc.SetColor(ax::Color(0.94));
		gc.DrawRectangleContour(rect);
	}
}
}