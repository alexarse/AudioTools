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

#include "editor/TextEditorLogic.hpp"
#include <algorithm>
#include <fst/ascii.h>

/*******************************************************************************
 * eos::TextEditor::Logic.
 ******************************************************************************/
TextEditorLogic::TextEditorLogic()
	: _file_path("")
	, _cursor_pos(-1, -1)
	, _selection_rectangle{ false, ax::Point(-1, -1), ax::Point(-1, -1) }
{
}

bool TextEditorLogic::OpenFile(const std::string& file_path)
{
	_file_path = file_path;

	std::ifstream t(file_path);

	std::string file_str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

	// Remove all tab for string.
	ax::util::String::ReplaceCharWithString(file_str, '\t', "    ");

	_file_data = ax::util::String::Split(file_str, "\n");

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

std::vector<std::string>& TextEditorLogic::GetFileData()
{
	return _file_data;
}

const std::vector<std::string>& TextEditorLogic::GetFileData() const
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
	if (cursor_pos.y < _file_data.size()) {
		if (cursor_pos.x < _file_data[cursor_pos.y].size()) {
			_cursor_pos = cursor_pos;
		}
		else {
			_cursor_pos = ax::Point((int)_file_data[cursor_pos.y].size(), cursor_pos.y);
		}
	}
}

void TextEditorLogic::MoveCursorRight()
{
	if (_selection_rectangle.active) {
		_cursor_pos = _selection_rectangle.right;
		_selection_rectangle.active = false;
		return;
	}

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
	if (_selection_rectangle.active) {
		_cursor_pos = _selection_rectangle.left;
		_selection_rectangle.active = false;
		return;
	}

	int x_pos = _cursor_pos.x - 1;

	if (x_pos < 0) {
		x_pos = 0;
	}

	_cursor_pos.x = x_pos;
}

