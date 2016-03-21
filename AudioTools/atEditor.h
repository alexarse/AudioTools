#ifndef __AX_EDITOR_H__
#define __AX_EDITOR_H__

#include <OpenAX/OpenAX.h>

namespace at {
namespace editor {

	class MainWindow;

	class App {
	public:
		enum Events : ax::Event::Id { LOADING_EVT_ID };

		static App* Create();

		static inline App* GetInstance()
		{
			return _instance.get();
		}

		static ax::Event::Object* GetMainEvtObj();

		int MainLoop();

	private:
		static std::unique_ptr<App> _instance;
		ax::Event::Object _obj;
		std::thread _loading_thread;
		std::shared_ptr<MainWindow> _mainWindow;

		App();

		void SetupApplication();
	};
}
}

#endif // __AX_EDITOR_H__