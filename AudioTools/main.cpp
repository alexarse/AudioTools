#include "axLib.h"
#include "axWindowManager.h"
//#include "axWindowServerFrame.h"
//#include "axWindowServerManager.h"

#include "axDropMenu.h"
#include "axLabel.h"
#include "axScrollBar.h"
#include "axTextBox.h"

//#include "axScript.h"
#include "axMidiCore.h"

#include "PyoAudio.h"
#include "axEditor.h"

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

int main()
{
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		fprintf(stdout, "Current working dir: %s\n", cwd);
	else
		perror("getcwd() error");
	
	PyoAudio* audio = PyoAudio::GetInstance();
	audio->InitAudio();
	audio->StartAudio();
	
	Midi* midi = new Midi(audio);
	
	ax::editor::App* app = ax::editor::App::Create();
	ax::Print(ax::App::GetInstance().GetAppDirectory());
	return app->MainLoop();

	//	ax::App& app(ax::App::GetInstance());
	//	app.AddMainEntry([&app]() {
	////		app.AddTopLevel(ax::shared<Editor>(ax::Rect(0, 0, 800, 800)));
	//		// Main window.
	//		ax::Window::Ptr win(ax::Window::Create(ax::Rect(0, 0, 800, 800)));
	//
	//		ax::Print("WINDOW CREATE");
	//
	//		win->event.OnPaint = ax::WFunc<ax::GC>([win](ax::GC gc) {
	//			const ax::Rect rect(win->dimension.GetRect());
	//			gc.SetColor(ax::Color(1.0));
	//			gc.DrawRectangle(rect);
	//		});
	//
	//		win->event.OnGlobalClick =
	//ax::WFunc<ax::Window::Event::GlobalClick>(
	//			[win](const ax::Window::Event::GlobalClick& msg) {
	//				using GClick = ax::Window::Event::GlobalClick;
	//
	//				if (msg.type == GClick::RIGHT_CLICK_DOWN
	//					|| msg.type == GClick::LEFT_CLICK_DOWN) {
	//
	//					ax::App& app = ax::App::GetInstance();
	//					auto man = app.GetWindowManager();
	//					auto popup = app.GetPopupManager();
	//					auto menu = popup->GetWindowTree()->GetTopLevel();
	//
	//					man->RemoveGlobalClickListener(win.get());
	//					menu->backbone = nullptr;
	//
	//					popup->GetWindowTree()->GetNodeVector().clear();
	//					popup->SetPastWindow(nullptr);
	//
	//					win->Update();
	//				}
	//			});
	//
	//		win->event.OnMouseRightDown
	//			= ax::WFunc<ax::Point>([win](const ax::Point& pos) {
	//				  ax::DropMenu::Info menu_info;
	//				  menu_info.normal = ax::Color(240, 240, 240);
	//				  menu_info.hover = ax::Color(246, 246, 246);
	//				  menu_info.font_color = ax::Color(0.0);
	//				  menu_info.selected = ax::Color(41, 222, 255);
	//				  menu_info.selected_hover = ax::Color(41, 226, 255);
	//				  menu_info.selected_font_color = ax::Color(0.0);
	//				  menu_info.contour = ax::Color(0.86);
	//				  menu_info.separation = ax::Color(0.86);
	//				  menu_info.up_down_arrow = ax::Color(0.35);
	//				  menu_info.right_arrow = ax::Color(0.70);
	//				  menu_info.item_height = 30;
	//
	//				  ax::StringVector items = { "Alexandre", "Peter", "",
	//					  "John Doe", ">", "Steeve Scott", "", "Mario",
	//					  "Etienne McFart", "Alexandre", "Peter", "John Doe",
	//					  "Steeve Scott", "Mario", "Etienne McFart" };
	//
	//				  auto menu = ax::shared<ax::DropMenu>(
	//					  ax::Rect(pos, ax::Size(200, 305)),
	//ax::DropMenu::Events(),
	//					  menu_info, items);
	//
	//				  menu->GetWindow()->backbone = menu;
	//
	//				  ax::App& app(ax::App::GetInstance());
	//				  app.GetPopupManager()->GetWindowTree()->AddTopLevel(
	//					  ax::Window::Ptr(menu->GetWindow()));
	//				  win->Update();
	//
	//				  app.GetWindowManager()->AddGlobalClickListener(win.get());
	//			  });
	//
	//		app.AddTopLevel(win);
	//
	//		ax::Window* scripted_win = ax::Window::Create(ax::Rect(400, 100, 100,
	//100));
	//		scripted_win->event.OnPaint = ax::WFunc<ax::GC>([scripted_win](ax::GC
	//gc) {
	//			ax::Script script;
	//			script.AddWindowHandle(scripted_win);
	//			script.ProcessFile("draw.chai");
	//		});
	//
	//		win->node.Add(ax::Window::Ptr(scripted_win));
	//
	//		ax::Label::Info label_info;
	//		label_info.normal = ax::Color(1.0, 1.0);
	//		label_info.contour = ax::Color(0.0, 0.0);
	//		label_info.font_color = ax::Color(0.0);
	//		label_info.font_size = 18;
	//		label_info.font_name = "LatoLight.ttf";
	//		label_info.alignement = ax::Utils::Alignement::axALIGN_CENTER;
	//
	//		win->node.Add(ax::shared<ax::Label>(
	//			ax::Rect(300, 100, 100, 25), label_info, "Patate"));
	//
	//		ax::TextBox::Info txt_info;
	//		txt_info.normal = ax::Color(1.0);
	//		txt_info.hover = ax::Color(1.0);
	//		txt_info.highlight = ax::Color(0.0f, 0.0f, 0.8f, 0.2f);
	//		txt_info.selected = ax::Color(1.0);
	//		txt_info.selected_shadow = ax::Color(0.0f, 0.0f, 1.0f, 0.2f);
	//		txt_info.cursor = ax::Color(0.35);
	//		txt_info.contour = ax::Color(0.0);
	//		txt_info.font_color = ax::Color(0.0);
	//
	//		win->node.Add(ax::shared<ax::TextBox>(ax::Rect(10, 100, 200, 25),
	//			ax::TextBox::Events(), txt_info, "", "Test",
	//			ax::TextBox::Flags::FLASHING_CURSOR
	//				| ax::TextBox::Flags::CONTOUR_HIGHLIGHT));
	//
	//		ax::DropMenu::Info menu_info;
	//		menu_info.normal = ax::Color(240, 240, 240);
	//		menu_info.hover = ax::Color(246, 246, 246);
	//		menu_info.font_color = ax::Color(0.0);
	//		menu_info.selected = ax::Color(41, 222, 255);
	//		menu_info.selected_hover = ax::Color(41, 226, 255);
	//		menu_info.selected_font_color = ax::Color(0.0);
	//		menu_info.contour = ax::Color(0.86);
	//		menu_info.separation = ax::Color(0.86);
	//		menu_info.up_down_arrow = ax::Color(0.35);
	//		menu_info.right_arrow = ax::Color(0.70);
	//		menu_info.item_height = 35;
	//
	//		ax::StringVector items = { "Alexandre", "Peter", "", "John Doe",
	//">",
	//			"Steeve Scott", "", "Mario", "Etienne McFart" };
	//
	//		ax::Event::Function menu_evt = [](ax::Event::Msg* msg) {
	//			ax::DropMenu::Msg* dmsg = static_cast<ax::DropMenu::Msg*>(msg);
	//			ax::Print(dmsg->GetItem());
	//		};
	//
	//		win->node.Add(ax::shared<ax::DropMenu>(ax::Rect(10, 300, 200, 205),
	//			ax::DropMenu::Events(menu_evt), menu_info, items));
	//
	//		win->node.Add(ax::shared<ax::DropMenu>(ax::Rect(320, 300, 200, 210),
	//			ax::DropMenu::Events(), menu_info, items));
	//
	//	});
	//
	//	app.AddAfterGUILoadFunction(
	//		[&app]() { app.SetFrameSize(ax::Size(800, 800)); });
	//
	//	app.MainLoop();

	return 0;
}
