/*
 * Copyright (c) 2016 AudioTools - All Rights Reserved
 *
 * This Software may not be distributed in parts or its entirety
 * without prior written agreement by AudioTools.
 *
 * Neither the name of the AudioTools nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY AUDIOTOOLS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL AUDIOTOOLS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Written by Alexandre Arsenault <alx.arsenault@gmail.com>
 */

#include <axlib/Core.hpp>

#include "atCommon.hpp"
#include "editor/atEditor.hpp"
#include "editor/atEditorMainWindow.hpp"

#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>

#include "PyoAudio.h"
#include "atMidi.h"

#include "atSkin.hpp"
#include "dialog/atSplashDialog.hpp"

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

	ax::event::Object* App::GetMainEvtObj()
	{
		return ax::App::GetInstance().GetTopLevel().get();
	}

	App::App()
		: _obj(ax::App::GetInstance().GetEventManager())
	{
		SetupApplication();
	}

	void App::OnSplashOpen(ax::event::Msg* msg)
	{
		ax::App& app(ax::App::GetInstance());
		app.SetBorderLess();
	}

	void App::SetupApplication()
	{
		//		_on_splash_open = [](ax::event::Msg* msg) {
		//			ax::App& app(ax::App::GetInstance());
		//			app.SetBorderLess();
		//		};

		ax::App& app(ax::App::GetInstance());
		app.GetEventManager()->AddConnection(0, 2000, ax::event::Bind(this, &App::OnSplashOpen));

		at::Skin::GetInstance()->SetLightSkin();
		// at::Skin::GetInstance()->SetDarkSkin();

		app.AddMainEntry([&]() {
			app.SetFrameSize(ax::Size(400, 500));
			auto splash_screen = ax::shared<at::SplashDialog>(ax::Rect(0, 0, 400, 500));
			_obj.AddConnection(Events::LOADING_EVT_ID, splash_screen->GetOnLoadingPercent());
			app.AddTopLevel(splash_screen);
		});

		app.AddAfterGUILoadFunction([&]() {
			app.SetFrameSize(ax::Size(400, 500));
			// app.SetResizable(false);
			// app.SetTitleBar(false);
			app.SetFocusAndCenter();
			//			app.SetBorderLess();

			app.GetEventManager()->PushEvent(0, 2000, new ax::event::EmptyMsg());
			// app.GetEventManager()->AddFunction(ax::event::axBindedEvent(_on_splash_open, new
			// ax::event::EmptyMsg()));

			// Start loading thread (audio and data).
			_loading_thread = std::thread(
				[](ax::event::Object& obj) {
					using MsgType = ax::event::SimpleMsg<at::SplashDialog::LoadInfoMsg>;
					obj.PushEvent(Events::LOADING_EVT_ID,
						new MsgType(at::SplashDialog::LoadInfoMsg(0.2, "Loading audio ...")));

					PyoAudio* audio = PyoAudio::GetInstance();
					audio->InitAudio();
					audio->StartAudio();

					obj.PushEvent(Events::LOADING_EVT_ID,
						new MsgType(at::SplashDialog::LoadInfoMsg(0.7, "Load midi ...")));

					/// @todo Save this somewhere.
					// at::Midi* midi = at::Midi::GetInstance();
					at::Midi::GetInstance();

					obj.PushEvent(
						Events::LOADING_EVT_ID, new MsgType(at::SplashDialog::LoadInfoMsg(1.0, "Done")));

				},
				std::ref(_obj));
			_loading_thread.detach();
		});
	}

	MainWindow* App::GetMainWindow()
	{
		auto w = ax::App::GetInstance().GetWindowManager()->GetWindowTree()->GetTopLevel();
		return static_cast<MainWindow*>(w->backbone.get());
	}

	int App::MainLoop()
	{
		char usr_name[200];
		int err = getlogin_r(usr_name, 200);

		if (err != 0) {
			ax::console::Error("Can't get unser name.");
		}

		ax::console::Print("User name :", usr_name);

		struct passwd* pw = getpwuid(getuid());
		const char* homedir = pw->pw_dir;
		ax::console::Print("Home dir :", homedir);

		//		std::string path(homedir + std::string("/Library/Application Support/AudioTools"));
		//		std::string path("/Users/Shared/Library/Application Support/AudioTools");
		std::string path("/Users/Shared/AudioTools");

		if (chdir(path.c_str()) == -1) {
			ax::console::Error("Could not set current directory : ", path, ".");
		}

		ax::App::GetInstance().MainLoop();

		return 0;
	}
}
}
