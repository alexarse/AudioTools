#include "atEditorRightSideMenu.h"
#include "atHelpBar.h"

#include <OpenAX/Button.h>

namespace at {
namespace editor {
	RightSideMenu::RightSideMenu(const ax::Rect& rect)
	{
		// Create window.
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &RightSideMenu::OnPaint);
		win->event.OnResize = ax::WBind<ax::Size>(this, &RightSideMenu::OnResize);

		auto inspector = ax::shared<InspectorMenu>(
			ax::Rect(0, TOP_BAR_HEIGHT, rect.size.x, rect.size.y - TOP_BAR_HEIGHT));
		win->node.Add(inspector);
		_inspector = inspector.get();

		// Documentation panel.
		auto pydoc
			= ax::shared<PyDoc>(ax::Rect(0, TOP_BAR_HEIGHT, rect.size.x, rect.size.y - TOP_BAR_HEIGHT));
		win->node.Add(pydoc);
		_pydoc = pydoc.get();
		_pydoc->GetWindow()->Hide();
	
		// Account panel.
		auto account
			= ax::shared<Account>(ax::Rect(0, TOP_BAR_HEIGHT, rect.size.x, rect.size.y - TOP_BAR_HEIGHT));
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
		ax::Size size(20, 20);

		auto inspector_btn = ax::shared<ax::Button>(ax::Rect(pos, size), GetOnInspectorButton(), btn_info,
			"resources/workspace.png", "", ax::Button::Flags::SINGLE_IMG);

		AttachHelpInfo(inspector_btn->GetWindow(), "Show widget inspector menu.");
		win->node.Add(inspector_btn);

		pos = inspector_btn->GetWindow()->dimension.GetRect().GetNextPosRight(5);

		// Documentation button.
		auto doc_btn = ax::shared<ax::Button>(ax::Rect(pos, size), GetOnPyDocButton(), btn_info,
			"resources/graduate.png", "", ax::Button::Flags::SINGLE_IMG);
		AttachHelpInfo(doc_btn->GetWindow(), "Show pyo documentation.");
		win->node.Add(doc_btn);

		pos = doc_btn->GetWindow()->dimension.GetRect().GetNextPosRight(5);

		// Account button.
		auto account_btn = ax::shared<ax::Button>(ax::Rect(pos, size), GetOnAccountButton(), btn_info,
			"resources/account.png", "", ax::Button::Flags::SINGLE_IMG);
		AttachHelpInfo(account_btn->GetWindow(), "Show user account information.");
		win->node.Add(account_btn);
	}

	void RightSideMenu::SetInspectorHandle(ax::Window* handle)
	{
		_inspector->SetWidgetHandle(handle);
	}

	void RightSideMenu::RemoveInspectorHandle()
	{
		_inspector->RemoveHandle();
	}

	void RightSideMenu::OnInspectorButton(const ax::Button::Msg& msg)
	{
		_inspector->GetWindow()->Show();
		_pydoc->GetWindow()->Hide();
		_account->GetWindow()->Hide();
	}

	void RightSideMenu::OnPyDocButton(const ax::Button::Msg& msg)
	{
		_inspector->GetWindow()->Hide();
		_pydoc->GetWindow()->Show();
		_account->GetWindow()->Hide();
	}

	void RightSideMenu::OnAccountButton(const ax::Button::Msg& msg)
	{
		_inspector->GetWindow()->Hide();
		_pydoc->GetWindow()->Hide();
		_account->GetWindow()->Show();
	}

	void RightSideMenu::OnResize(const ax::Size& size)
	{
		_inspector->GetWindow()->dimension.SetSize(size - ax::Size(0, TOP_BAR_HEIGHT));
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