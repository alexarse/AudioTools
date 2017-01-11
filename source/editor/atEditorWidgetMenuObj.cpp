//
//  atEditorWidgetMenuObj.cpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-21.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#include "editor/atEditorWidgetMenuObj.hpp"
#include "atSkin.hpp"
#include "editor/GlobalEvents.hpp"
#include "editor/atEditor.hpp"

namespace at {
namespace editor {
	WidgetMenuObj::WidgetMenuObj(const ax::Rect& rect, const WidgetMenuInfo& info)
		: _font("fonts/Lato.ttf")
		, _font_normal("fonts/LatoLight.ttf")
		, _info(info)
		, _selectable(true)
	{
		_font_normal.SetFontSize(11);

		_img = ax::shared<ax::Image>(_info.widget_img);

		// Create window.
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &WidgetMenuObj::OnPaint);
		win->event.OnMouseLeftDown = ax::WBind<ax::Point>(this, &WidgetMenuObj::OnMouseLeftDown);
		win->event.OnMouseLeftDragging = ax::WBind<ax::Point>(this, &WidgetMenuObj::OnMouseLeftDragging);
		win->event.OnMouseLeftUp = ax::WBind<ax::Point>(this, &WidgetMenuObj::OnMouseLeftUp);
	}

	void WidgetMenuObj::HideText()
	{
		if (_show_text) {
			_show_text = false;
			win->Update();
		}
	}

	void WidgetMenuObj::ShowText()
	{
		if (!_show_text) {
			_show_text = true;
			win->Update();
		}
	}

	void WidgetMenuObj::SetSelectable(bool selectable)
	{
		if (_selectable != selectable) {
			_selectable = selectable;
			win->Update();
		}
	}

	void WidgetMenuObj::OnMouseLeftDown(const ax::Point& pos)
	{
		if (_selectable) {
			win->event.GrabMouse();

			App::GetMainEvtObj()->PushEvent(global::CREATE_DRAGGING_WIDGET,
				new ax::event::SimpleMsg<std::pair<std::pair<std::string, std::string>, ax::Point>>(
					std::pair<std::pair<std::string, std::string>, ax::Point>(
						std::pair<std::string, std::string>(_info.buider_name, _info.file_path), pos)));

			win->Update();
		}
	}

	void WidgetMenuObj::OnMouseLeftDragging(const ax::Point& pos)
	{
		App::GetMainEvtObj()->PushEvent(global::DRAGGING_WIDGET, new ax::event::SimpleMsg<ax::Point>(pos));
	}

	void WidgetMenuObj::OnMouseLeftUp(const ax::Point& pos)
	{
		if (win->event.IsGrabbed()) {
			win->event.UnGrabMouse();

			App::GetMainEvtObj()->PushEvent(global::RELEASE_WIDGET, new ax::event::SimpleMsg<ax::Point>(pos));
			win->Update();
		}
	}

	void WidgetMenuObj::OnPaint(ax::GC gc)
	{
		ax::Rect rect(win->dimension.GetDrawingRect());
		gc.DrawRectangleColorFade(rect, at::Skin::GetInstance()->data.w_menu_obj_bg_0,
			at::Skin::GetInstance()->data.w_menu_obj_bg_1);

		ax::Size img_size(_img->GetSize());
		ax::Point img_pos(5 + (65 - img_size.w) / 2, 5 + (rect.size.h - 8 - img_size.h) / 2);
		gc.DrawImage(_img.get(), img_pos);

		if (_show_text) {
			gc.SetColor(at::Skin::GetInstance()->data.w_menu_title_txt);
			gc.DrawString(_font, _info.widget_label, ax::Point(75, 6));

			gc.SetColor(at::Skin::GetInstance()->data.w_menu_txt);
			gc.DrawString(_font_normal, _info.widget_desc, ax::Point(75, 20));
			gc.DrawString(_font_normal, _info.widget_size, ax::Point(75, 32));
		}

		gc.SetColor(at::Skin::GetInstance()->data.w_menu_obj_contour);

		gc.DrawRectangleContour(rect);

		// If not selectable.
		if (!_selectable) {
			gc.SetColor(ax::Color(0.5, 0.45));
			gc.DrawRectangle(rect);
		}
	}
}
}
