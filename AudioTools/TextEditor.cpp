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

#include "TextEditor.h"
#include <algorithm>

/*******************************************************************************
 * eos::TextEditor.
 ******************************************************************************/
TextEditor::TextEditor(const ax::Rect& rect, const TextEditor::Info& info)
	: _font("fonts/VeraMono.ttf")
	, _line_num_font("fonts/DejaVuSansMono.ttf")
	, _line_height(15)
	, _file_start_index(0)
	, _info(info)
{
	_line_num_font.SetFontSize(10);
	_n_line_shown = (rect.size.y - 1) / _line_height;

	win = ax::Window::Create(rect);
	win->event.OnResize = ax::WBind<ax::Size>(this, &TextEditor::OnResize);

	_scrollPanel = ax::Window::Create(ax::Rect(0, 0, rect.size.x, rect.size.y));

	_scrollPanel->property.AddProperty("BlockDrawing");

	_scrollPanel->event.OnPaint = ax::WBind<ax::GC>(this, &TextEditor::OnPaint);

	// Mouse events.
	_scrollPanel->event.OnMouseEnter = ax::WBind<ax::Point>(this, &TextEditor::OnMouseEnter);
	_scrollPanel->event.OnMouseEnterChild = ax::WBind<ax::Size>(this, &TextEditor::OnMouseEnterChild);
	_scrollPanel->event.OnMouseLeave = ax::WBind<ax::Size>(this, &TextEditor::OnMouseLeave);
	_scrollPanel->event.OnMouseLeaveChild = ax::WBind<ax::Size>(this, &TextEditor::OnMouseLeaveChild);
	_scrollPanel->event.OnMouseLeftDown = ax::WBind<ax::Point>(this, &TextEditor::OnMouseLeftDown);
	_scrollPanel->event.OnMouseLeftUp = ax::WBind<ax::Point>(this, &TextEditor::OnMouseLeftUp);
	_scrollPanel->event.OnScrollWheel = ax::WBind<ax::Point>(this, &TextEditor::OnScrollWheel);

	// Keyboard events.
	_scrollPanel->event.OnLeftArrowDown = ax::WBind<char>(this, &TextEditor::OnLeftArrowDown);
	_scrollPanel->event.OnRightArrowDown = ax::WBind<char>(this, &TextEditor::OnRightArrowDown);
	_scrollPanel->event.OnUpArrowDown = ax::WBind<char>(this, &TextEditor::OnUpArrowDown);
	_scrollPanel->event.OnDownArrowDown = ax::WBind<char>(this, &TextEditor::OnDownArrowDown);
	_scrollPanel->event.OnKeyDown = ax::WBind<char>(this, &TextEditor::OnKeyDown);
	_scrollPanel->event.OnEnterDown = ax::WBind<char>(this, &TextEditor::OnEnterDown);
	_scrollPanel->event.OnKeyDeleteDown = ax::WBind<char>(this, &TextEditor::OnKeyDeleteDown);
	_scrollPanel->event.OnBackSpaceDown = ax::WBind<char>(this, &TextEditor::OnBackSpaceDown);

	win->node.Add(ax::Window::Ptr(_scrollPanel));

	ax::ScrollBar::Info sInfo;
	sInfo.normal = ax::Color(0.60);
	sInfo.hover = ax::Color(0.65);
	sInfo.clicking = ax::Color(0.62);
	sInfo.slider_contour = ax::Color(0.0, 0.2);
	sInfo.contour = ax::Color(0.0, 0.0);
	sInfo.bg_top = ax::Color(0.8, 0.2);
	sInfo.bg_bottom = ax::Color(0.82, 0.2);

	ax::ScrollBar::Events scrollEvents;
	scrollEvents.value_change = GetOnScroll();

	auto scroll_bar
		= ax::shared<ax::ScrollBar>(ax::Rect(rect.size.x - 9, 0, 10, rect.size.y), scrollEvents, sInfo);

	_scrollBar = scroll_bar.get();

	win->node.Add(scroll_bar);

	// Scrollbar is use without window handle, it behave just like a slider.
	int h_size = (int)_logic.GetFileData().size() * _line_height;
	_scrollBar->UpdateWindowSize(ax::Size(rect.size.x, h_size));

	_number_cpp.insert("0");

	_key_words_cpp.insert("class");
	_key_words_cpp.insert("const");
	_key_words_cpp.insert("return");
	_key_words_cpp.insert("void");
	_key_words_cpp.insert("virtual");
	_key_words_cpp.insert("private");
	_key_words_cpp.insert("public");
	_key_words_cpp.insert("protected");
	_key_words_cpp.insert("virtual");
	_key_words_cpp.insert("if");
	_key_words_cpp.insert("else");
	_key_words_cpp.insert("while");
	_key_words_cpp.insert("float");
	_key_words_cpp.insert("double");
	_key_words_cpp.insert("unsigned");
	_key_words_cpp.insert("char");
	_key_words_cpp.insert("for");
	_key_words_cpp.insert("namespace");
	_key_words_cpp.insert("new");
	_key_words_cpp.insert("delete");
}

