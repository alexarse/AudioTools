#ifndef __CODE_EDITOR_H__
#define __CODE_EDITOR_H__

#include "axLib.h"
#include "axScrollBar.h"
#include "axTimer.h"

#include "TextEditor.h"
#include "axButton.h"

#include <fstream>
#include <set>

class CodeEditor : public ax::Window::Backbone {
public:
	CodeEditor(const ax::Rect& rect);
	
	bool OpenFile(const std::string& path);
	void SaveFile(const std::string& path);

	enum : ax::Event::Id
	{
		RESIZE
	};
	
	std::string GetScriptPath() const;
	
	static const int MINIMUM_HEIGHT = 200;

private:
	const int TOP_BAR_HEIGHT = 20;

	TextEditor* _txt_editor;
	std::string _file_path;
	ax::Font _font;
	ax::Button* _open_btn;
	ax::Button* _save_btn;
	ax::Point _delta_resize_click;
	ax::Rect _resize_click_old_rect;
	bool _has_resize_cursor = false;
	
	axEVENT_DECLARATION(ax::Button::Msg, OnOpenButton);
	axEVENT_DECLARATION(ax::Button::Msg, OnSaveButton);
	
	
	void OnMouseLeave(const ax::Point& pos);
	void OnMouseMotion(const ax::Point& pos);
	void OnMouseLeftDown(const ax::Point& pos);
	void OnMouseLeftDragging(const ax::Point& pos);
	void OnMouseLeftUp(const ax::Point& pos);
	
	void OnResize(const ax::Size& size);
	void OnPaint(ax::GC gc);
};

#endif // __CODE_EDITOR_H__
