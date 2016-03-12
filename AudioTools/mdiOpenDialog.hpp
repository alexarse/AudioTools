#ifndef __MDI_OPEN_DIALOG_H__
#define __MDI_OPEN_DIALOG_H__

#include "axButton.h"
#include "axDropMenu.h"
#include "axLib.h"

namespace ax {
namespace editor {
	/*
	 * mdi::StatusBar.
	 */
	class OpenDialog : public ax::Window::Backbone {
	public:
		OpenDialog(const ax::Rect& rect);

		virtual ~OpenDialog()
		{
		}

		enum : ax::Event::Id { OPEN, CANCEL };

	private:
		std::shared_ptr<ax::DropMenu> _menu;

		axEVENT_DECLARATION(ax::Button::Msg, OnOpen);
		axEVENT_DECLARATION(ax::Button::Msg, OnCancel);
		axEVENT_DECLARATION(ax::DropMenu::Msg, OnMenuSelection);

		void DeleteDialog();

		void OnGlobalClick(const ax::Window::Event::GlobalClick& gclick);

		void OnMouseLeftDown(const ax::Point& pos);
		void OnPaint(ax::GC gc);
	};
}
}

#endif // __MDI_OPEN_DIALOG_H__