void TextEditor::SaveFile(const std::string& path)
{
	_logic.SaveFile(path);
}

void TextEditor::SaveCurrentFile()
{
	_logic.SaveFile(_logic.GetFilePath());
}

bool TextEditor::OpenFile(const std::string& path)
{
	bool err = _logic.OpenFile(path);

	ax::Rect rect = win->dimension.GetRect();

	// Scrollbar is use without window handle, it behave just like a slider.
	int h_size = (int)_logic.GetFileData().size() * _line_height;
	_scrollBar->UpdateWindowSize(ax::Size(rect.size.x, h_size));
	win->Update();
	_scrollPanel->Update();

	return err;
}

std::string TextEditor::GetStringContent() const
{
	std::string content;
	const ax::StringVector& data = _logic.GetFileData();

	for (auto& n : data) {
		content += (n + "\n");
	}

	return content;
}

std::string TextEditor::GetFilePath() const
{
	return _logic.GetFilePath();
}

ax::Point TextEditor::FileCursorPosToNextPosIndex()
{
	ax::Point file_cursor(_logic.GetCursorPosition());

	// Cursor is above shown text.
	if (file_cursor.y < _file_start_index) {
		return ax::Point(-1, -1);
	}
	// Cursor is bellow shown text.
	if (file_cursor.y > _file_start_index + _n_line_shown - 1) {
		return ax::Point(-1, -1);
	}

	return ax::Point(file_cursor.x, file_cursor.y - _file_start_index);
}

void TextEditor::MoveToCursorPosition()
{
	ax::Point cur_pos(_logic.GetCursorPosition());

	// Cursor is bellow last shown line.
	if (cur_pos.y > _file_start_index + _n_line_shown - 1) {
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		// Possible problem when file size is smaller than _n_line_shown.
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		_file_start_index = cur_pos.y - _n_line_shown + 1;
	}
	else if (cur_pos.y < _file_start_index) {
		_file_start_index = cur_pos.y;
	}

	// Move scroll bar.
	int diff = (int)_logic.GetFileData().size() - _n_line_shown;
	double scroll_ratio = _file_start_index / double(diff);
	_scrollBar->SetZeroToOneValue(scroll_ratio);
}

void TextEditor::OnResize(const ax::Size& size)
{
	const int file_size((int)_logic.GetFileData().size());
//	double r = (_file_start_index / double(file_size));
	
	_n_line_shown = size.y / _line_height;
	_scrollPanel->dimension.SetSize(size);

	_scrollBar->GetWindow()->dimension.SetRect(ax::Rect(size.x - 9, 0, 10, size.y));

	int h_size = file_size * _line_height;

	if (h_size < size.y) {
		h_size = size.y;
		ax::Print("bb");
		_file_start_index = 0;
		_scrollBar->UpdateWindowSize(ax::Size(size.x, size.y));
	}
	else {
		_scrollBar->UpdateWindowSize(ax::Size(size.x, h_size));
	}

	

	// Move scroll bar.
//	int diff = file_size - _n_line_shown;
//	double scroll_ratio = _file_start_index / double(diff);
//	MoveToCursorPosition();
//	_scrollBar->SetZeroToOneValue(r);
}

