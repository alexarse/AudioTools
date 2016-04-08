#include <OpenAX/Core.h>

#include "atCommon.h"
#include "atEditor.h"
#include "atEditorMainWindow.h"

#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>

#include "PyoAudio.h"
#include <OpenAX/axMidiCore.h>

#include "atSplashDialog.hpp"

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
		return ax::App::GetInstance().GetTopLevel().get();
	}

	App::App()
		: _obj(ax::App::GetInstance().GetEventManager())
	{
		SetupApplication();
	}

	void App::SetupApplication()
	{
		ax::App& app(ax::App::GetInstance());

		app.AddMainEntry([&]() {
			app.SetFrameSize(ax::Size(400, 500));
			auto splash_screen = ax::shared<at::SplashDialog>(ax::Rect(0, 0, 400, 500));
			_obj.AddConnection(Events::LOADING_EVT_ID, splash_screen->GetOnLoadingPercent());
			app.AddTopLevel(splash_screen);
		});

		app.AddAfterGUILoadFunction([&]() {
			app.SetFrameSize(ax::Size(400, 500));
			app.SetFocusAndCenter();

			// Start loading thread (audio and data).
			_loading_thread = std::thread(
				[](ax::Event::Object& obj) {

					using MsgType = ax::Event::SimpleMsg<at::SplashDialog::LoadInfoMsg>;
					obj.PushEvent(Events::LOADING_EVT_ID,
						new MsgType(at::SplashDialog::LoadInfoMsg(0.2, "Loading audio ...")));

					PyoAudio* audio = PyoAudio::GetInstance();
					audio->InitAudio();
					audio->StartAudio();

					obj.PushEvent(Events::LOADING_EVT_ID,
						new MsgType(at::SplashDialog::LoadInfoMsg(0.7, "Load midi ...")));
					
					/// @todo Save this somewhere.
					Midi* midi = new Midi(audio);

					obj.PushEvent(
						Events::LOADING_EVT_ID, new MsgType(at::SplashDialog::LoadInfoMsg(1.0, "Done")));

				},
				std::ref(_obj));
			_loading_thread.detach();
		});
	}

	int App::MainLoop()
	{
		char usr_name[200];
		int err = getlogin_r(usr_name, 200);

		if(err != 0) {
			ax::Error("Can't get unser name.");
		}

		ax::Print("User name :", usr_name);

		struct passwd* pw = getpwuid(getuid());
		const char* homedir = pw->pw_dir;
		ax::Print("Home dir :", homedir);

		std::string path(homedir + std::string("/Library/Application Support/AudioTools"));

		if (chdir(path.c_str()) == -1) {
			ax::Error("Could not set current directory : ", path, ".");
		}

		ax::App::GetInstance().MainLoop();
		return 0;
	}
}
}