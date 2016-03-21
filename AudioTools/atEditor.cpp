#include <OpenAX/Core.h>

#include "atEditor.h"
#include "atEditorMainWindow.h"
#include "atCommon.h"

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>


#include <OpenAX/axMidiCore.h>
#include "PyoAudio.h"

class Midi : public ax::midi::Core {
public:
	Midi(PyoAudio* audio)
	: _audio(audio)
	{
		
	}
	
	virtual void OnMidiNoteOn(const ax::midi::Note& msg)
	{
		_audio->ProcessMidi(144, msg.GetNote(), msg.GetVelocity());
		ax::Print("ON", msg.GetNote(), msg.GetVelocity());
	}
	
	virtual void OnMidiNoteOff(const ax::midi::Note& msg)
	{
		_audio->ProcessMidi(144, msg.GetNote(), msg.GetVelocity());
		ax::Print("OFF", msg.GetNote(), msg.GetVelocity());
	}
	
private:
	PyoAudio* _audio;
	
};

namespace at {
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

		struct passwd *pw = getpwuid(getuid());
		const char* homedir = pw->pw_dir;
		ax::Print("Home dir :", homedir);
		
		std::string path(homedir + std::string("/Library/Application Support/AudioTools"));

		if(chdir(path.c_str()) == -1) {
			ax::Error("Could not set current directory : ", path, ".");
		}
		
		
		PyoAudio* audio = PyoAudio::GetInstance();
		audio->InitAudio();
		audio->StartAudio();
		
		Midi* midi = new Midi(audio);
		
		ax::App::GetInstance().MainLoop();
		return 0;
	}
}
}