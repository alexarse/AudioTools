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
	
	_txt_editor->OpenFile("scripts/simple_saw.py");
	_file_path = _txt_editor->GetFilePath();
//	"scripts/simple_saw.py"

	ax::Button::Info btn_info;
	btn_info.normal = ax::Color(0.0, 0.0);
	btn_info.hover = ax::Color(0.0, 0.0);
	btn_info.clicking = ax::Color(0.0, 0.0);
	btn_info.selected = ax::Color(0.0, 0.0);
	btn_info.contour = ax::Color(0.0, 0.0);
	btn_info.font_color = ax::Color(0.0, 0.0);

	auto save_btn
		= ax::shared<ax::Button>(ax::Rect(rect.size.x - 25, 2, 17, 17),
			GetOnSaveButton(), btn_info, "resources/save20.png", "",
			ax::Button::Flags::SINGLE_IMG | ax::Button::Flags::IMG_RESIZE);
	_save_btn = save_btn.get();
	win->node.Add(save_btn);
}

void CodeEditor::OnSaveButton(const ax::Button::Msg& msg)
{
	_txt_editor->SaveFile(_txt_editor->GetFilePath());
	win->PushEvent(10020, new ax::Event::StringMsg("Save"));
}

void CodeEditor::OnResize(const ax::Size& size)
{
	_save_btn->GetWindow()->dimension.SetPosition(ax::Point(size.x - 25, 2));
	_txt_editor->GetWindow()->dimension.SetSize(ax::Size(size.x, size.y - TOP_BAR_HEIGHT));
}

void CodeEditor::OnPaint(ax::GC gc)
{
	ax::Rect rect(win->dimension.GetDrawingRect());
	gc.SetColor(ax::Color(255, 0, 0));
	gc.DrawRectangle(rect);

	ax::Rect top_bar_rect(0, 0, rect.size.x, TOP_BAR_HEIGHT);
	gc.SetColor(ax::Color(180, 180, 180));
	gc.DrawRectangle(top_bar_rect);
	gc.DrawRectangleContour(top_bar_rect);
	gc.DrawRectangleContour(rect);
	
	gc.SetColor(ax::Color(0.0));
	gc.DrawString(_font, _file_path, ax::Point(5, 2));
}