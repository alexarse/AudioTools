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

#include "editor/atEditorWidgetMenu.hpp"
#include "atHelpBar.h"
#include "atSkin.hpp"
#include "editor/atEditor.hpp"

#include <axlib/Button.hpp>
#include <axlib/FileSystem.hpp>
#include <axlib/Xml.hpp>

#include <set>

namespace at {
namespace editor {
	WidgetMenu::WidgetMenu(const ax::Rect& rect)
	{
		// Create window.
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &WidgetMenu::OnPaint);
		win->event.OnResize = ax::WBind<ax::Size>(this, &WidgetMenu::OnResize);
		win->event.OnScrollWheel = ax::WBind<ax::Point>(this, &WidgetMenu::OnScrollWheel);
		win->event.OnMouseEnter = ax::WBind<ax::Point>(this, &WidgetMenu::OnMouseEnter);
		win->event.OnMouseEnterChild = ax::WBind<ax::Point>(this, &WidgetMenu::OnMouseEnterChild);
		win->event.OnMouseLeave = ax::WBind<ax::Point>(this, &WidgetMenu::OnMouseLeave);
		win->event.OnMouseLeaveChild = ax::WBind<ax::Point>(this, &WidgetMenu::OnMouseLeaveChild);

		// Create scrolling window.
		_panel = ax::Window::Create(ax::Rect(0, 0, rect.size.w, rect.size.h));

		win->node.Add(std::shared_ptr<ax::Window>(_panel));

		ax::Point pos(0, 0);
		const ax::Size size(rect.size.w, 50);
		const ax::Size separator_size(rect.size.w, 20);

		std::vector<WidgetMenuInfo> w_info = GetWidgetsInfo();
		std::string builder_name;

		for (auto& n : w_info) {
			// Each new builder name, create a separator.
			if (n.buider_name != builder_name) {
				builder_name = n.buider_name;

				auto sep = ax::shared<WidgetMenuSeparator>(ax::Rect(pos, separator_size), builder_name);
				_panel->node.Add(sep);
				pos = sep->GetWindow()->dimension.GetRect().GetNextPosDown(0);
			}

			// Create widget menu object.
			auto sep = ax::shared<WidgetMenuObj>(ax::Rect(pos, size), n);
			_panel->node.Add(sep);

			_objs.push_back(sep);

			pos = sep->GetWindow()->dimension.GetRect().GetNextPosDown(0);
		}

		ax::ScrollBar::Info sInfo;
		sInfo.normal = ax::Color(0.80, 0.3);
		sInfo.hover = ax::Color(0.85, 0.3);
		sInfo.clicking = ax::Color(0.82, 0.3);
		sInfo.slider_contour = ax::Color(0.6, 0.2);
		sInfo.contour = ax::Color(0.9, 0.2);
		sInfo.bg_top = ax::Color(0.9, 0.2);
		sInfo.bg_bottom = ax::Color(0.92, 0.2);

		ax::Rect sRect(rect.size.w - 9, 0, 10, rect.size.h);
		_scrollBar = ax::shared<ax::ScrollBar>(sRect, ax::ScrollBar::Events(), sInfo);

		win->node.Add(_scrollBar);

		_panel->property.AddProperty("BlockDrawing");
		_panel->dimension.SetSizeNoShowRect(ax::Size(rect.size.w, pos.y));

		_scrollBar->SetWindowHandle(_panel);
		_scrollBar->UpdateWindowSize(_panel->dimension.GetSize());

		SetOnlyMainWindowWidgetSelectable();
	}

	std::vector<WidgetMenuInfo> WidgetMenu::GetWidgetsInfo()
	{
		std::vector<WidgetMenuInfo> w_info;
		const std::string w_dir_path("widgets/");

		//		ax::os::Directory dir;
		//		dir.Goto(w_dir_path);
		ax::os::Path dir(w_dir_path);

		//		std::vector<ax::os::File> files = dir.GetContent();
		std::vector<ax::os::Path> files = dir.GetDirectoryContent();
		ax::console::Print("List directory :", dir.GetAbsolutePath());
		for (auto& n : files) {
			ax::console::Print("file :", n.GetAbsolutePath());

			if (n.GetExtension() != "xml") {
				continue;
			}

			try {
				const std::string file_path(n.GetAbsolutePath());
				ax::Xml xml(file_path);

				if (!xml.Parse()) {
					ax::console::Error("Parsing widget :", n.GetName());
					continue;
				}

				ax::Xml::Node node = xml.GetNode("Widget");

				if (!node.IsValid()) {
					ax::console::Error("Parsing widget :", n.GetName(), "can't find node Widget.");
					continue;
				}

				WidgetMenuInfo info;
				info.file_path = file_path;
				info.buider_name = node.GetAttribute("builder");
				info.widget_label = node.GetAttribute("label");
				info.widget_desc = node.GetAttribute("description");
				info.widget_size = node.GetAttribute("size");
				info.widget_img = node.GetAttribute("img");
				w_info.push_back(info);
			}
#warning("Catch this.")
			//			catch (rapidxml::parse_error& err) {
			//				ax::console::Error("Widget menu xml", err.what());
			//			}
			catch (ax::Xml::Exception& err) {
				ax::console::Error("Widget menu xml", err.what());
			}
		}

		std::sort(w_info.begin(), w_info.end(), [](WidgetMenuInfo& a, WidgetMenuInfo& b) {
			if (a.buider_name == "Panel") {
				return true;
			}
			else if (b.buider_name == "Panel") {
				return false;
			}
			return (a.buider_name < b.buider_name);
		});

		return w_info;
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
			= (delta.y / (double)ax::App::GetInstance().GetFrameSize().h) + _scrollBar->GetZeroToOneValue();
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
		ax::Rect sRect(size.w - 9, 0, 10, size.h);
		_scrollBar->GetWindow()->dimension.SetRect(sRect);
		_panel->dimension.SetShownRect(ax::Rect(0, 0, size.w, size.h));

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
