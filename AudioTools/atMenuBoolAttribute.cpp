#include "atMenuBoolAttribute.hpp"
#include <OpenAX/Button.h>
#include <OpenAX/ColorPicker.h>
#include <OpenAX/Label.h>
#include <OpenAX/TextBox.h>
#include <OpenAX/Toggle.h>
#include <OpenAX/WindowManager.h>
#include <OpenAX/Xml.h>

namespace at {
namespace inspector {
	BoolAttribute::BoolAttribute(
		const ax::Rect& rect, const std::string& name, const std::string& value, ax::Event::Function fct)
		: _name(name)
	{
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &BoolAttribute::OnPaint);

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

		ax::Toggle::Info tog_info;
		tog_info.normal = ax::Color(0.0, 0.0);
		tog_info.hover = ax::Color(0.0, 0.0);
		tog_info.clicking = ax::Color(0.0, 0.0);
		tog_info.font_color = ax::Color(0.0, 0.0);
		tog_info.selected = ax::Color(0.0, 0.0);
		tog_info.selected_hover = ax::Color(0.0, 0.0);
		tog_info.selected_clicking = ax::Color(0.0, 0.0);
		tog_info.selected_font_color = ax::Color(0.0, 0.0);

		tog_info.contour = ax::Color(0.0, 0.0);

		tog_info.font_size = 12;

		tog_info.img = "resources/bool_toggle.png";
		tog_info.single_img = false;

		win->node.Add(ax::shared<ax::Toggle>(ax::Rect(95, 4, 13, 13), ax::Toggle::Events(), tog_info));
	}

	void BoolAttribute::OnPaint(ax::GC gc)
	{
		const ax::Rect rect(win->dimension.GetDrawingRect());

		gc.SetColor(ax::Color(0.94));
		gc.DrawRectangle(rect);

		gc.SetColor(ax::Color(1.0));
		gc.DrawRectangle(rect.GetInteriorRect(ax::Point(1, 1)));

		gc.SetColor(ax::Color(0.94));
		gc.DrawRectangleContour(rect);
	}
}
}