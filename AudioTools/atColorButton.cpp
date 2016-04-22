//
//  atColorButton.cpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-21.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#include "atColorButton.h"

namespace at {
ColorButton::ColorButton(const ax::Rect& rect, const ax::Button::Events& events, const ax::Button::Info& info,
	std::string img_path, std::string label, ax::Flag flags, std::string msg)
	: ax::Button(rect, events, info, img_path, label, flags, msg)
{

	win->event.OnPaint = ax::WBind<ax::GC>(this, &ColorButton::OnCustomPaint);
}

void ColorButton::OnCustomPaint(ax::GC gc)
{
//	ax::Print("Draw ButtonColor");
	//	ax::Rect rect(win->dimension.GetRect());
	ax::Rect rect0(win->dimension.GetDrawingRect());

	ax::widget::Component::Ptr widget = win->component.Get<ax::widget::Component>("Widget");
	ax::Button::Info* info = static_cast<ax::Button::Info*>(widget->GetInfo());

	gc.SetColor(_currentColor);

	int radius = info->corner_radius;

	if (radius > 1) {
		gc.DrawRoundedRectangle(rect0, radius);
	}
	else {
		gc.DrawRectangle(rect0);
	}

	if (_btnImg->IsImageReady()) {

		if (_selected) {
			//		if (ax::IsFlag(Flags::SINGLE_IMG, _flags)) {
			gc.DrawImageResizeColor(
				_btnImg.get(), rect0.position, rect0.size - ax::Size(1, 1), _color);
			//		}
		}
		else {
			gc.DrawImageResize(_btnImg.get(), rect0.position, rect0.size - ax::Size(1, 1), 1.0);
		}
	}

	//	if (!_label.empty()) {
	//		gc.SetColor(info->font_color, 1.0);
	//		gc.DrawStringAlignedCenter(_font, _label, rect0);
	//	}

	//	gc.SetColor(info->contour);
	//
	//	if (radius > 1) {
	//		gc.DrawRoundedRectangleContour(rect0, radius);
	//	}
	//	else {
	gc.DrawRectangleContour(rect0);
	//	}
}
}