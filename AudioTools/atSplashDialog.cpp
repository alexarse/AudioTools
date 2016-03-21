#include "atSplashDialog.hpp"

#include <OpenAX/Button.h>
#include <OpenAX/WindowManager.h>

#include "atEditorMainWindow.h"

namespace at {
SplashDialog::SplashDialog(const ax::Rect& rect)
	: _font(0)
	, _font_app_name(0)
	, _logo_img("resources/audio_tools.png")
	, _load_percent(0.0)
{
	_font_app_name.SetFontSize(30);
	win = ax::Window::Create(rect);
	win->event.OnPaint = ax::WBind<ax::GC>(this, &SplashDialog::OnPaint);
}

void SplashDialog::OnLoadingPercent(const ax::Event::SimpleMsg<LoadInfoMsg>& msg)
{
	ax::Print(msg.GetMsg().first);
	_load_percent = msg.GetMsg().first;
	_load_info = msg.GetMsg().second;
	
	win->Update();

	if (_load_percent == 1.0) {
		auto win_manager = ax::App::GetInstance().GetWindowManager();
		win_manager->SetPastWindow(nullptr);
		win_manager->ReleaseMouseHover();
		
		ax::App& app(ax::App::GetInstance());
		
		auto main_win = ax::shared<at::editor::MainWindow>(ax::Rect(0, 0, 1000, 700));
		
		auto this_window = win_manager->GetWindowTree()->GetNodeVector()[0];
		win_manager->GetWindowTree()->GetNodeVector().clear();
		
		app.SetResizable(true);
		app.SetTitleBar(true);
		app.SetFrameSize(ax::Size(1000, 700));
		app.AddTopLevel(main_win);
		app.SetFocusAndCenter();
	}
}

void SplashDialog::OnPaint(ax::GC gc)
{
	const ax::Rect rect(win->dimension.GetRect());
	gc.SetColor(ax::Color(250, 172, 10));
	gc.DrawRectangle(rect);

	if (_logo_img.IsImageReady()) {
		ax::Size logo_size(_logo_img.GetSize());
		gc.DrawImage(&_logo_img, ax::Point((rect.size.x - logo_size.x) * 0.5, 140));
	}

	// App name.
	gc.SetColor(ax::Color(1.0));
	gc.DrawStringAlignedCenter(_font_app_name, "Audio Tools", ax::Rect(0, 30, rect.size.x, 50));

	// Process bar.
	gc.DrawRectangle(ax::Rect(0, rect.size.y - 100, rect.size.x * _load_percent, 4));

	// Current loading info.
	gc.DrawString(_font, _load_info, ax::Point(20, rect.size.y - 90));
}
}