//
//  atEditorStore.cpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-21.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#include "atEditorOnlineStoreMenu.h"

namespace at {
namespace editor {
	OnlineStoreMenu::OnlineStoreMenu(const ax::Rect& rect)
		: _font(0)
		, _font_bold("fonts/FreeSansBold.ttf")
	{
		// Create window.
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &OnlineStoreMenu::OnPaint);
	}

	void OnlineStoreMenu::OnPaint(ax::GC gc)
	{
		const ax::Rect rect(win->dimension.GetDrawingRect());

		gc.SetColor(ax::Color(1.0));
		gc.DrawRectangle(rect);

		gc.SetColor(ax::Color(0.3));
		gc.DrawString(_font_bold, "No implemented yet.", ax::Point(15, 20));

		gc.SetColor(ax::Color(0.7));
		gc.DrawRectangleContour(rect);
	}
}
}