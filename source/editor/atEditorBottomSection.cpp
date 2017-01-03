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

#include "editor/atEditorBottomSection.hpp"
#include "atHelpBar.h"
#include "atSkin.hpp"

namespace at {
namespace editor {
	BottomSection::BottomSection(const ax::Rect& rect, const std::string& open_path)
		: _file_path("Banana")
		, _font(0)
		, _is_txt_edit(true)
	{
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &BottomSection::OnPaint);
		win->event.OnResize = ax::WBind<ax::Size>(this, &BottomSection::OnResize);

		win->event.OnMouseLeave = ax::WBind<ax::Point>(this, &BottomSection::OnMouseLeave);
		win->event.OnMouseMotion = ax::WBind<ax::Point>(this, &BottomSection::OnMouseMotion);
		win->event.OnMouseLeftDown = ax::WBind<ax::Point>(this, &BottomSection::OnMouseLeftDown);
		win->event.OnMouseLeftDragging = ax::WBind<ax::Point>(this, &BottomSection::OnMouseLeftDragging);
		win->event.OnMouseLeftUp = ax::WBind<ax::Point>(this, &BottomSection::OnMouseLeftUp);
		win->event.OnMouseLeftDoubleClick
			= ax::WBind<ax::Point>(this, &BottomSection::OnMouseLeftDoubleClick);

		TextEditor::Info txt_info;
		txt_info.bg_color = ax::Color(1.0);
		txt_info.cursor_color = ax::Color(0.0);
		txt_info.line_number_bg_color = ax::Color(0.95);
		txt_info.line_number_color = ax::Color(0.4);
		txt_info.text_color = ax::Color(0.0);

		auto txt_editor = ax::shared<TextEditor>(
			ax::Rect(0, TOP_BAR_HEIGHT, rect.size.w - 1, rect.size.h - TOP_BAR_HEIGHT), txt_info);
		_txt_editor = txt_editor.get();
		win->node.Add(txt_editor);

		auto console
			= ax::shared<Console>(ax::Rect(0, TOP_BAR_HEIGHT, rect.size.w - 1, rect.size.h - TOP_BAR_HEIGHT));
		_console = console.get();
		win->node.Add(console);
		_console->GetWindow()->Hide();

		_console->GetWindow()->AddConnection(Console::WRITE_ERROR, GetOnConsoleErrorUpdate());

		// @todo Change this.
		_txt_editor->OpenFile(open_path);
		_file_path = _txt_editor->GetFilePath();

		ax::Button::Info btn_info;
		btn_info.normal = ax::Color(0.0, 0.0);
		btn_info.hover = ax::Color(0.0, 0.0);
		btn_info.clicking = ax::Color(0.0, 0.0);
		btn_info.selected = ax::Color(0.0, 0.0);
		btn_info.contour = ax::Color(0.0, 0.0);
		btn_info.font_color = ax::Color(0.0, 0.0);

		// Text editor button.
		auto txt_btn = ax::shared<at::ColorButton>(ax::Rect(5, 2, 20, 20), GetOnTextEditor(), btn_info,
			"resources/txt_btn.png", "", ax::Button::Flags::SINGLE_IMG | ax::Button::Flags::IMG_RESIZE);
		_txt_btn = txt_btn.get();
		_txt_btn->SetSelected(true);
		win->node.Add(txt_btn);

		AttachHelpInfo(txt_btn->GetWindow(), "Switch to code editor.");

		ax::Point pos = _txt_btn->GetWindow()->dimension.GetRect().GetNextPosRight(5);

		// Console button.
		auto console_btn = ax::shared<at::ColorButton>(ax::Rect(pos, ax::Size(20, 20)), GetOnConsole(),
			btn_info, "resources/console_btn.png", "",
			ax::Button::Flags::SINGLE_IMG | ax::Button::Flags::IMG_RESIZE);
		_console_btn = console_btn.get();
		win->node.Add(console_btn);

		AttachHelpInfo(console_btn->GetWindow(), "Switch to console.");

		// Clean up console.
		auto console_clean_btn = ax::shared<at::ColorButton>(
			ax::Rect(ax::Point(rect.size.w - 25, 2), ax::Size(20, 20)), GetOnConsoleClean(), btn_info,
			"resources/rubbish.png", "", ax::Button::Flags::SINGLE_IMG | ax::Button::Flags::IMG_RESIZE);
		_console_clean_btn = console_clean_btn.get();
		win->node.Add(console_clean_btn);

