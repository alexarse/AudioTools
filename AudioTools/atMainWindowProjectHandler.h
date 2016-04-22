//
//  atMainWindowProjectHandler.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-22.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef atMainWindowProjectHandler_hpp
#define atMainWindowProjectHandler_hpp

#include <OpenAX/OpenAX.h>

namespace at {
namespace editor {

	class MainWindow;

	class MainWindowProjectHandler {
	public:
		MainWindowProjectHandler(MainWindow* main_window);

		void SaveCurrentProject();

		axEVENT_DECLARATION(ax::Event::StringMsg, OnSaveProject);
		axEVENT_DECLARATION(ax::Event::StringMsg, OnSaveAsProject);
		axEVENT_DECLARATION(ax::Event::StringMsg, OnOpenProject);
		axEVENT_DECLARATION(ax::Event::StringMsg, OnCreateNewProject);

	private:
		MainWindow* _main_window;
	};
}
}

#endif /* atMainWindowProjectHandler_hpp */
