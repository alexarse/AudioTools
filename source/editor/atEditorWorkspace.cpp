//
//  atEditorWorkspace.cpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-21.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#include "editor/atEditorWorkspace.hpp"
#include "editor/atEditorWorkspaceObj.hpp"
#include <axlib/FileSystem.hpp>

namespace at {
namespace editor {
	Workspace::Workspace(const ax::Rect& rect)
		: _font(0)
		, _font_bold("fonts/FreeSansBold.ttf")
	{
		// Create window.
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &Workspace::OnPaint);
		win->event.OnResize = ax::WBind<ax::Size>(this, &Workspace::OnResize);
		win->event.OnScrollWheel = ax::WBind<ax::Point>(this, &Workspace::OnScrollWheel);
		win->event.OnMouseEnter = ax::WBind<ax::Point>(this, &Workspace::OnMouseEnter);
		win->event.OnMouseEnterChild = ax::WBind<ax::Point>(this, &Workspace::OnMouseEnterChild);
		win->event.OnMouseLeave = ax::WBind<ax::Point>(this, &Workspace::OnMouseLeave);
		win->event.OnMouseLeaveChild = ax::WBind<ax::Point>(this, &Workspace::OnMouseLeaveChild);

		// Create scrolling window.
		_panel = ax::Window::Create(ax::Rect(0, 0, rect.size.w, rect.size.h));
		win->node.Add(std::shared_ptr<ax::Window>(_panel));

		const ax::Size size(rect.size.w, 50);
		ax::Point pos(0, 0);

		//		ax::os::Path dir("workspace/");
		ax::os::Path dir("custom_widgets/");
		std::vector<ax::os::Path> files = dir.GetDirectoryContent();

		for (auto& n : files) {

			if (n.GetExtension() != "xml") {
				continue;
			}

			ax::console::Print(n.GetAbsolutePath());

			try {
				ax::Xml xml(n.GetAbsolutePath());

				if (!xml.Parse()) {
					ax::console::Error("parsing workspace :", n.GetAbsolutePath());
					continue;
				}

				//				ax::Xml::Node node = xml.GetNode("Widget");
				ax::Xml::Node node = xml.GetNode("CustomWidget");

				if (!node.IsValid()) {
					ax::console::Error("No widget node in parsing workspace :", n.GetAbsolutePath());
					continue;
				}

				//				std::string buider_name = node.GetAttribute("builder");
				//				std::string file_path = node.GetAttribute("file");
				std::string buider_name = "none";
				//				std::string file_path = "none";

				std::string widget_label = node.GetAttribute("name");
				std::string widget_desc = node.GetAttribute("description");
				std::string widget_size = node.GetAttribute("size");
				std::string widget_img = node.GetAttribute("img");

				auto obj = ax::shared<WorkspaceObj>(ax::Rect(pos, size), buider_name, n.GetAbsolutePath(),
					widget_label, widget_desc, widget_size, widget_img);
				win->node.Add(obj);

				_objs.push_back(obj);

				pos = obj->GetWindow()->dimension.GetRect().GetNextPosDown(0);
			}
#warning("Catch this.")
			//			catch (rapidxml::parse_error& err) {
			//				ax::console::Error("Widget menu xml", err.what());
			//			}
			catch (ax::Xml::Exception& err) {
				ax::console::Error("Widget menu xml", err.what());
			}
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
	}

	void Workspace::OnMouseEnter(const ax::Point& pos)
	{
		win->event.GrabScroll();
	}

	void Workspace::OnMouseEnterChild(const ax::Point& pos)
	{
		win->event.GrabScroll();
	}

	void Workspace::OnMouseLeave(const ax::Point& pos)
	{
		if (!win->dimension.GetAbsoluteRect().IsPointInside(pos)) {
			win->event.UnGrabScroll();
		}
	}

	void Workspace::OnMouseLeaveChild(const ax::Point& pos)
	{
		if (!win->dimension.GetAbsoluteRect().IsPointInside(pos)) {
			win->event.UnGrabScroll();
		}
	}

	void Workspace::OnScrollWheel(const ax::Point& delta)
	{
		double scroll_value
			= (delta.y / (double)ax::App::GetInstance().GetFrameSize().h) + _scrollBar->GetZeroToOneValue();
		_scrollBar->SetZeroToOneValue(scroll_value);
	}

	void Workspace::OnResize(const ax::Size& size)
	{
		ax::Rect sRect(size.w - 9, 0, 10, size.h);
		_scrollBar->GetWindow()->dimension.SetRect(sRect);
		_panel->dimension.SetShownRect(ax::Rect(0, 0, size.w, size.h));

		_scrollBar->UpdateWindowSize(_panel->dimension.GetSize());
	}

	void Workspace::OnPaint(ax::GC gc)
	{
		const ax::Rect rect(win->dimension.GetDrawingRect());

		gc.SetColor(ax::Color(1.0));
		gc.DrawRectangle(rect);

		gc.SetColor(ax::Color(0.3));
		gc.DrawString(_font_bold, "No layout in workspace.", ax::Point(15, 20));

		gc.SetColor(ax::Color(0.7));
		gc.DrawRectangleContour(rect);
	}
}
}
