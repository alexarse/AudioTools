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
 
#include "TextEditorLogic.h"
#include <algorithm>

/*******************************************************************************
 * eos::TextEditor::Logic.
 ******************************************************************************/
TextEditorLogic::TextEditorLogic()
	: _file_path("")
	, _cursor_pos(-1, -1)
{
}

bool TextEditorLogic::OpenFile(const std::string& file_path)
{
	_file_path = file_path;

	std::ifstream t(file_path);

	std::string file_str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

	// Remove all tab for string.
	ax::Utils::String::ReplaceCharWithString(file_str, '\t', "    ");

	_file_data = ax::Utils::String::Split(file_str, "\n");

	_cursor_pos = ax::Point(0, 0);

	return true;
}

bool TextEditorLogic::SaveFile(const std::string& file_path)
{
	_file_path = file_path;

	std::ofstream out(file_path);

	for (auto& n : _file_data) {
		out << n << std::endl;
	}

	out.close();

	return true;
}

ax::StringVector& TextEditorLogic::GetFileData()
{
	return _file_data;
}

const ax::StringVector& TextEditorLogic::GetFileData() const
{
	return _file_data;
}

std::string TextEditorLogic::GetFilePath() const
{
	return _file_path;
}

ax::Point TextEditorLogic::GetCursorPosition() const
{
	return _cursor_pos;
}

void TextEditorLogic::SetCursorPosition(const ax::Point& cursor_pos)
{
	/// @todo Check if position is valid.
	
	if(cursor_pos.y < _file_data.size()) {
		if(cursor_pos.x < _file_data[cursor_pos.y].size()) {
			_cursor_pos = cursor_pos;
		}
		else {
			_cursor_pos = ax::Point(_file_data[cursor_pos.y].size(), cursor_pos.y);
		}
	}
	
}

void TextEditorLogic::MoveCursorRight()
{
	int x_pos = _cursor_pos.x + 1;

	// Block cursor position at the last char index + 1
	// to allow append at the end of line.
	if (x_pos > _file_data[_cursor_pos.y].size()) {
		x_pos = (int)_file_data[_cursor_pos.y].size();
	}

	_cursor_pos.x = x_pos;
}

void TextEditorLogic::MoveCursorLeft()
{
	int x_pos = _cursor_pos.x - 1;

	if (x_pos < 0) {
		x_pos = 0;
	}

	_cursor_pos.x = x_pos;
}

void TextEditorLogic::MoveCursorUp()
{
	int x_pos = _cursor_pos.x;
	int y_pos = _cursor_pos.y - 1;

	// Block cursor at first line.
	if (y_pos < 0) {
		y_pos = 0;
	}

	// Block x cursor position at last char of line + 1.
	if (x_pos > _file_data[y_pos].size()) {
		x_pos = (int)_file_data[y_pos].size();
	}

	_cursor_pos.x = x_pos;
	_cursor_pos.y = y_pos;
}

void TextEditorLogic::MoveCursorDown()
{
	int x_pos = _cursor_pos.x;
	int y_pos = _cursor_pos.y + 1;

	// Block cursor at last line.
	if (y_pos > (int)_file_data.size() - 1) {
		y_pos = (int)_file_data.size() - 1;

		// ax::Print("Logic :: Cursor last line");

		// Set cursor at the last char of last line.
		x_pos = (int)_file_data[y_pos].size();

		_cursor_pos.x = x_pos;
		_cursor_pos.y = y_pos;
		return;
	}

	// Block x cursor position at last char of line + 1.
	if (x_pos > _file_data[y_pos].size()) {
		x_pos = (int)_file_data[y_pos].size();
	}

	_cursor_pos.x = x_pos;
	_cursor_pos.y = y_pos;
}

void TextEditorLogic::AddChar(const char& c)
{
	const char TAB = 9;

	ax::Print("AddChar :", (int)c);
	
	// Insert char.
	if (c == TAB) {
		_file_data[_cursor_pos.y].insert(_cursor_pos.x, std::string("    "));
		_cursor_pos.x += 4;
		return;
	}
	
	// Append at the end of the line.
	if (_cursor_pos.x == _file_data[_cursor_pos.y].size()) {

		_file_data[_cursor_pos.y].push_back(c);
		_cursor_pos.x++;
		return;
	}

	_file_data[_cursor_pos.y].insert(_cursor_pos.x, 1, c);
	_cursor_pos.x++;
}

void TextEditorLogic::Enter()
{
	// Append at the end of the line.
	if (_cursor_pos.x == _file_data[_cursor_pos.y].size()) {
		_file_data.insert(_file_data.begin() + _cursor_pos.y + 1, std::string(""));

		_cursor_pos.x = 0;
		_cursor_pos.y++;
		return;
	}

	// Beginning of not empty line.
	if (_cursor_pos.x == 0) {
		_file_data.insert(_file_data.begin() + _cursor_pos.y, std::string(""));

		_cursor_pos.y++;
		return;
	}

	// Enter in middle of line.
	std::string old_str = _file_data[_cursor_pos.y].substr(0, _cursor_pos.x);
	std::string new_str = _file_data[_cursor_pos.y].substr(_cursor_pos.x);

	_file_data[_cursor_pos.y] = old_str;
	_file_data.insert(_file_data.begin() + _cursor_pos.y + 1, new_str);

	_cursor_pos.x = 0;
	_cursor_pos.y++;
}

void TextEditorLogic::Delete()
{
	// Nothing to do when delete on last char of last line.
	if (_cursor_pos.x == _file_data[_cursor_pos.y].size() && _cursor_pos.y == _file_data.size() - 1) {
		return;
	}

	// Delete at the end of line.
	if (_cursor_pos.x == _file_data[_cursor_pos.y].size()) {
		std::string next_line = _file_data[_cursor_pos.y + 1];
		_file_data[_cursor_pos.y] += next_line;

		// Delete old line.
		_file_data.erase(_file_data.begin() + _cursor_pos.y + 1);
		return;
	}

	// Remove char in middle of line.
	_file_data[_cursor_pos.y].erase(_file_data[_cursor_pos.y].begin() + _cursor_pos.x);
}

void TextEditorLogic::BackSpace()
{
	// Nothing to do when backspace at the begenning of first line.
	if (_cursor_pos.x == 0 && _cursor_pos.y == 0) {
		return;
	}

	// Backspace at the beginning of line.
	if (_cursor_pos.x == 0) {
		std::string line = _file_data[_cursor_pos.y];
		int end_line_pos = (int)_file_data[_cursor_pos.y - 1].size();

		// Append line to the line above.
		_file_data[_cursor_pos.y - 1] += line;

		// Delete old line.
		_file_data.erase(_file_data.begin() + _cursor_pos.y);

		_cursor_pos.x = end_line_pos;
		_cursor_pos.y--;
		return;
	}

	// Remove at the end of the line.
	if (_cursor_pos.x == _file_data[_cursor_pos.y].size()) {
		//        ax::Print("POP");
		_file_data[_cursor_pos.y].pop_back();
		_cursor_pos.x--;

		if (_cursor_pos.x < 0) {
			_cursor_pos.x = 0;
		}
		return;
	}

	// Remove char in middle of line.
	_file_data[_cursor_pos.y].erase(_file_data[_cursor_pos.y].begin() + _cursor_pos.x - 1);
	_cursor_pos.x--;
}