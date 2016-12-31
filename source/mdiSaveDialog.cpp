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

#include "dialog/atSaveDialog.hpp"

#include <axlib/Core.hpp>
#include <axlib/Toggle.hpp>

namespace at {
namespace editor {
	SaveDialog::SaveDialog(const ax::Rect& rect, const std::string& default_name)
	{
		// Create window.
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &SaveDialog::OnPaint);
		win->event.OnGlobalClick
			= ax::WBind<ax::Window::Event::GlobalClick>(this, &SaveDialog::OnGlobalClick);

		win->event.GrabGlobalMouse();
		//		ax::App::GetInstance().GetWindowManager()->AddGlobalClickListener(win);

		ax::TextBox::Info txt_info;
		txt_info.normal = ax::Color(0.85);
		txt_info.hover = ax::Color(0.85);
		txt_info.highlight = ax::Color(0.6, 0.2); // This needs to be transparent (alpha < 1.0).
		txt_info.selected = ax::Color(0.85);
		txt_info.selected_shadow = ax::Color(0.85);
		txt_info.cursor = ax::Color(0.0);
		txt_info.contour = ax::Color(0.3);
		;
		txt_info.font_color = ax::Color(0.0);

		_txtBox = ax::shared<ax::TextBox>(
			ax::Rect(10, 10, 200, 30), ax::TextBox::Events(), txt_info, "", default_name);
		win->node.Add(_txtBox);

		auto save
			= ax::shared<ax::Button>(ax::Rect(10, 50, 60, 30), GetOnSave(), ax::Button::Info(), "", "Save");
		auto cancel = ax::shared<ax::Button>(
			ax::Rect(10, 120, 60, 30), GetOnCancel(), ax::Button::Info(), "", "Cancel");

		win->node.Add(save);
		win->node.Add(cancel);
	}

	void SaveDialog::OnGlobalClick(const ax::Window::Event::GlobalClick& gclick)
	{
		DeleteDialog();
	}

	void SaveDialog::OnSave(const ax::Button::Msg& msg)
	{
		std::string label = _txtBox->GetLabel();
		ax::console::Print(label);
		win->PushEvent(SAVE, new ax::event::StringMsg(label));
		DeleteDialog();
	}

	void SaveDialog::OnCancel(const ax::Button::Msg& msg)
	{
		win->PushEvent(CANCEL, new ax::event::StringMsg(""));
		DeleteDialog();
	}

	void SaveDialog::DeleteDialog()
	{
		/// @todo Change this.
		ax::App::GetInstance().GetWindowManager()->UnGrabGlobalMouse(win);
		ax::App::GetInstance().GetWindowManager()->SetPastWindow(nullptr);
		ax::App::GetInstance().GetWindowManager()->UnGrabKey();
		ax::App::GetInstance().GetWindowManager()->UnGrabMouse();

		win->event.UnGrabKey();
		win->event.UnGrabMouse();

		win->backbone = nullptr;
		ax::App::GetInstance().GetPopupManager()->GetWindowTree()->GetNodeVector().clear();
		ax::App::GetInstance().GetPopupManager()->SetPastWindow(nullptr);
		ax::console::Print("Delete window.");
	}

	void SaveDialog::OnPaint(ax::GC gc)
	{
		ax::Rect rect(win->dimension.GetDrawingRect());

		gc.SetColor(ax::Color(0.80));
		gc.DrawRectangle(rect);
		//		gc.DrawRectangleColorFade(rect, ax::Color(0.85),
		// ax::Color(0.90));

		//		gc.DrawRectangleContour(rect);
	}
}
}
