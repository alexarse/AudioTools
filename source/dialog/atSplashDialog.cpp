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

#include "dialog/atSplashDialog.hpp"
#include "dialog/atChooseProjectDialog.hpp"

#include <axlib/Button.hpp>
#include <axlib/WindowManager.hpp>

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

void SplashDialog::OnLoadingPercent(const ax::event::SimpleMsg<LoadInfoMsg>& msg)
{
	ax::console::Print(msg.GetMsg().first);
	_load_percent = msg.GetMsg().first;
	_load_info = msg.GetMsg().second;

	win->Update();

	if (_load_percent == 1.0) {
		// Remove mouse events.
		auto win_manager = ax::App::GetInstance().GetWindowManager();
		win_manager->SetPastWindow(nullptr);
		win_manager->ReleaseMouseHover();

		// Create choose project dialog.
		ax::App& app(ax::App::GetInstance());
		auto main_win = ax::shared<at::ChooseProjectDialog>(ax::Rect(0, 0, 400, 500));

		// Clean up window manager.
		auto this_window = win_manager->GetWindowTree()->GetNodeVector()[0];
		win_manager->GetWindowTree()->GetNodeVector().clear();

		app.AddTopLevel(main_win);
		main_win->GetWindow()->Update();
	}
}

void SplashDialog::OnPaint(ax::GC gc)
{
	const ax::Rect rect(win->dimension.GetRect());
	gc.SetColor(ax::Color(250, 172, 10));
	gc.DrawRectangle(rect);

	if (_logo_img.IsImageReady()) {
		ax::Size logo_size(_logo_img.GetSize());
		gc.DrawImage(&_logo_img, ax::Point((rect.size.w - logo_size.w) * 0.5, 140));
	}

	// App name.
	gc.SetColor(ax::Color(1.0));
	gc.DrawStringAlignedCenter(_font_app_name, "Audio Tools", ax::Rect(0, 30, rect.size.w, 50));

	// Process bar.
	gc.DrawRectangle(ax::Rect(0, rect.size.h - 100, rect.size.w * _load_percent, 4));

	// Current loading info.
	gc.DrawString(_font, _load_info, ax::Point(20, rect.size.h - 90));
}
}
