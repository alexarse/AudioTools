//
//  atEditorWorkspaceObj.cpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-21.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#include "editor/atEditorWorkspaceObj.hpp"
#include "atSkin.hpp"
#include "editor/GlobalEvents.hpp"
#include "editor/atEditor.hpp"

namespace at {
namespace editor {
	ax::Size CalculateAspectRatioFit(const ax::Size& img_size, const ax::Size& max_size)
	{
		const float ratio = std::min(max_size.w / (float)img_size.w, max_size.h / (float)img_size.h);
		return ax::Size(img_size.w * ratio, img_size.h * ratio);
	}

	WorkspaceObj::WorkspaceObj(const ax::Rect& rect, const std::string& builder_name,
		const std::string& file_path, const std::string& title, const std::string& info,
		const std::string& size, const std::string& img_path)
		: _font("fonts/Lato.ttf")
		, _font_normal("fonts/LatoLight.ttf")
		, _builder_name(builder_name)
		, _file_path(file_path)
		, _title(title)
		, _info(info)
		, _size_str(size)
		, _selectable(true)
	{
		_font_normal.SetFontSize(11);

		_img = ax::shared<ax::Image>(img_path);

		if (_img->IsImageReady()) {
			_img_size = CalculateAspectRatioFit(_img->GetSize(), ax::Size(45, 40));
		}

		// Create window.
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &WorkspaceObj::OnPaint);
		win->event.OnMouseLeftDown = ax::WBind<ax::Point>(this, &WorkspaceObj::OnMouseLeftDown);
		win->event.OnMouseLeftDragging = ax::WBind<ax::Point>(this, &WorkspaceObj::OnMouseLeftDragging);
		win->event.OnMouseLeftUp = ax::WBind<ax::Point>(this, &WorkspaceObj::OnMouseLeftUp);
	}

	void WorkspaceObj::HideText()
	{
		if (_show_text) {
			_show_text = false;
			win->Update();
		}
	}

	void WorkspaceObj::ShowText()
	{
		if (!_show_text) {
			_show_text = true;
			win->Update();
		}
	}

	void WorkspaceObj::SetSelectable(bool selectable)
	{
		if (_selectable != selectable) {
			_selectable = selectable;
			win->Update();
		}
	}

	void WorkspaceObj::OnMouseLeftDown(const ax::Point& pos)
	{
		if (_selectable) {
			win->event.GrabMouse();

			App::GetMainEvtObj()->PushEvent(global::CREATE_CUSTOM_DRAGGING_WIDGET,
				new ax::event::SimpleMsg<std::pair<std::pair<std::string, std::string>, ax::Point>>(
					std::pair<std::pair<std::string, std::string>, ax::Point>(
						std::pair<std::string, std::string>(_builder_name, _file_path), pos)));

			win->Update();
		}
	}

	void WorkspaceObj::OnMouseLeftDragging(const ax::Point& pos)
	{
		App::GetMainEvtObj()->PushEvent(global::DRAGGING_WIDGET, new ax::event::SimpleMsg<ax::Point>(pos));
	}

	void WorkspaceObj::OnMouseLeftUp(const ax::Point& pos)
	{
		if (win->event.IsGrabbed()) {
			win->event.UnGrabMouse();

			App::GetMainEvtObj()->PushEvent(global::RELEASE_WIDGET, new ax::event::SimpleMsg<ax::Point>(pos));
			win->Update();
		}
	}

	void WorkspaceObj::OnPaint(ax::GC gc)
	{
		ax::Rect rect(win->dimension.GetDrawingRect());
		gc.DrawRectangleColorFade(rect, at::Skin::GetInstance()->data.w_menu_obj_bg_0,
			at::Skin::GetInstance()->data.w_menu_obj_bg_1);

		ax::Size img_size(_img_size);
		ax::Point img_pos(5 + (65 - img_size.w) / 2, 5 + (rect.size.h - 8 - img_size.h) / 2);
		gc.DrawImageResize(_img.get(), img_pos, img_size);
		gc.SetColor(ax::Color(0.3));
		gc.DrawRectangleContour(ax::Rect(img_pos, img_size));

		if (_show_text) {
			gc.SetColor(at::Skin::GetInstance()->data.w_menu_title_txt);
			gc.DrawString(_font, _title, ax::Point(75, 6));

			gc.SetColor(at::Skin::GetInstance()->data.w_menu_txt);
			gc.DrawString(_font_normal, _info, ax::Point(75, 20));
			gc.DrawString(_font_normal, _size_str, ax::Point(75, 32));
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
