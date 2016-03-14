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

	
	std::string GetScriptPath() const;

private:
	const int TOP_BAR_HEIGHT = 20;

	TextEditor* _txt_editor;
	std::string _file_path;
	ax::Font _font;
	ax::Button* _save_btn;
	axEVENT_DECLARATION(ax::Button::Msg, OnSaveButton);
	
	void OnResize(const ax::Size& size);
	void OnPaint(ax::GC gc);
};

#endif // __CODE_EDITOR_H__
