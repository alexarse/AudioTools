//
//  atMainWindowViewHandler.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-22.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef atMainWindowViewHandler_hpp
#define atMainWindowViewHandler_hpp

#include <OpenAX/Button.h>
#include <OpenAX/OpenAX.h>
#include <OpenAX/Toggle.h>

namespace at {
namespace editor {

	class MainWindow;

	class MainWindowViewHandler {
	public:
		MainWindowViewHandler(MainWindow* main_window);

		axEVENT_DECLARATION(ax::Event::SimpleMsg<int>, OnViewLayout);
		axEVENT_DECLARATION(ax::Button::Msg, OnBackToEditor);

		axEVENT_DECLARATION(ax::Event::SimpleMsg<int>, OnResizeCodeEditor);
		axEVENT_DECLARATION(ax::Toggle::Msg, OnToggleLeftPanel);
		axEVENT_DECLARATION(ax::Toggle::Msg, OnToggleBottomPanel);
		axEVENT_DECLARATION(ax::Toggle::Msg, OnToggleRightPanel);

		axEVENT_DECLARATION(ax::Button::Msg, OnSmallerLeftMenu);
		
		void OnResize(const ax::Size& size);

	private:
		MainWindow* _main_window;
	};
}
}

#endif /* atMainWindowViewHandler_hpp */
