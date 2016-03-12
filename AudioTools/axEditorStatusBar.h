#ifndef mdiStatusBar_hpp
#define mdiStatusBar_hpp

#include "axButton.h"
#include "axLib.h"
#include "axToggle.h"

namespace ax {
namespace editor {
	/*
	 * ax::editor::StatusBar.
	 */
	class StatusBar : public ax::Window::Backbone {
	public:
		StatusBar(const ax::Rect& rect);

		enum : ax::Event::Id {
			SAVE_LAYOUT,
			OPEN_LAYOUT,
			VIEW_LAYOUT,
			TOGGLE_LEFT_PANEL,
			TOGGLE_BOTTOM_PANEL,
			TOGGLE_RIGHT_PANEL
		};

	private:
		ax::Toggle* _toggle_left;
		ax::Toggle* _toggle_bottom;
		ax::Toggle* _toggle_right;
		axEVENT_DECLARATION(ax::Button::Msg, OnSaveLayout);
		axEVENT_DECLARATION(ax::Button::Msg, OnOpenLayout);
		axEVENT_DECLARATION(ax::Button::Msg, OnViewLayout);

		axEVENT_DECLARATION(ax::Toggle::Msg, OnToggleLeftPanel);
		axEVENT_DECLARATION(ax::Toggle::Msg, OnToggleBottomPanel);
		axEVENT_DECLARATION(ax::Toggle::Msg, OnToggleRightPanel);

		axEVENT_DECLARATION(ax::Event::StringMsg, OnSaveDialog);
		axEVENT_DECLARATION(ax::Event::StringMsg, OnCancelDialog);
		
		axEVENT_DECLARATION(ax::Event::StringMsg, OnOpenDialog);

		void OnResize(const ax::Size& size);
		void OnPaint(ax::GC gc);
	};
}
}

#endif /* mdiStatusBar_hpp */