void TextEditor::OnScroll(const ax::ScrollBar::Msg& msg)
{
	int diff = (int)_logic.GetFileData().size() - _n_line_shown;
	diff = ax::Utils::Clamp<int>(diff, 0, (int)_logic.GetFileData().size());

	double scroll_ratio = _scrollBar->GetZeroToOneValue();
	_file_start_index = scroll_ratio * diff;
	_scrollPanel->Update();
}

void TextEditor::OnMouseEnter(const ax::Point& mouse)
{
	_scrollPanel->event.GrabKey();
	_scrollPanel->event.GrabScroll();
	_scrollPanel->Update();
}

void TextEditor::OnMouseLeave(const ax::Point& pos)
{
	if (!win->dimension.GetAbsoluteRect().IsPointInside(pos)) {
		_scrollPanel->event.UnGrabScroll();
	}

	_scrollPanel->event.UnGrabKey();
	_scrollPanel->Update();
}

void TextEditor::OnMouseEnterChild(const ax::Point& pos)
{
	_scrollPanel->event.GrabScroll();
}

void TextEditor::OnMouseLeaveChild(const ax::Point& pos)
{
	if (!win->dimension.GetAbsoluteRect().IsPointInside(pos)) {
		_scrollPanel->event.UnGrabScroll();
	}
}

void TextEditor::OnLeftArrowDown(const char& key)
{
	_logic.MoveCursorLeft();
	MoveToCursorPosition();
	_scrollPanel->Update();
}

void TextEditor::OnRightArrowDown(const char& key)
{
	_logic.MoveCursorRight();
	MoveToCursorPosition();
	_scrollPanel->Update();
}

void TextEditor::OnUpArrowDown(const char& key)
{
	_logic.MoveCursorUp();
	MoveToCursorPosition();
	_scrollPanel->Update();
}

void TextEditor::OnDownArrowDown(const char& key)
{
	_logic.MoveCursorDown();
	MoveToCursorPosition();
	_scrollPanel->Update();
}

void TextEditor::OnKeyDown(const char& key)
{
	// If command is down.
	if (ax::App::GetInstance().GetWindowManager()->IsCmdDown()) {

		if (key == 'v' || key == 'V') {
			std::string content = ax::App::GetInstance().GetPasteboardConent();
			ax::Utils::String::ReplaceCharWithString(content, '\t', "    ");

			if (!content.empty()) {
				ax::StringVector& file_data = _logic.GetFileData();
				ax::Point cur_pos(_logic.GetCursorPosition());

				ax::StringVector paste_content(ax::Utils::String::Split(content, "\n"));

				file_data[cur_pos.y].insert(cur_pos.x, paste_content[0]);

				for (int i = 1; i < paste_content.size(); i++) {
					file_data.insert(file_data.begin() + cur_pos.y + i, paste_content[i]);
				}

				_scrollPanel->Update();
				//				ax::Print("Pasteboard content :", content);
			}
		}
	}
	else {
		_logic.AddChar(key);
		MoveToCursorPosition();
		_scrollPanel->Update();
	}
}

void TextEditor::OnEnterDown(const char& key)
{
	_logic.Enter();

	int h_size = (int)_logic.GetFileData().size() * _line_height;
	_scrollBar->UpdateWindowSize(ax::Size(_scrollPanel->dimension.GetRect().size.x, h_size));
	MoveToCursorPosition();

	_scrollPanel->Update();
}

void TextEditor::OnBackSpaceDown(const char& key)
{
	_logic.BackSpace();
	int h_size = (int)_logic.GetFileData().size() * _line_height;
	_scrollBar->UpdateWindowSize(ax::Size(_scrollPanel->dimension.GetRect().size.x, h_size));
	MoveToCursorPosition();

	_scrollPanel->Update();
}

