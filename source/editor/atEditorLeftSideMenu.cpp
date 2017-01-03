#include "editor/atEditorLeftSideMenu.hpp"
#include "atHelpBar.h"

#include "widget/atColorButton.hpp"
#include <axlib/Button.hpp>

namespace at {
namespace editor {

	ax::Point LeftSideMenu::AddButton(const ax::Point& pos, ax::Window* win, const ax::Button::Events& evts,
		const ax::Button::Info& info, const std::string& img, const std::string& description)
	{
		auto btn = ax::shared<at::ColorButton>(
			ax::Rect(pos, ax::Size(20, 20)), evts, info, img, "", ax::Button::Flags::SINGLE_IMG);

		AttachHelpInfo(btn->GetWindow(), description);
		win->node.Add(btn);

		_menu_btns.push_back(btn.get());

		return btn->GetWindow()->dimension.GetRect().GetNextPosRight(5);
	}

	LeftSideMenu::LeftSideMenu(const ax::Rect& rect)
	{
		// Create window.
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &LeftSideMenu::OnPaint);
		win->event.OnResize = ax::WBind<ax::Size>(this, &LeftSideMenu::OnResize);

		auto widget_menu = ax::shared<WidgetMenu>(
			ax::Rect(ax::Rect(0, TOP_BAR_HEIGHT, rect.size.w, rect.size.h - TOP_BAR_HEIGHT)));
		win->node.Add(widget_menu);
		_widget_menu = widget_menu.get();

		auto workspace = ax::shared<Workspace>(
			ax::Rect(ax::Rect(0, TOP_BAR_HEIGHT, rect.size.w, rect.size.h - TOP_BAR_HEIGHT)));
		win->node.Add(workspace);
		_workspace = workspace.get();
		_workspace->GetWindow()->Hide();

		auto project_space = ax::shared<ProjectSpace>(
			ax::Rect(ax::Rect(0, TOP_BAR_HEIGHT, rect.size.w, rect.size.h - TOP_BAR_HEIGHT)));
		win->node.Add(project_space);
		_project_space = project_space.get();
		_project_space->GetWindow()->Hide();

		auto online_store_menu = ax::shared<OnlineStoreMenu>(
			ax::Rect(ax::Rect(0, TOP_BAR_HEIGHT, rect.size.w, rect.size.h - TOP_BAR_HEIGHT)));
		win->node.Add(online_store_menu);
		_online_store_menu = online_store_menu.get();
		_online_store_menu->GetWindow()->Hide();

		ax::Button::Info btn_info;
		btn_info.normal = ax::Color(0.0, 0.0);
		btn_info.hover = ax::Color(0.0, 0.0);
		btn_info.clicking = ax::Color(0.0, 0.0);
		btn_info.selected = ax::Color(0.0, 0.0);
		btn_info.contour = ax::Color(0.0, 0.0);
		btn_info.font_color = ax::Color(0.0, 0.0);

		ax::Point pos(5, 2);

		pos = AddButton(pos, win, GetOnSmallerMenu(), btn_info, "resources/resize.png", "Toggle small menu.");

		pos = AddButton(pos, win, GetOnProjectSpace(), btn_info, "resources/folder.png", "Project space.");

		pos = AddButton(pos, win, GetOnWidgetList(), btn_info, "resources/label31.png", "Show wiget list.");

		pos = AddButton(pos, win, GetOnWorkspace(), btn_info, "resources/work.png", "Show workspace layouts");

		pos = AddButton(
			pos, win, GetOnOnlineStoreMenu(), btn_info, "resources/cloud.png", "Download widgets.");

		_menu_btns[0]->SetSelected(true);
		_menu_btns[0]->SetColor(ax::Color(1.0));
		_menu_btns[2]->SetSelected(true);
	}

	void LeftSideMenu::SetOnlyMainWindowWidgetSelectable()
	{
		_widget_menu->SetOnlyMainWindowWidgetSelectable();
	}

	void LeftSideMenu::SetAllSelectable()
	{
		_widget_menu->SetAllSelectable();
	}

