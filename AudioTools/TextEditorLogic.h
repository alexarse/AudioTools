#ifndef __TEXT_EDITOR_LOGIC_H__
#define __TEXT_EDITOR_LOGIC_H__

#include "axLib.h"
#include "axScrollBar.h"
#include "axTimer.h"

#include <fstream>
#include <set>


class TextEditorLogic {
public:
	TextEditorLogic();

	bool OpenFile(const std::string& file_path);

	bool SaveFile(const std::string& file_path);

	ax::StringVector& GetFileData();

	const ax::StringVector& GetFileData() const;

	std::string GetFilePath() const;

	ax::Point GetCursorPosition() const;
	
	void SetCursorPosition(const ax::Point& cursor_pos);

	void MoveCursorRight();

	void MoveCursorLeft();

	void MoveCursorUp();

	void MoveCursorDown();

	void AddChar(const char& c);

	void Enter();

	void Delete();

	void BackSpace();

private:
	std::string _file_path;
	ax::Point _cursor_pos;
	ax::StringVector _file_data;
};

#endif // __TEXT_EDITOR_LOGIC_H__
