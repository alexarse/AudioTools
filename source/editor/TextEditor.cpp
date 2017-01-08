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

#include "editor/TextEditor.hpp"
#include <algorithm>

/*******************************************************************************
 * eos::TextEditor.
 ******************************************************************************/
TextEditor::TextEditor(const ax::Rect& rect, const TextEditor::Info& info)
	: _font("fonts/VeraMono.ttf")
	, _line_num_font("fonts/DejaVuSansMono.ttf")
	, _info(info)
	, _line_height(15)
	, _file_start_index(0)

{
	_line_num_font.SetFontSize(10);
	_n_line_shown = (rect.size.h - 1) / _line_height;

	win = ax::Window::Create(rect);
	win->event.OnResize = ax::WBind<ax::Size>(this, &TextEditor::OnResize);

	_scrollPanel = ax::Window::Create(ax::Rect(0, 0, rect.size.w, rect.size.h));

	_scrollPanel->property.AddProperty("BlockDrawing");

	_scrollPanel->event.OnPaint = ax::WBind<ax::GC>(this, &TextEditor::OnPaint);

	// Mouse events.
	_scrollPanel->event.OnMouseEnter = ax::WBind<ax::Point>(this, &TextEditor::OnMouseEnter);
	_scrollPanel->event.OnMouseEnterChild = ax::WBind<ax::Point>(this, &TextEditor::OnMouseEnterChild);
	_scrollPanel->event.OnMouseLeave = ax::WBind<ax::Point>(this, &TextEditor::OnMouseLeave);
	_scrollPanel->event.OnMouseLeaveChild = ax::WBind<ax::Point>(this, &TextEditor::OnMouseLeaveChild);
	_scrollPanel->event.OnMouseLeftDown = ax::WBind<ax::Point>(this, &TextEditor::OnMouseLeftDown);
	_scrollPanel->event.OnMouseLeftDoubleClick
		= ax::WBind<ax::Point>(this, &TextEditor::OnMouseLeftDoubleClick);
	_scrollPanel->event.OnMouseLeftDragging = ax::WBind<ax::Point>(this, &TextEditor::OnMouseLeftDragging);
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

	win->node.Add(std::shared_ptr<ax::Window>(_scrollPanel));

	ax::ScrollBar::Info sInfo;
	sInfo.normal = ax::Color(0.80, 0.3);
	sInfo.hover = ax::Color(0.85, 0.3);
	sInfo.clicking = ax::Color(0.82, 0.3);
	sInfo.slider_contour = ax::Color(0.6, 0.2);
	sInfo.contour = ax::Color(0.9, 0.2);
	sInfo.bg_top = ax::Color(0.9, 0.2);
	sInfo.bg_bottom = ax::Color(0.92, 0.2);

	ax::ScrollBar::Events scrollEvents;
	scrollEvents.value_change = GetOnScroll();

	auto scroll_bar
		= ax::shared<ax::ScrollBar>(ax::Rect(rect.size.w - 9, 0, 10, rect.size.h), scrollEvents, sInfo);

	_scrollBar = scroll_bar.get();

	win->node.Add(scroll_bar);

	// Scrollbar is use without window handle, it behave just like a slider.
	int h_size = (int)_logic.GetFileData().size() * _line_height;
	_scrollBar->UpdateWindowSize(ax::Size(rect.size.w, h_size));
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
	_scrollBar->UpdateWindowSize(ax::Size(rect.size.w, h_size));
	win->Update();
	_scrollPanel->Update();

	return err;
}

