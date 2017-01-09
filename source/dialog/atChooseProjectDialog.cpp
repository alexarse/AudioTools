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

#include "dialog/atChooseProjectDialog.hpp"

#include "atMainWindowProjectHandler.h"
#include "editor/atEditorMainWindow.hpp"
#include <axlib/WindowManager.hpp>

namespace at {
ChooseProjectDialog::ChooseProjectDialog(const ax::Rect& rect)
	: _font(0)
	, _font_app_name(0)
	, _logo_img("resources/audio_tools.png")
{
	_font_app_name.SetFontSize(30);
	win = ax::Window::Create(rect);
	win->event.OnPaint = ax::WBind<ax::GC>(this, &ChooseProjectDialog::OnPaint);

	//
	ax::Button::Info btn_info;
	btn_info.normal = ax::Color(0.97);
	btn_info.hover = ax::Color(0.99);
	btn_info.clicking = ax::Color(0.96);
	btn_info.selected = btn_info.normal;
	btn_info.contour = ax::Color(0.88);
	btn_info.font_color = ax::Color(0.0);
	btn_info.corner_radius = 0;

	ax::Point btn_pos(50, rect.size.h - 50);
	const ax::Size btn_size(88, 25);
	auto create_btn = ax::shared<ax::Button>(
		ax::Rect(btn_pos, btn_size), GetOnCreateProject(), btn_info, "", "Create project");
	win->node.Add(create_btn);

	btn_pos = create_btn->GetWindow()->dimension.GetRect().GetNextPosRight(10);
	auto open_btn = ax::shared<ax::Button>(
		ax::Rect(btn_pos, btn_size), GetOnOpenProject(), btn_info, "", "Open project");
	win->node.Add(open_btn);

	btn_pos = open_btn->GetWindow()->dimension.GetRect().GetNextPosRight(10);
	auto debug_btn = ax::shared<ax::Button>(
		ax::Rect(btn_pos, btn_size), GetOnDebugProject(), btn_info, "", "Debug project");
	win->node.Add(debug_btn);
}

std::shared_ptr<at::editor::MainWindow> ChooseProjectDialog::CreateMainWindow()
{
	ax::App& app(ax::App::GetInstance());
	auto win_manager = app.GetWindowManager();
	win_manager->SetPastWindow(nullptr);
	win_manager->ReleaseMouseHover();
	win_manager->ClearMouseManager();

	auto main_win = ax::shared<at::editor::MainWindow>(ax::Rect(0, 0, 1000, 700), app.AppOpenFilePath());

	auto this_window = win_manager->GetWindowTree()->GetNodeVector()[0];
	win_manager->GetWindowTree()->GetNodeVector().clear();

	app.SetDefaultBorder();
	app.SetResizable(true);
	app.SetFrameSize(ax::Size(1000, 700));
	app.AddTopLevel(main_win);
	app.SetFocusAndCenter();
	return main_win;
}

void ChooseProjectDialog::OnCreateProject(const ax::Button::Msg& msg)
{
	const std::string project_path = ax::App::GetInstance().SaveFileDialog();
	if (!at::editor::MainWindowProjectHandler::IsNewProjectPathValid(project_path)) {
		return;
	}

	std::shared_ptr<at::editor::MainWindow> main_window = CreateMainWindow();
	at::editor::MainWindowProjectHandler* pj_handler = main_window->GetProjectHandler();
	pj_handler->CreateProject(project_path);
}

void ChooseProjectDialog::OnOpenProject(const ax::Button::Msg& msg)
{
	const std::string project_path = ax::App::GetInstance().OpenFileDialog();
	if (!at::editor::MainWindowProjectHandler::IsProjectPathValid(project_path)) {
		return;
	}

	std::shared_ptr<at::editor::MainWindow> main_window = CreateMainWindow();
	at::editor::MainWindowProjectHandler* pj_handler = main_window->GetProjectHandler();
	pj_handler->OpenProject(project_path);
}

void ChooseProjectDialog::OnDebugProject(const ax::Button::Msg& msg)
{
	std::shared_ptr<at::editor::MainWindow> main_window = CreateMainWindow();
}

void ChooseProjectDialog::OnPaint(ax::GC gc)
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

	//	// Process bar.
	//	gc.DrawRectangle(ax::Rect(0, rect.size.h - 100, rect.size.w * _load_percent, 4));
	//
	//	// Current loading info.
	//	gc.DrawString(_font, _load_info, ax::Point(20, rect.size.h - 90));
}
}
