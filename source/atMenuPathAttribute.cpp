//
//  atMenuPathAttribute.cpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-28.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#include "atMenuPathAttribute.hpp"

#include <OpenAX/Label.h>
#include <OpenAX/TextBox.h>

namespace at {
namespace inspector {
	PathAttribute::PathAttribute(
		const ax::Rect& rect, const std::string& name, const std::string& value, ax::Event::Function fct)
		: _name(name)
		, _value(value)
		, _font("fonts/Lato.ttf")
	{
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &PathAttribute::OnPaint);

		if (fct) {
			win->AddConnection(Events::ASSIGN_VALUE, fct);
		}

		ax::TextBox::Info txtInfo;
		txtInfo.normal = ax::Color(1.0);
		txtInfo.hover = ax::Color(1.0);
		txtInfo.selected = ax::Color(1.0);
		txtInfo.highlight = ax::Color(0.4f, 0.4f, 0.6f, 0.2f);
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
		win->node.Add(ax::shared<ax::Label>(ax::Rect(pos, ax::Size(90, rect.size.y + 1)), labelInfo, _name));

		ax::TextBox::Events txt_evts;
		txt_evts.enter_click = ax::Event::Function([&](ax::Event::Msg* msg) {
			ax::TextBox::Msg* tmsg = static_cast<ax::TextBox::Msg*>(msg);
			std::string msg_str = tmsg->GetMsg();

			win->PushEvent(Events::ASSIGN_VALUE,
				new ax::Event::SimpleMsg<ax::StringPair>(ax::StringPair(_name, msg_str)));
		});

		auto txt_editor = ax::shared<ax::TextBox>(
			ax::Rect(ax::Point(90, 0), ax::Size(rect.size.x - 90 - 20, rect.size.y + 1)), txt_evts, txtInfo,
			"", _value);

		win->node.Add(txt_editor);

		ax::Button::Info btn_info;
		btn_info.normal = ax::Color(1.0);
		btn_info.hover = ax::Color(0.97);
		btn_info.clicking = ax::Color(0.95);
		btn_info.selected = ax::Color(1.0);
		btn_info.contour = ax::Color(0.30);
		btn_info.font_color = ax::Color(1.0);

		// Open button.
		auto create_btn
			= ax::shared<ax::Button>(ax::Rect(txt_editor->GetWindow()->dimension.GetRect().GetNextPosRight(0),
										 ax::Size(20, rect.size.y + 1)),
				GetOnOpenPath(), btn_info, "", "", ax::Button::Flags::SINGLE_IMG);
		win->node.Add(create_btn);
	}

	void PathAttribute::OnOpenPath(const ax::Button::Msg& msg)
	{
		std::string file_path = ax::App::GetInstance().OpenFileDialog();

		if (file_path.empty()) {
			return;
		}

		ax::Print("path :", file_path);

		win->PushEvent(
			Events::ASSIGN_VALUE, new ax::Event::SimpleMsg<ax::StringPair>(ax::StringPair(_name, file_path)));
	}

	void PathAttribute::OnPaint(ax::GC gc)
	{
		const ax::Rect rect(win->dimension.GetDrawingRect());

		gc.SetColor(ax::Color(0.94));
		gc.DrawRectangle(rect);
		gc.DrawRectangleContour(rect);
	}
}
}