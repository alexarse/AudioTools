//
//  atMainWindowViewHandler.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-22.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef atMainWindowViewHandler_hpp
#define atMainWindowViewHandler_hpp

#include <axlib/Button.hpp>
#include <axlib/Toggle.hpp>
#include <axlib/axlib.hpp>

namespace at {
namespace editor {

	class MainWindow;

	class MainWindowViewHandler {
	public:
		MainWindowViewHandler(MainWindow* main_window);

		axEVENT_DECLARATION(ax::event::SimpleMsg<int>, OnViewLayout);
		axEVENT_DECLARATION(ax::Button::Msg, OnBackToEditor);

		axEVENT_DECLARATION(ax::event::SimpleMsg<int>, OnResizeCodeEditor);
		axEVENT_DECLARATION(ax::Toggle::Msg, OnToggleLeftPanel);
		axEVENT_DECLARATION(ax::Toggle::Msg, OnToggleBottomPanel);
		axEVENT_DECLARATION(ax::Toggle::Msg, OnToggleRightPanel);

		axEVENT_DECLARATION(ax::Button::Msg, OnSmallerLeftMenu);

		void OnResize(const ax::Size& size);

	private:
		MainWindow* _main_window;

		struct ViewInfo {
			ax::Point old_main_window_position;
			bool left_menu_shown;
			bool right_menu_shown;
			bool editor_shown;
			ax::Size old_frame_size;
		};

		ViewInfo _view_info;
		bool _view_mode = false;
	};
}
}

#endif /* atMainWindowViewHandler_hpp */
