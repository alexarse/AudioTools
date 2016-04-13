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
 
#include "CodeEditor.h"
#include "atSkin.hpp"
#include <OpenAX/Button.h>
#include <algorithm>

CodeEditor::CodeEditor(const ax::Rect& rect)
	: _font(0)
{
	win = ax::Window::Create(rect);
	win->event.OnPaint = ax::WBind<ax::GC>(this, &CodeEditor::OnPaint);
	win->event.OnResize = ax::WBind<ax::Size>(this, &CodeEditor::OnResize);

	win->event.OnMouseLeave = ax::WBind<ax::Point>(this, &CodeEditor::OnMouseLeave);
	win->event.OnMouseMotion = ax::WBind<ax::Point>(this, &CodeEditor::OnMouseMotion);
	win->event.OnMouseLeftDown = ax::WBind<ax::Point>(this, &CodeEditor::OnMouseLeftDown);
	win->event.OnMouseLeftDragging = ax::WBind<ax::Point>(this, &CodeEditor::OnMouseLeftDragging);
	win->event.OnMouseLeftUp = ax::WBind<ax::Point>(this, &CodeEditor::OnMouseLeftUp);

	TextEditor::Info txt_info;
	txt_info.bg_color = ax::Color(1.0);
	txt_info.cursor_color = ax::Color(0.0);
	txt_info.line_number_bg_color = ax::Color(0.95);
	txt_info.line_number_color = ax::Color(0.4);
	txt_info.text_color = ax::Color(0.0);

	auto txt_editor = ax::shared<TextEditor>(
		ax::Rect(0, TOP_BAR_HEIGHT, rect.size.x, rect.size.y - TOP_BAR_HEIGHT),
		txt_info);
	_txt_editor = txt_editor.get();

	win->node.Add(txt_editor);
	
	// @todo Change this.
	_txt_editor->OpenFile("scripts/default.py");
	_file_path = _txt_editor->GetFilePath();

	ax::Button::Info btn_info;
	btn_info.normal = ax::Color(0.0, 0.0);
	btn_info.hover = ax::Color(0.0, 0.0);
	btn_info.clicking = ax::Color(0.0, 0.0);
	btn_info.selected = ax::Color(0.0, 0.0);
	btn_info.contour = ax::Color(0.0, 0.0);
	btn_info.font_color = ax::Color(0.0, 0.0);


	auto open_btn
	= ax::shared<ax::Button>(ax::Rect(rect.size.x - 50, 2, 17, 17),
							 GetOnOpenButton(), btn_info, "resources/folder.png", "",
							 ax::Button::Flags::SINGLE_IMG | ax::Button::Flags::IMG_RESIZE);
	_open_btn = open_btn.get();
	win->node.Add(open_btn);

	ax::Point pos = _open_btn->GetWindow()->dimension.GetRect().GetNextPosRight(5);

	auto save_btn
		= ax::shared<ax::Button>(ax::Rect(pos, ax::Size(17, 17)),
			GetOnSaveButton(), btn_info, "resources/save.png", "",
			ax::Button::Flags::SINGLE_IMG | ax::Button::Flags::IMG_RESIZE);
	_save_btn = save_btn.get();
	win->node.Add(save_btn);
}


bool CodeEditor::OpenFile(const std::string& path)
{
	bool err = _txt_editor->OpenFile(path);
	_file_path = path;
	win->Update();
	return err;
}

void CodeEditor::SaveFile(const std::string& path)
{
	_txt_editor->SaveFile(path);
}

std::string CodeEditor::GetScriptPath() const
{
	return _file_path;
}

void CodeEditor::OnOpenButton(const ax::Button::Msg& msg)
{
	ax::Print("CodeEditor.cpp -> CodeEditor::OnOpenButton doesn't do anything yet.");
//	_txt_editor->SaveFile(_txt_editor->GetFilePath());
//	win->PushEvent(10020, new ax::Event::StringMsg("Save"));
}

void CodeEditor::OnSaveButton(const ax::Button::Msg& msg)
{
	_txt_editor->SaveFile(_txt_editor->GetFilePath());
	win->PushEvent(10020, new ax::Event::StringMsg("Save"));
}



void CodeEditor::OnMouseLeave(const ax::Point& pos)
{
	if(_has_resize_cursor) {
		ax::App::GetInstance().SetCursor(0);
		_has_resize_cursor = false;
	}
}

void CodeEditor::OnMouseMotion(const ax::Point& pos)
{
	ax::Point mouse_pos = pos - win->dimension.GetAbsoluteRect().position;
	
	if(mouse_pos.y < 6) {
		if(_has_resize_cursor == false) {
			ax::App::GetInstance().SetCursor(1);
			_has_resize_cursor = true;
		}
	}
	else if(_has_resize_cursor) {
		ax::App::GetInstance().SetCursor(0);
		_has_resize_cursor = false;
	}
}

