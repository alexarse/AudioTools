/*
 * Copyright (c) 2016 AudioTools - All Rights Reserved
 *
 * This Software may not be distributed in parts or its entirety
 * without prior written agreement by AutioTools.
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

#include "atSplashDialog.hpp"

#include <OpenAX/Button.h>
#include <OpenAX/WindowManager.h>

#include "editor/atEditorMainWindow.hpp"

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

		auto main_win = ax::shared<at::editor::MainWindow>(ax::Rect(0, 0, 1000, 700), app.AppOpenFilePath());

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
