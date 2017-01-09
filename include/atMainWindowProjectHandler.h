//
//  atMainWindowProjectHandler.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-22.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef atMainWindowProjectHandler_hpp
#define atMainWindowProjectHandler_hpp

#include <axlib/axlib.hpp>

namespace at {
namespace editor {

	class MainWindow;

	class MainWindowProjectHandler {
	public:
		MainWindowProjectHandler(MainWindow* main_window);

		void SaveCurrentProject();

		bool OpenProject(const std::string& project_path);

		bool CreateProject(const std::string& project_path);

		static bool IsProjectPathValid(const std::string& project_path);

		static bool IsNewProjectPathValid(const std::string& project_path);

		axEVENT_DECLARATION(ax::event::StringMsg, OnSaveProject);
		axEVENT_DECLARATION(ax::event::StringMsg, OnSaveAsProject);
		axEVENT_DECLARATION(ax::event::StringMsg, OnOpenProject);
		axEVENT_DECLARATION(ax::event::StringMsg, OnCreateNewProject);

	private:
		MainWindow* _main_window;
	};
}
}

#endif /* atMainWindowProjectHandler_hpp */