std::string TextEditor::GetStringContent() const
{
	std::string content;
	const std::vector<std::string>& data = _logic.GetFileData();

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

ax::Point TextEditor::GetPositionFromCursorIndex(const ax::Point& indexes)
{
	// Cursor is above shown text.
	if (indexes.y < _file_start_index) {
		return ax::Point(-1, -1);
	}
	// Cursor is bellow shown text.
	if (indexes.y > _file_start_index + _n_line_shown - 1) {
		return ax::Point(-1, -1);
	}

	return ax::Point(indexes.x, indexes.y - _file_start_index);
}

void TextEditor::MoveToCursorPosition()
{
	ax::Point cur_pos(_logic.GetCursorPosition());

	// Cursor is bellow last shown line.
	if (cur_pos.y > _file_start_index + _n_line_shown - 1) {
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		// Possible problem when file size is smaller than _n_line_shown.
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//		ax::console::Print("Problme");
		_file_start_index = cur_pos.y - _n_line_shown + 1;

		if (_file_start_index < 0) {
			_file_start_index = 0;
		}
	}
	else if (cur_pos.y < _file_start_index) {
		_file_start_index = cur_pos.y;
	}

	// Move scroll bar.
	int diff = (int)_logic.GetFileData().size() - _n_line_shown;

	if (diff < 0) {
		_scrollBar->SetZeroToOneValue(0.0);
		return;
	}

	//	ax::console::Print("F start :", _file_start_index);
	double scroll_ratio = _file_start_index / double(diff);
	_scrollBar->SetZeroToOneValue(scroll_ratio);
}

void TextEditor::OnResize(const ax::Size& size)
{
	const int file_size((int)_logic.GetFileData().size());
	//	double r = (_file_start_index / double(file_size));

	_n_line_shown = size.h / _line_height;
	_scrollPanel->dimension.SetSize(size);

	_scrollBar->GetWindow()->dimension.SetRect(ax::Rect(size.w - 9, 0, 10, size.h));

	int h_size = file_size * _line_height;

	if (h_size < size.h) {
		h_size = size.h;
		ax::console::Print("bb");
		_file_start_index = 0;
		_scrollBar->UpdateWindowSize(ax::Size(size.w, size.h));
	}
	else {
		_scrollBar->UpdateWindowSize(ax::Size(size.w, h_size));
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

	if (diff < 0) {
		diff = 0;
	}

	const double scroll_ratio = _scrollBar->GetZeroToOneValue();
	_file_start_index = ceil(scroll_ratio * diff);
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
			ax::util::String::ReplaceCharWithString(content, '\t', "    ");

			if (!content.empty()) {
				std::vector<std::string>& file_data = _logic.GetFileData();
				ax::Point cur_pos(_logic.GetCursorPosition());

				std::vector<std::string> paste_content(ax::util::String::Split(content, "\n"));

				file_data[cur_pos.y].insert(cur_pos.x, paste_content[0]);

				for (int i = 1; i < paste_content.size(); i++) {
					file_data.insert(file_data.begin() + cur_pos.y + i, paste_content[i]);
				}

				_scrollPanel->Update();
			}
		}
		else if (key == 'a' || key == 'A') {
			_logic.SelectAll();
			_scrollPanel->Update();
		}
		else if (key == 'c' || key == 'C') {
			std::string str = _logic.GetSelectedContent();
			ax::App::GetInstance().SetPasteboardContent(str);
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
	_scrollBar->UpdateWindowSize(ax::Size(_scrollPanel->dimension.GetRect().size.w, h_size));
	MoveToCursorPosition();

	_scrollPanel->Update();
}

void TextEditor::OnBackSpaceDown(const char& key)
{
	_logic.BackSpace();
	int h_size = (int)_logic.GetFileData().size() * _line_height;
	_scrollBar->UpdateWindowSize(ax::Size(_scrollPanel->dimension.GetRect().size.w, h_size));
	MoveToCursorPosition();

	_scrollPanel->Update();
}

void TextEditor::OnKeyDeleteDown(const char& key)
{
	_logic.Delete();
	int h_size = (int)_logic.GetFileData().size() * _line_height;
	_scrollBar->UpdateWindowSize(ax::Size(_scrollPanel->dimension.GetRect().size.w, h_size));
	MoveToCursorPosition();

	_scrollPanel->Update();
}

void TextEditor::OnScrollWheel(const ax::Point& delta)
{
	ax::Size size = _scrollPanel->dimension.GetShownRect().size;
	double scroll_value = (2.0 * delta.y) / double(size.h) + _scrollBar->GetZeroToOneValue();
	scroll_value = ax::util::Clamp(scroll_value, 0.0, 1.0);
	_scrollBar->SetZeroToOneValue(scroll_value);
}

ax::Point TextEditor::GetCursorPositionFromMousePos(const ax::Point& m_pos)
{
	if (!_font) {
		return ax::Point(-1, -1);
	}

	// Line number selection.
	/// @todo Change 25 for a constant.
	if (m_pos.x < 25) {
		/// @todo Don't know what to do here.
		return ax::Point(-1, -1);
	}

	// Calculate line index.
	const int line_index = _file_start_index + m_pos.y / _line_height;

	const std::vector<std::string>& data = _logic.GetFileData();

	// Click bellow text, go to last char.
	if (line_index >= data.size()) {
		return ax::Point((int)data[data.size() - 1].size(), (int)data.size() - 1);
	}

	// Selected line data.
	const std::string& text = data[line_index];

	// Store all characters width from line.
	std::vector<int> next_vec;
	next_vec.reserve(text.size() + 1);

	/// @todo Change 25 for a constant.
	next_vec.push_back(25 + 4);

	// For all char in line.
	for (int i = 0; i < text.size(); i++) {
		_font.SetChar(text[i]);
		next_vec.push_back(_font.GetNextPosition());
	}

	// Line is empty.
	// Set cursor to begnning of line.
	if (next_vec.size() <= 1) {
		return ax::Point(0, line_index);
	}

	// Find char index in line.
	int cursor_index_x = -1;

	// First char in line.
	if (m_pos.x >= next_vec[0] && m_pos.x < next_vec[0] + 0.30 * next_vec[1]) {
		cursor_index_x = 0;
	}
	else {
		int sum_size_x = next_vec[0];

		for (int i = 1; i < next_vec.size() - 1; i++) {
			int left = sum_size_x + 0.5 * next_vec[i];
			int right = sum_size_x + next_vec[i] + 0.5 * next_vec[i + 1];
			if (m_pos.x >= left && m_pos.x < right) {
				cursor_index_x = i;
				break;
			}
			sum_size_x += next_vec[i];
		}
	}

	// Char is not found.
	// Goto to last char for line.
	if (cursor_index_x == -1) {
		return ax::Point((int)text.size(), line_index);
	}

	if (cursor_index_x < _logic.GetFileData()[line_index].size()) {
		return ax::Point(cursor_index_x, line_index);
	}

	return ax::Point(-1, -1);
}

void TextEditor::OnMouseLeftDown(const ax::Point& pos)
{
	const ax::Point mouse_pos = pos - win->dimension.GetAbsoluteRect().position;

	// Line number selection.
	if (mouse_pos.x < 25) {
		return;
	}

	const ax::Point cur_position = GetCursorPositionFromMousePos(mouse_pos);

	if (cur_position.x == -1 || cur_position.y == -1) {
		return;
	}

	_logic.SetCursorPosition(cur_position);
	_logic.UnselectRectangle();
	_logic.BeginSelectCursor();

	_scrollPanel->event.GrabMouse();
	_scrollPanel->Update();
}

void TextEditor::OnMouseLeftDoubleClick(const ax::Point& mouse_pos)
{
	ax::console::Print("Double down");
	//_logic.SelectCurrentLine();
	_logic.SelectCurrentWord();
	_scrollPanel->Update();
}

void TextEditor::OnMouseLeftDragging(const ax::Point& pos)
{
	if (_scrollPanel->event.IsGrabbed()) {
		ax::Point mouse_pos = pos - win->dimension.GetAbsoluteRect().position;

		if (mouse_pos.x < 25) {
			mouse_pos.x = 25;
		}

		const ax::Point cur_position = GetCursorPositionFromMousePos(mouse_pos);

		if (cur_position.x == -1 || cur_position.y == -1) {
			return;
		}

		_logic.ContinueSelectCursor(cur_position);
		_scrollPanel->Update();
	}
}

void TextEditor::OnMouseLeftUp(const ax::Point& pos)
{
	if (_scrollPanel->event.IsGrabbed()) {
		_scrollPanel->event.UnGrabMouse();

		const ax::Point mouse_pos = pos - win->dimension.GetAbsoluteRect().position;

		// Line number selection.
		if (mouse_pos.x < 25) {
			_scrollPanel->Update();
			return;
		}

		const ax::Point cur_position = GetCursorPositionFromMousePos(mouse_pos);

		if (cur_position.x == -1 || cur_position.y == -1) {
			_scrollPanel->Update();
			return;
		}

		_logic.EndSelectCursor(cur_position);
		_scrollPanel->Update();
	}
}

inline bool is_special(char x)
{
	return x == '(' || x == ')' || x == ':' || x == '[' || x == ']' || x == ',' || x == ';' || x == '{'
		|| x == '}' || x == '=';
}

std::string RemoveSpecialChar(const std::string& str)
{
	std::string r = str;
	r.erase(std::remove_if(r.begin(), r.end(), [](char x) { return is_special(x); }), r.end());

	return r;
}

inline bool is_number(const std::string& str)
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
	gc.DrawRectangle(ax::Rect(0, 0, 25, rect.size.h));

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

	const std::vector<std::string>& data = _logic.GetFileData();

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

				// Draw char.
				x = gc.DrawChar(_font, text[i], ax::Point(x, line_pos.y)).x;
				next_vec[i + 1] = x;
			}
		}

		_next_pos_data.push_back(next_vec);
		line_pos += ax::Point(0, 15);
	}

	TextEditorLogic::SelectionRectangle selection_rectangle = _logic.GetSelectionRectangle();

	// Line cursor.
	if (!selection_rectangle.active) {
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

	if (selection_rectangle.active) {
		ax::Point left = GetPositionFromCursorIndex(selection_rectangle.left);
		ax::Point right = GetPositionFromCursorIndex(selection_rectangle.right);

		if (left == ax::Point(-1, -1) || right == ax::Point(-1, -1)) {
			return;
		}

		// Single line selection.
		if (left.y == right.y) {
			const int y_line_index = left.y;
			int l_x = _next_pos_data[y_line_index][left.x];
			int r_x = _next_pos_data[y_line_index][right.x];
			int y = y_line_index * _line_height;
			ax::console::Print(l_x, r_x, y);

			const ax::Rect sel_rect(ax::Point(l_x, y), ax::Size(r_x - l_x, _line_height));
			gc.SetColor(ax::Color(1.0f, 0.0f, 0.0f, 0.3f));
			gc.DrawRectangle(sel_rect);
			return;
		}

		// Multiple lines selection.
		const int n_selected_lines = right.y - left.y;

		if (n_selected_lines == 1) {
			// First line.
			int l_x_1 = _next_pos_data[left.y][left.x];
			int r_x_1 = _next_pos_data[left.y][_next_pos_data[left.y].size() - 1];
			int y_1 = left.y * _line_height;

			const ax::Rect sel_rect_1(ax::Point(l_x_1, y_1), ax::Size(r_x_1 - l_x_1, _line_height));
			gc.SetColor(ax::Color(1.0f, 0.0f, 0.0f, 0.3f));
			gc.DrawRectangle(sel_rect_1);

			// Second line.
			int l_x_2 = _next_pos_data[right.y][0];
			int r_x_2 = _next_pos_data[right.y][right.x];
			int y_2 = right.y * _line_height;

			const ax::Rect sel_rect_2(ax::Point(l_x_2, y_2), ax::Size(r_x_2 - l_x_2, _line_height));
			gc.SetColor(ax::Color(1.0f, 0.0f, 0.0f, 0.3f));
			gc.DrawRectangle(sel_rect_2);

			return;
		}

		// First line.
		int l_x_1 = _next_pos_data[left.y][left.x];
		int r_x_1 = _next_pos_data[left.y][_next_pos_data[left.y].size() - 1];
		int y_1 = left.y * _line_height;

		const ax::Rect sel_rect_1(ax::Point(l_x_1, y_1), ax::Size(r_x_1 - l_x_1, _line_height));
		gc.SetColor(ax::Color(1.0f, 0.0f, 0.0f, 0.3f));
		gc.DrawRectangle(sel_rect_1);

		// Second line.
		int l_x_2 = _next_pos_data[right.y][0];
		int r_x_2 = _next_pos_data[right.y][right.x];
		int y_2 = right.y * _line_height;

		const ax::Rect sel_rect_2(ax::Point(l_x_2, y_2), ax::Size(r_x_2 - l_x_2, _line_height));
		gc.SetColor(ax::Color(1.0f, 0.0f, 0.0f, 0.3f));
		gc.DrawRectangle(sel_rect_2);

		// Middle lines.
		for (int i = left.y + 1; i < right.y; i++) {
			int l_x = _next_pos_data[i][0];
			int r_x = _next_pos_data[i][_next_pos_data[i].size() - 1];
			int y = i * _line_height;

			const ax::Rect sel_rect(ax::Point(l_x, y), ax::Size(r_x - l_x, _line_height));
			gc.SetColor(ax::Color(1.0f, 0.0f, 0.0f, 0.3f));
			gc.DrawRectangle(sel_rect);
		}
	}
}