void TextEditor::OnKeyDeleteDown(const char& key)
{
	_logic.Delete();
	int h_size = (int)_logic.GetFileData().size() * _line_height;
	_scrollBar->UpdateWindowSize(ax::Size(_scrollPanel->dimension.GetRect().size.x, h_size));
	MoveToCursorPosition();

	_scrollPanel->Update();
}

void TextEditor::OnScrollWheel(const ax::Point& delta)
{
	ax::Size size = _scrollPanel->dimension.GetShownRect().size;
	double scroll_value = (2.0 * delta.y) / double(size.y) + _scrollBar->GetZeroToOneValue();
	scroll_value = ax::Utils::Clamp(scroll_value, 0.0, 1.0);
	_scrollBar->SetZeroToOneValue(scroll_value);
}

void TextEditor::OnMouseLeftDown(const ax::Point& pos)
{
	const ax::Point mouse_pos = pos - win->dimension.GetAbsoluteRect().position;

	// Line number selection.
	if(mouse_pos.x < 25) {
		return;
	}

	// Find new cursor line.
	int line_index = _file_start_index + mouse_pos.y / _line_height;

	//	ax::Print(line_index);
	const ax::StringVector& data = _logic.GetFileData();

	if (line_index >= data.size()) {
		ax::Print("go to last char");
		_logic.SetCursorPosition(ax::Point((int)data[data.size() - 1].size(), (int)data.size() - 1));
		_scrollPanel->Update();
		return;
	}

//	int actual_line_index = line_index + _file_start_index;

	// Find x cursor position.
	const std::string& text = data[line_index];

	std::vector<int> next_vec;
	next_vec.reserve(text.size() + 1);

	ax::Point line_pos(25 + 4, 0);

	if(!_font) {
		return;
	}
	
	int x = line_pos.x;
	next_vec.push_back(x);
	
	// For all char in line.
	for (int i = 0; i < text.size(); i++) {
		_font.SetChar(text[i]);
		next_vec.push_back(_font.GetNextPosition());
	}
	
	// Line is empty.
	// Set cursor to begnning of line.
	if (next_vec.size() <= 1) {
		_logic.SetCursorPosition(ax::Point(0, line_index));
		_scrollPanel->Update();
		return;
	}

	// Find char index in line.
	int cursor_index_x = -1;
	int sum_size_x = 0;
	
	for (int i = 0; i < next_vec.size() - 1; i++) {
		if (mouse_pos.x >= sum_size_x + next_vec[i]
			&& mouse_pos.x < sum_size_x + next_vec[i] + next_vec[i + 1]) {
			cursor_index_x = i;
			break;
		}
		sum_size_x += next_vec[i];
	}

	// Char is not found.
	// Goto to last char for line.
	if (cursor_index_x == -1) {
		_logic.SetCursorPosition(ax::Point((int)text.size(), line_index));
		_scrollPanel->Update();
		return;
	}
	
	if (cursor_index_x < _logic.GetFileData()[line_index].size()) {
		_logic.SetCursorPosition(ax::Point(cursor_index_x, line_index));
		_scrollPanel->Update();
	}
}

void TextEditor::OnMouseLeftUp(const ax::Point& mouse_pos)
{
	//        ax::Point pos = mouse_pos - GetAbsoluteRect().position;
	//        ax::Size w_size(GetSize());
	//
	//        int n_shown = w_size.y / 15;
	//
	//        _selected_index = (pos.y / (double)w_size.y) * n_shown;
	//
	//        int index = _file_start_index + _selected_index;
	//
	//        const std::vector<ax::os::File>& content = _dir.GetContent();
	//
	//        if(index < content.size())
	//        {
	//            if(content[index].type == ax::os::File::FOLDER)
	//            {
	//                _dir.Goto(_dir.GetPath() + _dir.GetContent()[index].name +
	//                "/");
	//                ResetParams();
	//            }
	//        }
	//
	//        Update();
}

bool is_special(const char& x)
{
	return x == '(' || x == ')' || x == ':' || x == '[' || x == ']' || x == ',' || x == ';' || x == '{'
		|| x == '}' || x == '=';
}

// bool is_number_char();