		AttachHelpInfo(console_clean_btn->GetWindow(), "Erase console content.");
		_console_clean_btn->GetWindow()->Hide();
	}

	bool BottomSection::OpenFile(const std::string& path)
	{
		bool err = _txt_editor->OpenFile(path);
		_file_path = path;
		win->Update();
		return err;
	}

	void BottomSection::SaveFile(const std::string& path)
	{
		_txt_editor->SaveFile(path);
	}

	std::string BottomSection::GetScriptPath() const
	{
		return _file_path;
	}

	void BottomSection::OnTextEditor(const ax::Button::Msg& msg)
	{
		_txt_editor->GetWindow()->Show();
		_console->GetWindow()->Hide();
		_console_clean_btn->GetWindow()->Hide();

		if (!_is_txt_edit) {
			_is_txt_edit = true;
			_txt_btn->SetSelected(true);
			_console_btn->SetSelected(false);
			win->Update();
		}
	}

	void BottomSection::OnConsole(const ax::Button::Msg& msg)
	{
		_console->GetWindow()->Show();
		_console_clean_btn->GetWindow()->Show();
		_txt_editor->GetWindow()->Hide();

		if (_is_txt_edit) {
			_is_txt_edit = false;
			_txt_btn->SetSelected(false);
			_console_btn->SetSelected(true);
			win->Update();
		}
	}

	void BottomSection::OnConsoleClean(const ax::Button::Msg& msg)
	{
		_console->Clear();
	}

	void BottomSection::OnConsoleErrorUpdate(const ax::event::EmptyMsg& msg)
	{
		_console->GetWindow()->Show();
		_console_clean_btn->GetWindow()->Show();
		_txt_editor->GetWindow()->Hide();

		if (_is_txt_edit) {
			_is_txt_edit = false;
			_txt_btn->SetSelected(false);
			_console_btn->SetSelected(true);
			win->Update();
		}
	}

	void BottomSection::OnMouseLeftDoubleClick(const ax::Point& pos)
	{
		ax::Rect rect(win->dimension.GetRect());

		if (rect.position.y > 30) {
			// Drop up.
			int bottom_pos_y = rect.position.y + rect.size.h;
			int size_y = bottom_pos_y - 30;

			win->PushEvent(RESIZE, new ax::event::SimpleMsg<int>(0));
			ax::console::Print("Go up");
			win->dimension.SetRect(ax::Rect(rect.position.x, 30, rect.size.w, size_y));
			ax::console::Print("Go up 2");
		}
		else {
			// Drop down.
			win->PushEvent(RESIZE, new ax::event::SimpleMsg<int>(0));
			int y_pos = rect.position.y + rect.size.h - MINIMUM_HEIGHT;
			win->dimension.SetRect(ax::Rect(rect.position.x, y_pos, rect.size.w, MINIMUM_HEIGHT));
		}
	}

	void BottomSection::OnMouseLeave(const ax::Point& pos)
	{
		if (_has_resize_cursor) {
			ax::App::GetInstance().SetCursor(ax::core::Core::Cursor::NORMAL);
			_has_resize_cursor = false;
		}
	}

	void BottomSection::OnMouseMotion(const ax::Point& pos)
	{
		ax::Point mouse_pos = pos - win->dimension.GetAbsoluteRect().position;

		if (mouse_pos.y < 6) {
			if (_has_resize_cursor == false) {
				ax::App::GetInstance().SetCursor(ax::core::Core::Cursor::RESIZE_UP_DOWN);
				_has_resize_cursor = true;
			}
		}
		else if (_has_resize_cursor) {
			ax::App::GetInstance().SetCursor(ax::core::Core::Cursor::NORMAL);
			_has_resize_cursor = false;
		}
	}

	void BottomSection::OnMouseLeftDown(const ax::Point& pos)
	{
		ax::Point mouse_pos = pos - win->dimension.GetAbsoluteRect().position;

		if (mouse_pos.y < 6) {
			ax::App::GetInstance().SetCursor(ax::core::Core::Cursor::RESIZE_UP_DOWN);
			_delta_resize_click = pos;
			_resize_click_old_rect = win->dimension.GetRect();
			win->event.GrabMouse();
			//			ax::console::Print("Resize editor.");
		}
	}

