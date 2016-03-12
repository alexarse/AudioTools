#include "axCore.h"
#include "axEditor.h"
#include "axEditorMainWindow.h"
#include "atCommon.h"
//#include "mdiApplicationManager.hpp"
#include <unistd.h>
namespace ax {
namespace editor {
	std::unique_ptr<App> App::_instance = nullptr;

	App* App::Create()
	{
		if (_instance == nullptr) {
			_instance = std::unique_ptr<App>(new App());
		}
		return _instance.get();
	}

	ax::Event::Object* App::GetMainEvtObj()
	{
		auto& app(App::_instance);
		return app->_mainWindow->GetWindow();
	}

	App::App()
	{
		SetupApplication();
	}

	void App::SetupApplication()
	{
		ax::App& app(ax::App::GetInstance());

		app.AddMainEntry([&]() {
			const ax::Size size(1000, 700);
			app.SetFrameSize(size);

			_mainWindow = ax::shared<MainWindow>(ax::Rect(0, 0, size));
			app.AddTopLevel(_mainWindow);
		});

		app.AddAfterGUILoadFunction(
			[&app]() { app.SetFrameSize(ax::Size(1000, 700)); });
	}

	int App::MainLoop()
	{
//		if(chdir(APP_FOLDER.c_str()) == -1) {
		char usr_name[200];
		int err = getlogin_r(usr_name, 200);

		ax::Print("User name :", usr_name);

		std::string path(std::string("/Users/") + usr_name + std::string("/Library/Application Support/AudioTools"));

		if(chdir(path.c_str()) == -1) {
			ax::Error("Could not set current directory.");
		}
		
		ax::App::GetInstance().MainLoop();
		return 0;
	}
}
}