void TextEditorLogic::MoveCursorUp()
{
	if (_selection_rectangle.active) {
		_cursor_pos = _selection_rectangle.left;
		_selection_rectangle.active = false;
	}

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
	if (_selection_rectangle.active) {
		_cursor_pos = _selection_rectangle.right;
		_selection_rectangle.active = false;
	}

	int x_pos = _cursor_pos.x;
	int y_pos = _cursor_pos.y + 1;

	// Block cursor at last line.
	if (y_pos > (int)_file_data.size() - 1) {
		y_pos = (int)_file_data.size() - 1;

		// ax::console::Print("Logic :: Cursor last line");

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
	if (_selection_rectangle.active) {
		RemoveSelectedText();
	}

	const char TAB = 9;

	//	ax::console::Print("AddChar :", (int)c);

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
	if (_selection_rectangle.active) {
		RemoveSelectedText();
	}

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
	if (_selection_rectangle.active) {
		RemoveSelectedText();
		return;
	}

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
	if (_selection_rectangle.active) {
		RemoveSelectedText();
		return;
	}

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
		//        ax::console::Print("POP");
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

void TextEditorLogic::UnselectRectangle()
{
	_selection_rectangle.active = false;
}

void TextEditorLogic::SelectCurrentLine()
{
	_selection_rectangle.active = true;
	_selection_rectangle.left = ax::Point(0, _cursor_pos.y);
	_selection_rectangle.right = ax::Point(_file_data[_cursor_pos.y].size(), _cursor_pos.y);
}

void TextEditorLogic::SelectCurrentWord()
{
	const int line_length((int)_file_data[_cursor_pos.y].size());

	if (_cursor_pos.x >= line_length) {
		_selection_rectangle.active = false;
		return;
	}

	char cur_char = _file_data[_cursor_pos.y][_cursor_pos.x];

	// Clicking on special character.
	if (fst::ascii::is_special(cur_char) || fst::ascii::is_space_or_tab(cur_char)) {
		_selection_rectangle.left.x = _cursor_pos.x;

		// Block cursor position at the last char index + 1 to allow append at the end of line.
		if (_selection_rectangle.left.x > line_length) {
			_selection_rectangle.active = false;
			return;
		}

		_selection_rectangle.active = true;
		_selection_rectangle.right.x = _cursor_pos.x + 1;
		_selection_rectangle.left.y = _cursor_pos.y;
		_selection_rectangle.right.y = _cursor_pos.y;
		return;
	}

	// Find word left position.
	_selection_rectangle.left.x = 0;
	_selection_rectangle.left.y = _cursor_pos.y;

	for (int i = _cursor_pos.x; i > 0; i--) {
		char l_char = _file_data[_cursor_pos.y][i];
		if (fst::ascii::is_special(l_char) || fst::ascii::is_space_or_tab(l_char)) {
			_selection_rectangle.left.x = i;
			break;
		}
	}

	if (_selection_rectangle.left.x != 0 && _selection_rectangle.left.x != _cursor_pos.x) {
		_selection_rectangle.left.x++;
	}

	// Find word right position.
	_selection_rectangle.right.x = _file_data[_cursor_pos.y].size();
	_selection_rectangle.right.y = _cursor_pos.y;

	for (int i = _cursor_pos.x; i < _file_data[_cursor_pos.y].size(); i++) {
		char r_char = _file_data[_cursor_pos.y][i];
		if (fst::ascii::is_special(r_char) || fst::ascii::is_space_or_tab(r_char)) {
			_selection_rectangle.right.x = i;
			break;
		}
	}

	_selection_rectangle.active = true;
}

void TextEditorLogic::BeginSelectCursor()
{
	_selection_rectangle.active = true;
	_selection_rectangle.left = _selection_rectangle.right = _cursor_pos;
}

void TextEditorLogic::ContinueSelectCursor(const ax::Point& pos)
{
	AssignSelectionPos(pos);
}

void TextEditorLogic::EndSelectCursor(const ax::Point& pos)
{
	AssignSelectionPos(pos);

	if (_selection_rectangle.left == _selection_rectangle.right) {
		_selection_rectangle.active = false;
		return;
	}
}

void TextEditorLogic::RemoveSelectedText()
{
	if (!_selection_rectangle.active) {
		return;
	}

	// Deselect text.
	_selection_rectangle.active = false;

	// Set cursor position to left side of selected rectangle.
	_cursor_pos = _selection_rectangle.left;

	// One line selection.
	if (_selection_rectangle.left.y == _selection_rectangle.right.y) {
		std::string& line = _file_data[_selection_rectangle.left.y];
		line.erase(line.begin() + _selection_rectangle.left.x, line.begin() + _selection_rectangle.right.x);
		return;
	}

	// Two lines selection.
	if (_selection_rectangle.right.y - _selection_rectangle.left.y == 1) {
		// Delete first line completely.
		if (_selection_rectangle.left.x == 0) {
			// Remove line.
			_file_data.erase(_file_data.begin() + _selection_rectangle.left.y);
			_selection_rectangle.left = ax::Point(0, _selection_rectangle.left.y);

			// Remove second line.
			std::string& line = _file_data[_selection_rectangle.left.y];
			line.erase(
				line.begin() + _selection_rectangle.left.x, line.begin() + _selection_rectangle.right.x);
			return;
		}

		// Delete second line completely.
		if (_selection_rectangle.right.x == _file_data[_selection_rectangle.right.y].size()) {
			// Remove last line.
			_file_data.erase(_file_data.begin() + _selection_rectangle.right.y);

			// Set selection right side to last character of first line.
			_selection_rectangle.right
				= ax::Point(GetLineLength(_selection_rectangle.left.y), _selection_rectangle.left.y);

			// Crop first line.
			std::string& line = _file_data[_selection_rectangle.left.y];
			line.erase(
				line.begin() + _selection_rectangle.left.x, line.begin() + _selection_rectangle.right.x);

			return;
		}

		// Copy last selected line.
		std::string cropped_last_line = _file_data[_selection_rectangle.right.y];

		// Delete left side of second line and keep the right part.
		cropped_last_line.erase(
			cropped_last_line.begin(), cropped_last_line.begin() + _selection_rectangle.right.x);

		// Remove last line.
		_file_data.erase(_file_data.begin() + _selection_rectangle.right.y);

		// Set selection right side to last character of first line.
		_selection_rectangle.right
			= ax::Point(_file_data[_selection_rectangle.left.y].size(), _selection_rectangle.left.y);

		// Crop first line.
		std::string& first_line = _file_data[_selection_rectangle.left.y];
		first_line.erase(first_line.begin() + _selection_rectangle.left.x,
			first_line.begin() + _selection_rectangle.right.x);

		// Append the rest of the last line to first line.
		first_line.append(cropped_last_line);
		return;
	}

	// Multiple lines selection.
	// Copy last selected line.
	std::string cropped_last_line = _file_data[_selection_rectangle.right.y];

	// Delete left side of second line and keep the right part.
	cropped_last_line.erase(
		cropped_last_line.begin(), cropped_last_line.begin() + _selection_rectangle.right.x);

	// Remove middle lines.
	_file_data.erase(_file_data.begin() + _selection_rectangle.left.y + 1,
		_file_data.begin() + _selection_rectangle.right.y);

	// Set selection right side to last character of first line.
	_selection_rectangle.right
		= ax::Point(_file_data[_selection_rectangle.left.y].size(), _selection_rectangle.left.y);

	// Crop first line.
	std::string& first_line = _file_data[_selection_rectangle.left.y];
	first_line.erase(
		first_line.begin() + _selection_rectangle.left.x, first_line.begin() + _selection_rectangle.right.x);

	// Append the rest of the last line to first line.
	first_line.append(cropped_last_line);
}

void TextEditorLogic::AssignSelectionPos(const ax::Point& pos)
{
	ax::Point& left = _selection_rectangle.left;
	ax::Point& right = _selection_rectangle.right;

	if (pos.y > left.y) {
		right = pos;
		return;
	}

	if (pos.y == left.y && pos.x > left.x) {
		right = pos;
		return;
	}

	left = pos;
}

void TextEditorLogic::SelectAll()
{
	_selection_rectangle.active = true;
	_selection_rectangle.left = ax::Point(0, 0);
	_selection_rectangle.right = ax::Point(_file_data[_file_data.size() - 1].size(), _file_data.size());
}

std::string TextEditorLogic::GetSelectedContent() const
{
	if (!_selection_rectangle.active) {
		return "";
	}

	const ax::Point& left = _selection_rectangle.left;
	const ax::Point& right = _selection_rectangle.right;

	// One line.
	if (left.y == right.y) {
		return _file_data[left.y].substr(left.x, right.x);
	}

	// Multiple lines.
	std::string content = _file_data[left.y].substr(left.x) + "\n";

	// Two lines.
	if (right.y - left.y == 1) {
		return content + _file_data[right.y].substr(0, right.x);
	}

	for (int i = left.y + 1; i < right.y; i++) {
		content += _file_data[i] + "\n";
	}

	return content + _file_data[right.y].substr(0, right.x);
}
