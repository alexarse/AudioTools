//
//  atEditorPyDocElement.cpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-19.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#include "editor/atEditorPyDocSeparator.hpp"
#include "atSkin.hpp"
#include <axlib/Toggle.hpp>

namespace at {
namespace editor {
	PyDocSeparator::PyDocSeparator(const ax::Rect& rect, const std::string& name,
		const std::vector<std::pair<std::string, std::string>>& elements)
		: _font("fonts/FreeSansBold.ttf")
		, _name(name)
	{
		// Create window.
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &PyDocSeparator::OnPaint);

		ax::Toggle::Info tog_info;
		tog_info.normal = ax::Color(0.0, 0.0);
		tog_info.hover = ax::Color(0.0, 0.0);
		tog_info.clicking = ax::Color(0.0, 0.0);
		tog_info.selected = ax::Color(0.0, 0.0);
		tog_info.font_color = ax::Color(0.0, 0.0);

		tog_info.selected = ax::Color(0.0, 0.0);
		tog_info.selected_hover = ax::Color(0.0, 0.0);
		tog_info.selected_clicking = ax::Color(0.0, 0.0);
		tog_info.selected = ax::Color(0.0, 0.0);
		tog_info.selected_font_color = ax::Color(0.0, 0.0);

		tog_info.contour = ax::Color(0.0, 0.0);
		tog_info.font_size = 12;
		tog_info.img = "resources/top_menu_toggle_left.png";
		tog_info.single_img = false;

		auto tog_left = ax::shared<ax::Toggle>(
			ax::Rect(ax::Point(rect.size.w - 28, 3), ax::Size(15, 15)), GetOnResizeSeparator(), tog_info);

		win->node.Add(tog_left);

		ax::Point pos(0, rect.size.h);
		ax::Size size(rect.size.w, 40);

		for (auto& n : elements) {
			auto elem = ax::shared<PyDocElement>(ax::Rect(pos, size), n.first, n.second);
			win->node.Add(elem);
			_elements.push_back(elem.get());

			pos = elem->GetWindow()->dimension.GetRect().GetNextPosDown(-1);
		}

		win->dimension.SetSize(ax::Size(rect.size.w, pos.y));
	}

	void PyDocSeparator::OnResizeSeparator(const ax::Toggle::Msg& msg)
	{
		if (msg.GetSelected()) {
			win->dimension.SetSize(ax::Size(win->dimension.GetSize().w, 20));

			for (auto& n : _elements) {
				n->GetWindow()->Hide();
			}
		}
		else {
			ax::Rect rect(_elements.back()->GetWindow()->dimension.GetRect());
			win->dimension.SetSize(ax::Size(rect.size.w, rect.position.y + rect.size.h));

			for (auto& n : _elements) {
				n->GetWindow()->Show();
			}
		}

		//		ax::console::Print("Inse sp need resize");
		win->PushEvent(NEED_RESIZE, new ax::event::EmptyMsg());
	}

	void PyDocSeparator::OnPaint(ax::GC gc)
	{
		const ax::Rect rect(win->dimension.GetDrawingRect());

		gc.SetColor(at::Skin::GetInstance()->data.w_menu_separator_bg);
		gc.DrawRectangle(rect);

		//		gc.SetColor(at::Skin::GetInstance()->data.w_menu_separator_contour);
		gc.SetColor(ax::Color(0.7));
		gc.DrawRectangleContour(rect);

		gc.SetColor(at::Skin::GetInstance()->data.w_menu_separator_text);
		gc.DrawString(_font, _name, ax::Point(10, 2));
	}
}
}
