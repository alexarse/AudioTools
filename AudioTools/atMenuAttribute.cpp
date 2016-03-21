//
//  atInspectorMenuAttribute.cpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-03-12.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#include "atMenuAttribute.h"
#include <OpenAX/Label.h>
#include <OpenAX/TextBox.h>

namespace at {
namespace inspector {
	MenuAttribute::MenuAttribute(
		const ax::Rect& rect, const std::string& name, const std::string& value, ax::Event::Function fct)
		: _name(name)
		, _value(value)
		, _font("fonts/Lato.ttf")
	{
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &MenuAttribute::OnPaint);

		if (fct) {
			win->AddConnection(Events::ASSIGN_VALUE, fct);
		}

		ax::TextBox::Info txtInfo;
		txtInfo.normal = ax::Color(1.0);
		txtInfo.hover = ax::Color(1.0);
		txtInfo.selected = ax::Color(1.0);
		txtInfo.highlight = ax::Color(0.4f, 0.4f, 0.6f);
		txtInfo.contour = ax::Color(0.88);
		txtInfo.cursor = ax::Color(1.0f, 0.0f, 0.0f);
		txtInfo.selected_shadow = ax::Color(0.8f, 0.8f, 0.8f);
		txtInfo.font_color = ax::Color(0.0);

		ax::Label::Info labelInfo;
		labelInfo.normal = ax::Color(0.98);
		labelInfo.contour = ax::Color(0.88);
		labelInfo.font_color = ax::Color(0.0);
		labelInfo.font_size = 12;
		labelInfo.font_name = "fonts/Lato.ttf";
		labelInfo.alignement = ax::Utils::Alignement::axALIGN_LEFT;

		ax::Point pos(0, 0);
		win->node.Add(ax::shared<ax::Label>(ax::Rect(pos, ax::Size(90, 25)), labelInfo, _name));

		ax::TextBox::Events txt_evts;
		txt_evts.enter_click = ax::Event::Function([&](ax::Event::Msg* msg) {
			ax::TextBox::Msg* tmsg = static_cast<ax::TextBox::Msg*>(msg);
			std::string msg_str = tmsg->GetMsg();
			
			win->PushEvent(Events::ASSIGN_VALUE,
				new ax::Event::SimpleMsg<ax::StringPair>(ax::StringPair(_name, msg_str)));
		});

		win->node.Add(ax::shared<ax::TextBox>(
			ax::Rect(ax::Point(90, 0), ax::Size(rect.size.x - 90, 25)), txt_evts, txtInfo, "", _value));
	}

	void MenuAttribute::OnPaint(ax::GC gc)
	{
		const ax::Rect rect(win->dimension.GetDrawingRect());

		gc.SetColor(ax::Color(0.94));
		gc.DrawRectangle(rect);
		gc.DrawRectangleContour(rect);
	}
}
}