std::string RemoveSpecialChar(const std::string& str)
{
	std::string r = str;
	r.erase(std::remove_if(r.begin(), r.end(), [](char x) { return is_special(x); }), r.end());

	return r;
}

// bool is_number(const std::string& s)
//{
//    return !s.empty() && std::find_if(s.begin(), s.end(),
//                                      [](char c)
//    {
//        return !std::isdigit(c);
//    }) == s.end();
//}

bool is_number(const std::string& str)
{
	char* p;
	strtod(str.c_str(), &p); //, 10);
	return *p == 0;
}

void TextEditor::OnPaint(ax::GC gc)
{
	ax::Rect rect(_scrollPanel->dimension.GetDrawingRect());

	gc.SetColor(_info.bg_color);
	gc.DrawRectangle(rect);
	gc.DrawRectangleContour(rect);

	// Draw line number background.
	gc.SetColor(_info.line_number_bg_color);
	gc.DrawRectangle(ax::Rect(0, 0, 25, rect.size.y));

	ax::Point num_pos(4, 2);

	gc.SetColor(_info.line_number_color);

	// Draw line number.
	for (int i = 0; i < _n_line_shown; i++) {
		int num = i + _file_start_index;
		std::string num_str = std::to_string(num);

		if (num < 10) {
			num_str = "  " + num_str;
		}
		else if (num < 100) {
			num_str = " " + num_str;
		}

		gc.DrawString(_line_num_font, num_str, num_pos);

		num_pos += ax::Point(0, 15);
	}

	// Text initial position.
	ax::Point line_pos(25 + 4, 0);

	_next_pos_data.clear();

	const ax::StringVector& data = _logic.GetFileData();

	// For all shown line in text.
	for (int i = 0, k = _file_start_index; k < data.size() && i < _n_line_shown; i++, k++) {
		// Line.
		const std::string& text = data[k];
		std::vector<int> next_vec(text.size() + 1);

		// Draw string.
		if (_font) {
			int x = line_pos.x;

			next_vec[0] = x;

			// Set text color.
			gc.SetColor(_info.text_color);

			bool line_comment = false;
			bool string_literal = false;
			bool last_char_str_literal = false;

			// For all char in line.
			for (int i = 0; i < text.size(); i++) {

				if (text[i] == '#') {
					line_comment = true;
				}
				else if (text[i] == '\"') {
					string_literal = !string_literal;
					last_char_str_literal = true;
				}

				if (!line_comment) {
					if (string_literal || last_char_str_literal) {
						gc.SetColor(ax::Color(180, 10, 10));
					}
					else {
						if (text[i] == '=') {
							gc.SetColor(ax::Color(222, 69, 199));
						}
						else if (text[i] == ':') {
							gc.SetColor(ax::Color(0, 0, 255));
						}
						else if (text[i] == ';') {
							gc.SetColor(ax::Color(0, 0, 255));
						}
						else if (text[i] == '(' || text[i] == ')') {
							gc.SetColor(ax::Color(222, 69, 199));
						}
						else if (text[i] == '.') {
							gc.SetColor(ax::Color(0, 0, 255));
						}
						else {
							gc.SetColor(_info.text_color);
						}
					}

					last_char_str_literal = false;
				}
				else {
					gc.SetColor(ax::Color(0.6));
				}

				x = gc.DrawChar(_font, text[i], ax::Point(x, line_pos.y)).x;
				next_vec[i + 1] = x;
			}
		}

		_next_pos_data.push_back(next_vec);
		line_pos += ax::Point(0, 15);
	}

	// Line cursor.
	if (_scrollPanel->event.IsKeyGrab()) {
		ax::Point cursor_index = FileCursorPosToNextPosIndex();

		if (cursor_index.x != -1 && cursor_index.y != -1) {
			int x = _next_pos_data[cursor_index.y][cursor_index.x];
			int y = cursor_index.y * _line_height;

			gc.SetColor(_info.cursor_color);
			gc.DrawLine(ax::Point(x, y), ax::Point(x, y + _line_height));
		}
	}
}