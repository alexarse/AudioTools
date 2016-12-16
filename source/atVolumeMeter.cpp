//
//  atVolumeMeter.cpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-03-17.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#include "atVolumeMeter.hpp"

namespace at {
VolumeMeter::VolumeMeter(const ax::Rect& rect)
	: _value(0.0)
{
	// Create window.
	win = ax::Window::Create(rect);
	win->event.OnPaint = ax::WBind<ax::GC>(this, &VolumeMeter::OnPaint);
}

void VolumeMeter::OnPaint(ax::GC gc)
{
	ax::Rect rect(win->dimension.GetDrawingRect());

	int n_square = 10;
	int w_between_square = 2;
	int w_square = (rect.size.x - (n_square - 1) * 2) / double(n_square);

	//	gc.SetColor(ax::Color(255, 0, 0));
	//	gc.DrawRectangle(rect);
	//	gc.DrawRectangleContour(rect);

	ax::Point pos(rect.position);

	gc.SetColor(ax::Color(0, 0, 0));
	for (int i = 0; i < n_square; i++) {
		ax::Rect sq_rect(ax::Rect(pos, ax::Size(w_square, rect.size.y)));
		gc.DrawRectangleContour(sq_rect);
		gc.DrawRectangleColorFade(sq_rect, ax::Color(0.5, 0.5), ax::Color(0.6, 0.5));
		pos.x += w_between_square + w_square;
	}

	pos = rect.position;

	gc.SetColor(ax::Color(0.0f, 1.0f, 0.0f, 0.3f));
	int n_green_square = _value * n_square;
	for (int i = 0; i < n_green_square; i++) {
		ax::Rect sq_rect(ax::Rect(pos, ax::Size(w_square, rect.size.y)));
		ax::Color c0;
		ax::Color c1;

		if (i >= n_square - 1) {
			c0 = ax::Color(1.0f, 0.0f, 0.0f, 0.7f);
			c1 = ax::Color(0.9f, 0.0f, 0.0f, 0.7f);
		}
		else if (i >= n_square - 3) {
			c0 = ax::Color(1.0f, 1.0f, 0.0f, 0.7f);
			c1 = ax::Color(0.9f, 0.9f, 0.0f, 0.7f);
		}
		else {
			c0 = ax::Color(0.0f, 1.0f, 0.0f, 0.7f);
			c1 = ax::Color(0.0f, 0.9f, 0.0f, 0.7f);
		}

		gc.DrawRectangleContour(sq_rect);
		gc.DrawRectangleColorFade(sq_rect, c0, c1);
		pos.x += w_between_square + w_square;
	}

	//	gc.
	//
	//	gc.SetColor(ax::Color(0.30));
	//	gc.DrawRectangle(ax::Rect(0, rect.size.y - 18, rect.size.x, 18));
}
}