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

#include "dialog/atOpenDialog.hpp"
#include "editor/atEditorMainWindow.hpp"

#include <axlib/Core.hpp>
#include <axlib/FileSystem.hpp>
#include <axlib/Toggle.hpp>

namespace at {
namespace editor {
	OpenDialog::OpenDialog(const ax::Rect& rect)
	{
		// Create window.
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &OpenDialog::OnPaint);
		win->event.OnGlobalClick
			= ax::WBind<ax::Window::Event::GlobalClick>(this, &OpenDialog::OnGlobalClick);
		win->event.OnMouseLeftDown = ax::WBind<ax::Point>(this, &OpenDialog::OnMouseLeftDown);

		win->event.OnAssignToWindowManager = ax::WBind<int>(this, &OpenDialog::OnAssignToWindowManager);
		//		win->event.GrabGlobalMouse();
		//		ax::App::GetInstance().GetPopupManager()->AddGlobalClickListener(win);

		ax::DropMenu::Info menu_info;
		menu_info.normal = ax::Color(240, 240, 240);
		menu_info.hover = ax::Color(246, 246, 246);
		menu_info.font_color = ax::Color(0.0);
		menu_info.selected = ax::Color(41, 222, 255);
		menu_info.selected_hover = ax::Color(41, 226, 255);
		menu_info.selected_font_color = ax::Color(0.0);
		menu_info.contour = ax::Color(0.86);
		menu_info.separation = ax::Color(0.86);
		menu_info.up_down_arrow = ax::Color(0.35);
		menu_info.right_arrow = ax::Color(0.70);
		menu_info.item_height = 35;

		ax::os::Path dir("layouts/");
		//		ax::os::Directory dir;
		//		dir.Goto("layouts/");

		//		std::vector<ax::os::File> files = dir.GetContent();
		std::vector<ax::os::Path> files = dir.GetDirectoryContent();

		std::vector<std::string> layout_files;

		for (auto& n : files) {
			//		ax::console::Print(n.name);
			//			if (n.ext == "xml") {
			if (n.GetExtension() == "xml") {
				layout_files.push_back(n.GetAbsolutePath());
			}
		}

		for (auto& n : layout_files) {
			ax::console::Print(n);
		}

		//	ax::Size fsize(ax::App::GetInstance().GetFrameSize());
		ax::Size size(at::editor::MainWindow::WIDGET_MENU_WIDTH, rect.size.h);

		_menu = ax::shared<ax::DropMenu>(
			ax::Rect(ax::Point(0, 0), size), GetOnMenuSelection(), menu_info, layout_files);

		win->node.Add(_menu);

		ax::Size menu_size(_menu->GetWindow()->dimension.GetSize());

		auto open = ax::shared<ax::Button>(ax::Rect(rect.position.x, menu_size.h, size.w * 0.5, 30),
			GetOnOpen(), ax::Button::Info(), "", "Open");

		auto cancel = ax::shared<ax::Button>(
			ax::Rect(ax::Point(size.w * 0.5, menu_size.h), ax::Size(size.w * 0.5, 30)), GetOnCancel(),
			ax::Button::Info(), "", "Cancel");

		win->node.Add(open);
		win->node.Add(cancel);

		win->dimension.SetPosition(ax::Point(0, rect.position.y));

		//	win->dimension.SetSize(ax::Size(menu_size.w, menu_size.h + 30));

		//
		//		std::string font_path;
		//		ax::TextBox::Info txt_info;
		//		txt_info.normal = ax::Color(0.85);
		//		txt_info.hover = ax::Color(0.85);
		//		txt_info.highlight = ax::Color(0.6, 0.2); // This needs to be
		// transparent (alpha < 1.0).
		//		txt_info.selected = ax::Color(0.85);
		//		txt_info.selected_shadow = ax::Color(0.85);
		//		txt_info.cursor = ax::Color(0.0);
		//		txt_info.contour = ax::Color(0.3);;
		//		txt_info.font_color = ax::Color(0.0);
		//
		//		_txtBox = ax::shared<ax::TextBox>(ax::Rect(10, 10, 200, 30),
		// ax::TextBox::Events(), txt_info, "",
		//"default.xml");
		//		win->node.Add(_txtBox);
		//
		//		auto save = ax::shared<ax::Button>(ax::Rect(10, 50, 60, 30),
		// GetOnSave(), ax::Button::Info(), "",
		//"Save");
		//		auto cancel = ax::shared<ax::Button>(ax::Rect(10, 120, 60, 30),
		// GetOnCancel(), ax::Button::Info(),
		//"", "Cancel");
		//
		//		win->node.Add(save);
		//		win->node.Add(cancel);
	}

	void OpenDialog::OnAssignToWindowManager(const int& v)
	{
		win->event.GrabGlobalMouse();
		//		win->event.GrabGlobalKey();
	}

	void OpenDialog::OnGlobalClick(const ax::Window::Event::GlobalClick& gclick)
	{
		if (!win->dimension.GetAbsoluteRect().IsPointInside(gclick.pos)) {
			DeleteDialog();
		}
	}

	void OpenDialog::OnOpen(const ax::Button::Msg& msg)
	{
		//		std::string label = _txtBox->GetLabel();
		//		ax::console::Print(label);
		win->PushEvent(OPEN, new ax::event::StringMsg(_menu->GetSelectedItem()));
		DeleteDialog();
	}

	void OpenDialog::OnCancel(const ax::Button::Msg& msg)
	{
		win->PushEvent(CANCEL, new ax::event::StringMsg(""));
		DeleteDialog();
	}

	void OpenDialog::OnMenuSelection(const ax::DropMenu::Msg& msg)
	{
		win->PushEvent(OPEN, new ax::event::StringMsg(msg.GetItem()));
		DeleteDialog();
	}

	void OpenDialog::DeleteDialog()
	{
		ax::App::GetInstance().GetWindowManager()->SetPastWindow(nullptr);
		ax::App::GetInstance().GetWindowManager()->UnGrabKey();
		ax::App::GetInstance().GetWindowManager()->UnGrabMouse();

		ax::App::GetInstance().GetPopupManager()->Clear();
		//		ax::App::GetInstance().GetPopupManager()->RemoveGlobalClickListener(win);
		//		ax::App::GetInstance().GetPopupManager()->GetWindowTree()->GetNodeVector().clear();
		//		ax::App::GetInstance().GetPopupManager()->UnGrabKey();
		//		ax::App::GetInstance().GetPopupManager()->UnGrabMouse();
		//		ax::App::GetInstance().GetPopupManager()->SetPastWindow(nullptr);
		ax::App::GetInstance().UpdateAll();
	}

	void OpenDialog::OnMouseLeftDown(const ax::Point& pos)
	{
		DeleteDialog();
	}

	void OpenDialog::OnPaint(ax::GC gc)
	{
		const ax::Rect rect(win->dimension.GetDrawingRect());

		gc.SetColor(ax::Color(0.0, 0.6));
		gc.DrawRectangle(rect);
		gc.DrawRectangleContour(rect);
	}
}
}
