//
//  atEditorWidgetMenuSeparator.cpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-21.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#include "editor/atEditorWidgetMenuSeparator.hpp"
#include "atSkin.hpp"

namespace at {
namespace editor {
	WidgetMenuSeparator::WidgetMenuSeparator(const ax::Rect& rect, const std::string& name)
		: _font("fonts/FreeSansBold.ttf")
		, _name(name)
	{
		// Create window.
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &WidgetMenuSeparator::OnPaint);
		win->event.OnMouseLeftDown = ax::WBind<ax::Point>(this, &WidgetMenuSeparator::OnMouseLeftDown);
		win->event.OnMouseLeftDragging
			= ax::WBind<ax::Point>(this, &WidgetMenuSeparator::OnMouseLeftDragging);
		win->event.OnMouseLeftUp = ax::WBind<ax::Point>(this, &WidgetMenuSeparator::OnMouseLeftUp);
	}

	void WidgetMenuSeparator::OnMouseLeftDown(const ax::Point& pos)
	{
	}

	void WidgetMenuSeparator::OnMouseLeftDragging(const ax::Point& pos)
	{
	}

	void WidgetMenuSeparator::OnMouseLeftUp(const ax::Point& pos)
	{
	}

	void WidgetMenuSeparator::OnPaint(ax::GC gc)
	{
		const ax::Rect rect(win->dimension.GetDrawingRect());

		gc.SetColor(at::Skin::GetInstance()->data.w_menu_separator_bg);
		gc.DrawRectangle(rect);

		gc.SetColor(at::Skin::GetInstance()->data.w_menu_separator_contour);
		gc.DrawRectangleContour(rect);

		gc.SetColor(at::Skin::GetInstance()->data.w_menu_separator_text);
		gc.DrawString(_font, _name, ax::Point(10, 2));
	}
}
}