	void BottomSection::OnMouseLeftDragging(const ax::Point& pos)
	{
		int delta_y = _delta_resize_click.y - pos.y;

		//		ax::console::Print("Resize editor delta :", delta_y);

		if (delta_y > 0) {
			ax::Rect rect(win->dimension.GetRect());
			rect.position.y -= delta_y;
			rect.size.h += delta_y;

			if (rect.position.y < 30) {
				//				ax::console::Print("Maximum height");
				int delta_pos_y = 30 - rect.position.y;
				rect.size.h -= delta_pos_y;
				rect.position.y = 30;
			}

			win->dimension.SetRect(rect);
		}
		else if (delta_y < 0) {
			ax::Rect rect(win->dimension.GetRect());
			rect.position.y -= delta_y;
			rect.size.h += delta_y;

			if (rect.size.h < MINIMUM_HEIGHT) {
				//				ax::console::Print("Minimum height");
				int delta_pos_y = MINIMUM_HEIGHT - rect.size.h;
				rect.size.h = MINIMUM_HEIGHT;
				rect.position.y -= delta_pos_y;
			}

			win->PushEvent(RESIZE, new ax::event::SimpleMsg<int>(0));
			win->dimension.SetRect(rect);
		}

		_delta_resize_click = pos;
	}

	void BottomSection::OnMouseLeftUp(const ax::Point& pos)
	{
		if (win->event.IsGrabbed()) {
			int delta_y = _delta_resize_click.y - pos.y;

			if (delta_y > 0) {
				ax::Rect rect(win->dimension.GetRect());
				rect.position.y -= delta_y;
				rect.size.h += delta_y;
				win->dimension.SetRect(rect);
			}
			else if (delta_y < 0) {
				ax::Rect rect(win->dimension.GetRect());
				rect.position.y -= delta_y;
				rect.size.h += delta_y;

				if (rect.size.h < MINIMUM_HEIGHT) {
					//					ax::console::Print("Minimum height");
					int delta_pos_y = MINIMUM_HEIGHT - rect.size.h;
					rect.size.h = MINIMUM_HEIGHT;
					rect.position.y -= delta_pos_y;
				}

				win->PushEvent(RESIZE, new ax::event::SimpleMsg<int>(0));
				win->dimension.SetRect(rect);
			}

			win->event.UnGrabMouse();

			if (_has_resize_cursor) {
				ax::App::GetInstance().SetCursor(ax::core::Core::Cursor::NORMAL);
				_has_resize_cursor = false;
			}
		}
	}

	void BottomSection::OnResize(const ax::Size& size)
	{
		//		_open_btn->GetWindow()->dimension.SetPosition(ax::Point(size.w - 50, 2));
		//
		//		ax::Point pos = _open_btn->GetWindow()->dimension.GetRect().GetNextPosRight(5);
		//		_save_btn->GetWindow()->dimension.SetPosition(pos);

		_txt_editor->GetWindow()->dimension.SetSize(ax::Size(size.w - 1, size.h - TOP_BAR_HEIGHT));
		_console->GetWindow()->dimension.SetSize(ax::Size(size.w - 1, size.h - TOP_BAR_HEIGHT));
	}

	void BottomSection::OnPaint(ax::GC gc)
	{
		const ax::Rect rect(win->dimension.GetDrawingRect());
		gc.SetColor(ax::Color(0.6));
		gc.DrawRectangle(rect);

		ax::Rect top_bar_rect(rect.position.x, rect.position.y, rect.size.w, TOP_BAR_HEIGHT);
		gc.SetColor(at::Skin::GetInstance()->data.txt_edit_bar);
		gc.DrawRectangle(top_bar_rect);

		gc.SetColor(at::Skin::GetInstance()->data.txt_edit_bar_contour);
		gc.DrawRectangleContour(top_bar_rect);
		gc.DrawRectangleContour(rect);

		gc.SetColor(at::Skin::GetInstance()->data.txt_edit_bar_label);
		if (_is_txt_edit) {
			gc.DrawStringAlignedCenter(_font, _file_path, top_bar_rect);
		}
		else {
			gc.DrawStringAlignedCenter(_font, "Console", top_bar_rect);
		}
	}
}
}
