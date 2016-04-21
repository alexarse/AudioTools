/*
 * Copyright (c) 2016 AudioTools - All Rights Reserved
 *
 * This Software may not be distributed in parts or its entirety
 * without prior written agreement by AudioTools.
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

		// Create scrolling window.
		_panel = ax::Window::Create(ax::Rect(0, 0, rect.size.x, rect.size.y));

		win->node.Add(ax::Window::Ptr(_panel));

		ax::Point pos(0, 0);
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

		ax::Rect sRect(rect.size.x - 9, 0, 10, rect.size.y);
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
		double scroll_value
			= (delta.y / (double)ax::App::GetInstance().GetFrameSize().y) + _scrollBar->GetZeroToOneValue();
		_scrollBar->SetZeroToOneValue(scroll_value);
	}

	void WidgetMenu::SetSmall()
	{
		for (auto& n : _objs) {
			n->HideText();
		}

		win->Update();
	}

	void WidgetMenu::SetWide()
	{
		for (auto& n : _objs) {
			n->ShowText();
		}
		
		win->Update();
	}

	void WidgetMenu::OnResize(const ax::Size& size)
	{
		ax::Rect sRect(size.x - 9, 0, 10, size.y);
		_scrollBar->GetWindow()->dimension.SetRect(sRect);
		_panel->dimension.SetShownRect(ax::Rect(0, 0, size.x, size.y));

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