void CodeEditor::OnMouseLeftDown(const ax::Point& pos)
{
	ax::Point mouse_pos = pos - win->dimension.GetAbsoluteRect().position;
	
	if(mouse_pos.y < 6) {
		ax::App::GetInstance().SetCursor(1);
		_delta_resize_click = pos;
		_resize_click_old_rect = win->dimension.GetRect();
		win->event.GrabMouse();
		ax::Print("Resize editor.");
	}
}

void CodeEditor::OnMouseLeftDragging(const ax::Point& pos)
{
	int delta_y = _delta_resize_click.y - pos.y;
	
	ax::Print("Resize editor delta :", delta_y);
	
	if(delta_y > 0) {
		ax::Rect rect(win->dimension.GetRect());
		rect.position.y -= delta_y;
		rect.size.y += delta_y;
		
		
		if(rect.position.y < 30) {
			ax::Print("Maximum height");
			int delta_pos_y = 30 - rect.position.y;
			rect.size.y -= delta_pos_y;
			rect.position.y = 30;
		}
		
		win->dimension.SetRect(rect);
	}
	else if(delta_y < 0) {
		ax::Rect rect(win->dimension.GetRect());
		rect.position.y -= delta_y;
		rect.size.y += delta_y;
		
		if(rect.size.y < MINIMUM_HEIGHT) {
			ax::Print("Minimum height");
			int delta_pos_y = MINIMUM_HEIGHT - rect.size.y;
			rect.size.y = MINIMUM_HEIGHT;
			rect.position.y -= delta_pos_y;
		}
		
		win->PushEvent(RESIZE, new ax::Event::SimpleMsg<int>(0));
		win->dimension.SetRect(rect);
	}
	
	_delta_resize_click = pos;

	
}

void CodeEditor::OnMouseLeftUp(const ax::Point& pos)
{
	if(win->event.IsGrabbed()) {
		int delta_y = _delta_resize_click.y - pos.y;

		if(delta_y > 0) {
			ax::Rect rect(win->dimension.GetRect());
			rect.position.y -= delta_y;
			rect.size.y += delta_y;
			win->dimension.SetRect(rect);
		}
		else if(delta_y < 0) {
			ax::Rect rect(win->dimension.GetRect());
			rect.position.y -= delta_y;
			rect.size.y += delta_y;
			
			if(rect.size.y < MINIMUM_HEIGHT) {
				ax::Print("Minimum height");
				int delta_pos_y = MINIMUM_HEIGHT - rect.size.y;
				rect.size.y = MINIMUM_HEIGHT;
				rect.position.y -= delta_pos_y;
			}
			
			win->PushEvent(RESIZE, new ax::Event::SimpleMsg<int>(0));
			win->dimension.SetRect(rect);
		}
		
		win->event.UnGrabMouse();
		
		if(_has_resize_cursor) {
			ax::App::GetInstance().SetCursor(0);
			_has_resize_cursor = false;
		}
	}
}

void CodeEditor::OnResize(const ax::Size& size)
{
	_open_btn->GetWindow()->dimension.SetPosition(ax::Point(size.x - 50, 2));
	
	ax::Point pos = _open_btn->GetWindow()->dimension.GetRect().GetNextPosRight(5);
	_save_btn->GetWindow()->dimension.SetPosition(pos);
	_txt_editor->GetWindow()->dimension.SetSize(ax::Size(size.x, size.y - TOP_BAR_HEIGHT));
}

void CodeEditor::OnPaint(ax::GC gc)
{
	ax::Rect rect(win->dimension.GetDrawingRect());
	gc.SetColor(ax::Color(255, 0, 0));
	gc.DrawRectangle(rect);

	ax::Rect top_bar_rect(0, 0, rect.size.x, TOP_BAR_HEIGHT);
//	gc.SetColor(ax::Color(0.45));
	gc.SetColor(at::Skin::GetInstance()->data.txt_edit_bar);
	gc.DrawRectangle(top_bar_rect);
	
	gc.SetColor(at::Skin::GetInstance()->data.txt_edit_bar_contour);
	gc.DrawRectangleContour(top_bar_rect);
	gc.DrawRectangleContour(rect);
	
//	gc.SetColor(ax::Color(1.0));
	gc.SetColor(at::Skin::GetInstance()->data.txt_edit_bar_label);

	gc.DrawString(_font, _file_path, ax::Point(5, 2));
}