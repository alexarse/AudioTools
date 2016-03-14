#include "CodeEditor.h"
#include "axButton.h"
#include <algorithm>

CodeEditor::CodeEditor(const ax::Rect& rect)
	: _font(0)
{
	win = ax::Window::Create(rect);
	win->event.OnPaint = ax::WBind<ax::GC>(this, &CodeEditor::OnPaint);
	win->event.OnResize = ax::WBind<ax::Size>(this, &CodeEditor::OnResize);

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
	gc.SetColor(ax::Color(0.45));
	gc.DrawRectangle(top_bar_rect);
	gc.DrawRectangleContour(top_bar_rect);
	gc.DrawRectangleContour(rect);
	
	gc.SetColor(ax::Color(1.0));
	gc.DrawString(_font, _file_path, ax::Point(5, 2));
}