#include "axNumberScroll.h"

#include "atMenuSizeAttribute.hpp"
#include <OpenAX/Button.h>
#include <OpenAX/ColorPicker.h>
#include <OpenAX/Label.h>
#include <OpenAX/TextBox.h>
#include <OpenAX/Toggle.h>
#include <OpenAX/WindowManager.h>
#include <OpenAX/Xml.h>

namespace ax {
NumberScroll::NumberScroll(const ax::Rect& rect, double value)
{
	win = ax::Window::Create(rect);
	win->event.OnPaint = ax::WBind<ax::GC>(this, &NumberScroll::OnPaint);

	ax::TextBox::Info txtInfo;
	txtInfo.normal = ax::Color(1.0);
	txtInfo.hover = ax::Color(1.0);
	txtInfo.selected = ax::Color(1.0);
	txtInfo.highlight = ax::Color(0.4f, 0.4f, 0.6f, 0.2f);
	txtInfo.contour = ax::Color(0.88);
	txtInfo.cursor = ax::Color(1.0f, 0.0f, 0.0f);
	txtInfo.selected_shadow = ax::Color(0.8f, 0.8f, 0.8f);
	txtInfo.font_color = ax::Color(0.0);

	auto txt_box = ax::shared<ax::TextBox>(ax::Rect(ax::Point(110, 0),
		ax::Size(60, 25)), ax::TextBox::Events(), txtInfo, "", std::to_string(value));
	
	win->node.Add(txt_box);

	//	ax::Label::Info labelInfo;
	//	labelInfo.normal = ax::Color(0.98);
	//	labelInfo.contour = ax::Color(0.88);
	//	labelInfo.font_color = ax::Color(0.0);
	//	labelInfo.font_size = 12;
	//	labelInfo.font_name = "fonts/Lato.ttf";
	//	labelInfo.alignement = ax::Utils::Alignement::axALIGN_LEFT;
	//
	//	ax::Point pos(0, 0);
	//	win->node.Add(ax::shared<ax::Label>(ax::Rect(pos, ax::Size(90, 25)), labelInfo, _name));

	//		ax::Toggle::Info tog_info;
	//		tog_info.normal = ax::Color(0.0, 0.0);
	//		tog_info.hover = ax::Color(0.0, 0.0);
	//		tog_info.clicking = ax::Color(0.0, 0.0);
	//		tog_info.font_color = ax::Color(0.0, 0.0);
	//		tog_info.selected = ax::Color(0.0, 0.0);
	//		tog_info.selected_hover = ax::Color(0.0, 0.0);
	//		tog_info.selected_clicking = ax::Color(0.0, 0.0);
	//		tog_info.selected_font_color = ax::Color(0.0, 0.0);
	//
	//		tog_info.contour = ax::Color(0.0, 0.0);
	//
	//		tog_info.font_size = 12;
	//
	//		tog_info.img = "resources/bool_toggle.png";
	//		tog_info.single_img = false;
	//
	//		win->node.Add(ax::shared<ax::Toggle>(ax::Rect(95, 4, 13, 13), ax::Toggle::Events(),
	// tog_info));

	//	ax::TextBox::Info txtInfo;
	//	txtInfo.normal = ax::Color(1.0);
	//	txtInfo.hover = ax::Color(1.0);
	//	txtInfo.selected = ax::Color(1.0);
	//	txtInfo.highlight = ax::Color(0.4f, 0.4f, 0.6f, 0.2f);
	//	txtInfo.contour = ax::Color(0.88);
	//	txtInfo.cursor = ax::Color(1.0f, 0.0f, 0.0f);
	//	txtInfo.selected_shadow = ax::Color(0.8f, 0.8f, 0.8f);
	//	txtInfo.font_color = ax::Color(0.0);
	//
	//	ax::TextBox::Events txt_evts;
	//
	//	txt_evts.enter_click = ax::Event::Function([&](ax::Event::Msg* msg) {
	//		ax::TextBox::Msg* tmsg = static_cast<ax::TextBox::Msg*>(msg);
	//		std::string msg_str = tmsg->GetMsg();
	//
	//		win->PushEvent(
	//			Events::ASSIGN_VALUE, new ax::Event::SimpleMsg<ax::StringPair>(ax::StringPair(_name,
	//msg_str)));
	//	});
	//
	//	auto size_values = ax::Utils::String::Split(value, ",");
	//	win->node.Add(ax::shared<ax::TextBox>(
	//		ax::Rect(ax::Point(110, 0), ax::Size(60, 25)), txt_evts, txtInfo, "", size_values[0]));
	//
	//	win->node.Add(ax::shared<ax::TextBox>(
	//		ax::Rect(ax::Point(210, 0), ax::Size(60, 25)), txt_evts, txtInfo, "", size_values[1]));
	//
	//	auto btn_top = ax::shared<ax::Button>(ax::Rect(ax::Point(150, 0), ax::Size(20, rect.size.y / 2)),
	//		ax::Button::Events(), ax::Button::Info(), "", "");
	//	win->node.Add(btn_top);
	//
	//	ax::Point btn_pos(btn_top->GetWindow()->dimension.GetRect().GetNextPosDown(0));
	//
	//	auto btn_bottom = ax::shared<ax::Button>(
	//		ax::Rect(btn_pos, ax::Size(20, rect.size.y / 2)), ax::Button::Events(), ax::Button::Info(), "",
	//"");
	//	win->node.Add(btn_bottom);
}

void NumberScroll::OnPaint(ax::GC gc)
{
	const ax::Rect rect(win->dimension.GetDrawingRect());

	gc.SetColor(ax::Color(0.94));
	gc.DrawRectangle(rect);

	gc.SetColor(ax::Color(0.94));
	gc.DrawRectangleContour(rect);
}
}