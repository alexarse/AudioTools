#include "atEditorLeftSideMenu.h"
#include "atHelpBar.h"

#include <OpenAX/Button.h>

namespace at {
namespace editor {

	ax::Point LeftSideMenu::AddButton(const ax::Point& pos, ax::Window* win, const ax::Button::Events& evts,
		const ax::Button::Info& info, const std::string& img, const std::string& description)
	{
		auto btn = ax::shared<ax::Button>(
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
			ax::Rect(ax::Rect(0, TOP_BAR_HEIGHT, rect.size.x, rect.size.y - TOP_BAR_HEIGHT)));
		win->node.Add(widget_menu);
		_widget_menu = widget_menu.get();

		ax::Button::Info btn_info;
		btn_info.normal = ax::Color(0.0, 0.0);
		btn_info.hover = ax::Color(0.0, 0.0);
		btn_info.clicking = ax::Color(0.0, 0.0);
		btn_info.selected = ax::Color(0.0, 0.0);
		btn_info.contour = ax::Color(0.0, 0.0);
		btn_info.font_color = ax::Color(0.0, 0.0);

		ax::Point pos(5, 2);

		// Widget info.
		pos = AddButton(pos, win, GetOnSmallerMenu(), btn_info, "resources/resize.png", "Toggle small menu.");

		pos = AddButton(
			pos, win, ax::Button::Events(), btn_info, "resources/label31.png", "Show wiget list.");

		pos = AddButton(pos, win, ax::Button::Events(), btn_info, "resources/work.png", "Show work layouts");

		pos = AddButton(pos, win, ax::Button::Events(), btn_info, "resources/cloud.png", "Download widgets.");
	}

	void LeftSideMenu::SetOnlyMainWindowWidgetSelectable()
	{
	}

	void LeftSideMenu::SetAllSelectable()
	{
	}

	void LeftSideMenu::OnSmallerMenu(const ax::Button::Msg& msg)
	{
		// Is already small -> going bigger.
		if (_dropped_smaller) {
			win->dimension.SetSize(ax::Size(250, win->dimension.GetRect().size.y));

			_dropped_smaller = false;

			// Show all icons.
			for (int i = 3; i < _menu_btns.size(); i++) {
				_menu_btns[i]->GetWindow()->Show();
			}

			win->Update();
		}
		else {
			win->dimension.SetSize(ax::Size(85, win->dimension.GetRect().size.y));

			_dropped_smaller = true;

			// Only show left 3 icons.
			for (int i = 3; i < _menu_btns.size(); i++) {
				_menu_btns[i]->GetWindow()->Hide();
			}

			win->Update();
		}

		win->PushEvent(SMALLER_MENU, new ax::Button::Msg(msg));
	}
	//		void RightSideMenu::OnInspectorButton(const ax::Button::Msg& msg)
	//		{
	//			_inspector->GetWindow()->Show();
	//			_pydoc->GetWindow()->Hide();
	//			_account->GetWindow()->Hide();
	//		}
	//
	//		void RightSideMenu::OnPyDocButton(const ax::Button::Msg& msg)
	//		{
	//			_inspector->GetWindow()->Hide();
	//			_pydoc->GetWindow()->Show();
	//			_account->GetWindow()->Hide();
	//		}
	//
	//		void RightSideMenu::OnAccountButton(const ax::Button::Msg& msg)
	//		{
	//			_inspector->GetWindow()->Hide();
	//			_pydoc->GetWindow()->Hide();
	//			_account->GetWindow()->Show();
	//		}

	void LeftSideMenu::OnResize(const ax::Size& size)
	{
		const ax::Size s(size - ax::Size(0, TOP_BAR_HEIGHT));
		//			_inspector->GetWindow()->dimension.SetSize(s);
		//			_pydoc->GetWindow()->dimension.SetSize(s);
		//			_account->GetWindow()->dimension.SetSize(s);
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