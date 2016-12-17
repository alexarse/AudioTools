#include "editor/atEditorRightSideMenu.hpp"
#include "atHelpBar.h"

#include <axlib/Button.hpp>

namespace at {
namespace editor {

	ax::Point RightSideMenu::AddButton(const ax::Point& pos, ax::Window* win, const ax::Button::Events& evts,
		const ax::Button::Info& info, const std::string& img, const std::string& description)
	{
		auto btn = ax::shared<at::ColorButton>(
			ax::Rect(pos, ax::Size(20, 20)), evts, info, img, "", ax::Button::Flags::SINGLE_IMG);

		AttachHelpInfo(btn->GetWindow(), description);
		win->node.Add(btn);
		_btns.push_back(btn.get());

		return btn->GetWindow()->dimension.GetRect().GetNextPosRight(5);
	}

	RightSideMenu::RightSideMenu(const ax::Rect& rect)
	{
		// Create window.
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &RightSideMenu::OnPaint);
		win->event.OnResize = ax::WBind<ax::Size>(this, &RightSideMenu::OnResize);

		auto inspector = ax::shared<InspectorMenu>(
			ax::Rect(0, TOP_BAR_HEIGHT, rect.size.w, rect.size.h - TOP_BAR_HEIGHT));
		win->node.Add(inspector);
		_inspector = inspector.get();

		// Account panel.
		auto proj_info
			= ax::shared<ProjectInfo>(ax::Rect(0, TOP_BAR_HEIGHT, rect.size.w, rect.size.h - TOP_BAR_HEIGHT));
		win->node.Add(proj_info);
		_project_info = proj_info.get();
		_project_info->GetWindow()->Hide();

		// Documentation panel.
		auto pydoc
			= ax::shared<PyDoc>(ax::Rect(0, TOP_BAR_HEIGHT, rect.size.w, rect.size.h - TOP_BAR_HEIGHT));
		win->node.Add(pydoc);
		_pydoc = pydoc.get();
		_pydoc->GetWindow()->Hide();

		// Account panel.
		auto account
			= ax::shared<Account>(ax::Rect(0, TOP_BAR_HEIGHT, rect.size.w, rect.size.h - TOP_BAR_HEIGHT));
		win->node.Add(account);
		_account = account.get();
		_account->GetWindow()->Hide();

		ax::Button::Info btn_info;
		btn_info.normal = ax::Color(0.0, 0.0);
		btn_info.hover = ax::Color(0.0, 0.0);
		btn_info.clicking = ax::Color(0.0, 0.0);
		btn_info.selected = ax::Color(0.0, 0.0);
		btn_info.contour = ax::Color(0.0, 0.0);
		btn_info.font_color = ax::Color(0.0, 0.0);

		ax::Point pos(5, 2);
		//		ax::Size size(20, 20);

		// Widget info.
		pos = AddButton(pos, win, GetOnInspectorButton(), btn_info, "resources/workspace.png",
			"Show widget inspector menu.");

		// Project info.
		pos = AddButton(
			pos, win, GetOnProjectInfoButton(), btn_info, "resources/info.png", "Show Project information.");

		// Pyo documentation.
		pos = AddButton(
			pos, win, GetOnPyDocButton(), btn_info, "resources/graduate.png", "Show pyo documentation.");

		// User account.
		pos = AddButton(pos, win, GetOnAccountButton(), btn_info, "resources/account.png",
			"Show user account information.");

		// Code snippet.
		pos = AddButton(
			pos, win, ax::Button::Events(), btn_info, "resources/attachment.png", "Show code snippet.");

		_btns[0]->SetSelected(true);
	}

	void RightSideMenu::SetInspectorHandle(ax::Window* handle)
	{
		_inspector->SetWidgetHandle(handle);
	}

	void RightSideMenu::RemoveInspectorHandle()
	{
		_inspector->RemoveHandle();
	}

	void RightSideMenu::SetMultipleWidgetSelected(bool on)
	{
		_inspector->SetMultipleWidgetSelected(on);
	}

	void RightSideMenu::OnInspectorButton(const ax::Button::Msg& msg)
	{
		_inspector->GetWindow()->Show();
		_pydoc->GetWindow()->Hide();
		_account->GetWindow()->Hide();
		_project_info->GetWindow()->Hide();

		for (auto& n : _btns) {
			n->SetSelected(false);
		}

		_btns[0]->SetSelected(true);
	}

	void RightSideMenu::OnProjectInfoButton(const ax::Button::Msg& msg)
	{
		_inspector->GetWindow()->Hide();
		_pydoc->GetWindow()->Hide();
		_account->GetWindow()->Hide();
		_project_info->GetWindow()->Show();

		for (auto& n : _btns) {
			n->SetSelected(false);
		}

		_btns[1]->SetSelected(true);
	}

	void RightSideMenu::OnPyDocButton(const ax::Button::Msg& msg)
	{
		_inspector->GetWindow()->Hide();
		_pydoc->GetWindow()->Show();
		_account->GetWindow()->Hide();
		_project_info->GetWindow()->Hide();

		for (auto& n : _btns) {
			n->SetSelected(false);
		}

		_btns[2]->SetSelected(true);
	}

	void RightSideMenu::OnAccountButton(const ax::Button::Msg& msg)
	{
		_inspector->GetWindow()->Hide();
		_pydoc->GetWindow()->Hide();
		_account->GetWindow()->Show();
		_project_info->GetWindow()->Hide();

		for (auto& n : _btns) {
			n->SetSelected(false);
		}

		_btns[3]->SetSelected(true);
	}

	void RightSideMenu::OnResize(const ax::Size& size)
	{
		const ax::Size s(size - ax::Size(0, TOP_BAR_HEIGHT));
		_inspector->GetWindow()->dimension.SetSize(s);
		_pydoc->GetWindow()->dimension.SetSize(s);
		_account->GetWindow()->dimension.SetSize(s);
		_project_info->GetWindow()->dimension.SetSize(s);
	}

	void RightSideMenu::OnPaint(ax::GC gc)
	{
		const ax::Rect rect(win->dimension.GetDrawingRect());
		gc.SetColor(ax::Color(0.45));
		gc.DrawRectangle(rect);
		gc.DrawRectangleContour(rect);
	}
}
}
