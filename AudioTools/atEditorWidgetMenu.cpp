/*
 * Copyright (c) 2016 AudioTools - All Rights Reserved
 *
 * This Software may not be distributed in parts or its entirety
 * without prior written agreement by AutioTools.
 *
 * Neither the name of the AudioTools nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY AUDIOTOOLS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL AUDIOTOOLS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Written by Alexandre Arsenault <alx.arsenault@gmail.com>
 */

#include "atCommon.h"
#include "atEditor.h"
#include "atEditorWidgetMenu.h"
#include "atHelpBar.h"
#include "atSkin.hpp"

#include <OpenAX/Button.h>
#include <OpenAX/Xml.h>
#include <OpenAX/rapidxml.hpp>

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

	WidgetMenuObj::WidgetMenuObj(const ax::Rect& rect, const std::string& builder_name,
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

			App::GetMainEvtObj()->PushEvent(
				8000,
				new ax::Event::SimpleMsg<std::pair<ax::StringPair, ax::Point>>(
					std::pair<ax::StringPair, ax::Point>(ax::StringPair(_builder_name, _file_path), pos)));

			win->Update();
		}
	}

	void WidgetMenuObj::OnMouseLeftDragging(const ax::Point& pos)
	{
		App::GetMainEvtObj()->PushEvent(8001, new ax::Event::SimpleMsg<ax::Point>(pos));
	}

	void WidgetMenuObj::OnMouseLeftUp(const ax::Point& pos)
	{
		if (win->event.IsGrabbed()) {
			win->event.UnGrabMouse();

			App::GetMainEvtObj()->PushEvent(8002, new ax::Event::SimpleMsg<ax::Point>(pos));
			win->Update();
		}
	}

	void WidgetMenuObj::OnPaint(ax::GC gc)
	{
		ax::Rect rect(win->dimension.GetDrawingRect());
		gc.DrawRectangleColorFade(rect, at::Skin::GetInstance()->data.w_menu_obj_bg_0,
			at::Skin::GetInstance()->data.w_menu_obj_bg_1);

		ax::Size img_size(_img->GetSize());
		ax::Point img_pos(5 + (65 - img_size.x) / 2, 5 + (rect.size.y - 8 - img_size.y) / 2);
		gc.DrawImage(_img.get(), img_pos);

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

	WidgetMenu::WidgetMenu(const ax::Rect& rect)
	{
		// Create window.
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &WidgetMenu::OnPaint);
		win->event.OnResize = ax::WBind<ax::Size>(this, &WidgetMenu::OnResize);
		win->event.OnScrollWheel = ax::WBind<ax::Size>(this, &WidgetMenu::OnScrollWheel);
		win->event.OnMouseEnter = ax::WBind<ax::Size>(this, &WidgetMenu::OnMouseEnter);
		win->event.OnMouseEnterChild = ax::WBind<ax::Size>(this, &WidgetMenu::OnMouseEnterChild);
		win->event.OnMouseLeave = ax::WBind<ax::Size>(this, &WidgetMenu::OnMouseLeave);
		win->event.OnMouseLeaveChild = ax::WBind<ax::Size>(this, &WidgetMenu::OnMouseLeaveChild);

		ax::Button::Info btn_info;
		btn_info.normal = ax::Color(0.0, 0.0);
		btn_info.hover = ax::Color(0.0, 0.0);
		btn_info.clicking = ax::Color(0.0, 0.0);
		btn_info.selected = ax::Color(0.0, 0.0);
		btn_info.contour = ax::Color(0.0, 0.0);
		btn_info.font_color = ax::Color(0.0, 0.0);

		auto view_btn = ax::shared<ax::Button>(ax::Rect(ax::Point(5, 2), ax::Size(20, 20)),
			GetOnSmallerMenu(), btn_info, "resources/menu.png", "", ax::Button::Flags::SINGLE_IMG);
		AttachHelpInfo(view_btn->GetWindow(), "Show / Hide widgets information.");
		win->node.Add(view_btn);

		ax::Point pos = view_btn->GetWindow()->dimension.GetRect().GetNextPosRight(5);

		// Documentation button.
		auto doc_btn = ax::shared<ax::Button>(ax::Rect(pos, ax::Size(20, 20)), ax::Button::Events(), btn_info,
			"resources/work.png", "", ax::Button::Flags::SINGLE_IMG);
		AttachHelpInfo(doc_btn->GetWindow(), "Show work.");
		win->node.Add(doc_btn);

		pos = doc_btn->GetWindow()->dimension.GetRect().GetNextPosRight(5);

		auto cloud_btn = ax::shared<ax::Button>(ax::Rect(pos, ax::Size(20, 20)), ax::Button::Events(),
			btn_info, "resources/cloud.png", "", ax::Button::Flags::SINGLE_IMG);
		AttachHelpInfo(cloud_btn->GetWindow(), "Download widgets.");
		win->node.Add(cloud_btn);
		
		pos = cloud_btn->GetWindow()->dimension.GetRect().GetNextPosRight(5);

		

		// Create scrolling window.
		_panel = ax::Window::Create(ax::Rect(0, TOP_BAR_HEIGHT, rect.size.x, rect.size.y - TOP_BAR_HEIGHT));

		win->node.Add(ax::Window::Ptr(_panel));

		pos = ax::Point(0, 0);
		ax::Size size(rect.size.x, 50);
		ax::Size separator_size(rect.size.x, 20);

		ax::Xml xml("resources/widget_menu.xml");

		if (!xml.Parse()) {
			ax::Error("parsing widget menu.");
			return;
		}

		ax::Xml::Node top_node = xml.GetNode("WidgetMenu");
		ax::Xml::Node node = top_node.GetFirstNode();

		try {
			while (node.IsValid()) {
				std::string node_name = node.GetName();
				//				ax::Print("Node name :", node_name);

				if (node_name == "separator") {
					std::string separator_name = node.GetAttribute("name");
					ax::Rect sep_rect(pos, separator_size);

					auto sep = ax::shared<WidgetMenuSeparator>(sep_rect, separator_name);
					_panel->node.Add(sep);

					pos = sep->GetWindow()->dimension.GetRect().GetNextPosDown(0);
				}
				else if (node_name == "widget") {
					//					ax::Print("WIDGET");
					std::string buider_name = node.GetAttribute("builder");
					std::string file_path = node.GetAttribute("file");
					std::string widget_label = node.GetAttribute("label");
					std::string widget_desc = node.GetAttribute("description");
					std::string widget_size = node.GetAttribute("size");
					std::string widget_img = node.GetAttribute("img");

					auto sep = ax::shared<WidgetMenuObj>(ax::Rect(pos, size), buider_name, file_path,
						widget_label, widget_desc, widget_size, widget_img);
					_panel->node.Add(sep);

					_objs.push_back(sep);

					pos = sep->GetWindow()->dimension.GetRect().GetNextPosDown(0);
				}

				node = node.GetNextSibling();
			}
		}
		catch (rapidxml::parse_error& err) {
			ax::Error("Widget menu xml", err.what());
		}
		catch (ax::Xml::Exception& err) {
			ax::Error("Widget menu xml", err.what());
		}

		ax::ScrollBar::Info sInfo;
		sInfo.normal = ax::Color(0.80, 0.3);
		sInfo.hover = ax::Color(0.85, 0.3);
		sInfo.clicking = ax::Color(0.82, 0.3);
		sInfo.slider_contour = ax::Color(0.6, 0.2);
		sInfo.contour = ax::Color(0.9, 0.2);
		sInfo.bg_top = ax::Color(0.9, 0.2);
		sInfo.bg_bottom = ax::Color(0.92, 0.2);

		ax::Rect sRect(rect.size.x - 9, TOP_BAR_HEIGHT, 10, rect.size.y - TOP_BAR_HEIGHT);
		_scrollBar = ax::shared<ax::ScrollBar>(sRect, ax::ScrollBar::Events(), sInfo);

		win->node.Add(_scrollBar);

		_panel->property.AddProperty("BlockDrawing");
		_panel->dimension.SetSizeNoShowRect(ax::Size(rect.size.x, pos.y));

		_scrollBar->SetWindowHandle(_panel);
		_scrollBar->UpdateWindowSize(_panel->dimension.GetSize());

		SetOnlyMainWindowWidgetSelectable();
	}

	void WidgetMenu::SetOnlyMainWindowWidgetSelectable()
	{
		for (int i = 1; i < _objs.size(); i++) {
			_objs[i]->SetSelectable(false);
		}
	}

	void WidgetMenu::SetAllSelectable()
	{
		for (int i = 1; i < _objs.size(); i++) {
			_objs[i]->SetSelectable(true);
		}
	}

	void WidgetMenu::OnMouseEnter(const ax::Point& pos)
	{
		win->event.GrabScroll();
	}

	void WidgetMenu::OnMouseEnterChild(const ax::Point& pos)
	{
		win->event.GrabScroll();
	}

	void WidgetMenu::OnMouseLeave(const ax::Point& pos)
	{
		if (!win->dimension.GetAbsoluteRect().IsPointInside(pos)) {
			win->event.UnGrabScroll();
		}
	}

	void WidgetMenu::OnMouseLeaveChild(const ax::Point& pos)
	{
		if (!win->dimension.GetAbsoluteRect().IsPointInside(pos)) {
			win->event.UnGrabScroll();
		}
	}

	void WidgetMenu::OnScrollWheel(const ax::Point& delta)
	{
//		ax::Size size = _panel->dimension.GetShownRect().size;
//		double scroll_value = (delta.y / double(size.y / 2.0)) + _scrollBar->GetZeroToOneValue();
		double scroll_value = (delta.y / (double)ax::App::GetInstance().GetFrameSize().y) + _scrollBar->GetZeroToOneValue();
		_scrollBar->SetZeroToOneValue(scroll_value);
	}

	void WidgetMenu::OnSmallerMenu(const ax::Button::Msg& msg)
	{
		// Is already small -> going bigger.
		if (_dropped_smaller) {
			win->dimension.SetSize(ax::Size(250, win->dimension.GetRect().size.y));

			for (auto& n : _objs) {
				n->ShowText();
			}

			_dropped_smaller = false;
			win->Update();
		}
		else {
			win->dimension.SetSize(ax::Size(85, win->dimension.GetRect().size.y));

			for (auto& n : _objs) {
				n->HideText();
			}

			_dropped_smaller = true;
			win->Update();
		}

		win->PushEvent(SMALLER_MENU, new ax::Button::Msg(msg));
	}

	void WidgetMenu::OnResize(const ax::Size& size)
	{
		ax::Rect sRect(size.x - 9, TOP_BAR_HEIGHT, 10, size.y - (TOP_BAR_HEIGHT + 1));
		_scrollBar->GetWindow()->dimension.SetRect(sRect);
		_panel->dimension.SetShownRect(ax::Rect(0, TOP_BAR_HEIGHT, size.x, size.y - TOP_BAR_HEIGHT));

		_scrollBar->UpdateWindowSize(_panel->dimension.GetSize());
	}

	void WidgetMenu::OnPaint(ax::GC gc)
	{
		const ax::Rect rect(win->dimension.GetDrawingRect());
		gc.SetColor(at::Skin::GetInstance()->data.w_menu_top_bar);
		gc.DrawRectangle(rect);
		gc.DrawRectangleContour(rect);
	}
}
}