	void LeftSideMenu::OnSmallerMenu(const ax::Button::Msg& msg)
	{
		if (!_widget_menu->GetWindow()->IsShown()) {
			return;
		}

		// Is already small -> going bigger.
		if (_dropped_smaller) {
			win->dimension.SetSize(ax::Size(250, win->dimension.GetRect().size.h));

			_dropped_smaller = false;

			// Show all icons.
			for (int i = 3; i < _menu_btns.size(); i++) {
				_menu_btns[i]->GetWindow()->Show();
			}

			_widget_menu->SetWide();

			win->Update();
		}
		else {
			win->dimension.SetSize(ax::Size(85, win->dimension.GetRect().size.h));

			_dropped_smaller = true;

			// Only show left 3 icons.
			for (int i = 3; i < _menu_btns.size(); i++) {
				_menu_btns[i]->GetWindow()->Hide();
			}

			_widget_menu->SetSmall();

			win->Update();
		}

		win->PushEvent(SMALLER_MENU, new ax::Button::Msg(msg));
	}

	void LeftSideMenu::SetWide()
	{
		if (_dropped_smaller) {
			win->dimension.SetSize(ax::Size(250, win->dimension.GetRect().size.h));

			_dropped_smaller = false;

			// Show all icons.
			for (int i = 3; i < _menu_btns.size(); i++) {
				_menu_btns[i]->GetWindow()->Show();
			}

			_widget_menu->SetWide();
			win->Update();
			win->PushEvent(SMALLER_MENU, new ax::Button::Msg(nullptr, ""));
		}
	}

	void LeftSideMenu::OnWidgetList(const ax::Button::Msg& msg)
	{
		_widget_menu->GetWindow()->Show();
		_workspace->GetWindow()->Hide();
		_project_space->GetWindow()->Hide();
		_online_store_menu->GetWindow()->Hide();

		for (int i = 1; i < _menu_btns.size(); i++) {
			_menu_btns[i]->SetSelected(false);
		}

		_menu_btns[2]->SetSelected(true);
		_menu_btns[0]->SetColor(ax::Color(1.0));
	}

	void LeftSideMenu::OnWorkspace(const ax::Button::Msg& msg)
	{
		_widget_menu->GetWindow()->Hide();
		_workspace->GetWindow()->Show();
		_project_space->GetWindow()->Hide();
		_online_store_menu->GetWindow()->Hide();

		for (int i = 1; i < _menu_btns.size(); i++) {
			_menu_btns[i]->SetSelected(false);
		}

		_menu_btns[3]->SetSelected(true);
		_menu_btns[0]->SetColor(ax::Color(0.6));
		SetWide();
	}

	void LeftSideMenu::OnProjectSpace(const ax::Button::Msg& msg)
	{
		_widget_menu->GetWindow()->Hide();
		_workspace->GetWindow()->Hide();
		_project_space->GetWindow()->Show();
		_project_space->UpdateTree();
		_online_store_menu->GetWindow()->Hide();

		SetWide();

		for (int i = 1; i < _menu_btns.size(); i++) {
			_menu_btns[i]->SetSelected(false);
		}

		_menu_btns[1]->SetSelected(true);
		_menu_btns[0]->SetColor(ax::Color(0.6));
	}

	void LeftSideMenu::OnOnlineStoreMenu(const ax::Button::Msg& msg)
	{
		_widget_menu->GetWindow()->Hide();
		_workspace->GetWindow()->Hide();
		_project_space->GetWindow()->Hide();
		_online_store_menu->GetWindow()->Show();

		SetWide();

		for (int i = 1; i < _menu_btns.size(); i++) {
			_menu_btns[i]->SetSelected(false);
		}

		_menu_btns[4]->SetSelected(true);
		_menu_btns[0]->SetColor(ax::Color(0.6));
	}

	void LeftSideMenu::OnResize(const ax::Size& size)
	{
		const ax::Size s(size - ax::Size(0, TOP_BAR_HEIGHT));
		_widget_menu->GetWindow()->dimension.SetSize(s);
		_workspace->GetWindow()->dimension.SetSize(s);
		_project_space->GetWindow()->dimension.SetSize(s);
		_online_store_menu->GetWindow()->dimension.SetSize(s);
	}

	void LeftSideMenu::OnPaint(ax::GC gc)
	{
		const ax::Rect rect(win->dimension.GetDrawingRect());
		gc.SetColor(ax::Color(0.45));
		gc.DrawRectangle(rect);
		gc.DrawRectangleContour(rect);
	}
}
}
