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

#pragma once

#include <axlib/ScrollBar.hpp>
#include <axlib/Timer.hpp>
#include <axlib/axlib.hpp>

#include "editor/TextEditorLogic.hpp"
#include <fstream>
#include <set>

class TextEditor : public ax::Window::Backbone {
public:
	class Info {
	public:
		Info()
			: text_color(0.0)
			, bg_color(1.0)
			, line_number_color(0.3)
			, line_number_bg_color(0.9)
			, cursor_color(0.1)
		{
		}

		Info(const ax::Color& text_c, const ax::Color& bg_c, const ax::Color& line_num_c,
			const ax::Color& line_num_bg_c, const ax::Color& cursor_c)
			: text_color(text_c)
			, bg_color(bg_c)
			, line_number_color(line_num_c)
			, line_number_bg_color(line_num_bg_c)
			, cursor_color(cursor_c)
		{
		}

		ax::Color text_color;
		ax::Color bg_color;
		ax::Color line_number_color;
		ax::Color line_number_bg_color;
		ax::Color cursor_color;
	};

	TextEditor(const ax::Rect& rect, const Info& info);

	void Resize(const ax::Size& size);

	std::string GetStringContent() const;
	std::string GetFilePath() const;

	void SaveFile(const std::string& path);
	void SaveCurrentFile();

	bool OpenFile(const std::string& path);

private:
	ax::Font _font;
	ax::Font _line_num_font;
	TextEditorLogic _logic;
	Info _info;
	ax::Window* _scrollPanel;
	bool _find_cursor_position_x = false;

	std::vector<std::vector<int>> _next_pos_data;

	std::set<std::string> _key_words_cpp;
	std::set<std::string> _number_cpp;

	int _line_height, _file_start_index;
	int _n_line_shown;

	//	axScrollBar* _scrollBar;
	//	ax::ScrollBar::Ptr _scrollBar;
	ax::ScrollBar* _scrollBar;

	ax::Point FileCursorPosToNextPosIndex();

	ax::Point GetPositionFromCursorIndex(const ax::Point& indexes);

	void MoveToCursorPosition();

	axEVENT_ACCESSOR(ax::ScrollBar::Msg, OnScroll);
	void OnScroll(const ax::ScrollBar::Msg& msg);

	void OnResize(const ax::Size& size);

	// Keyboard events.
	void OnLeftArrowDown(const char& key);

	void OnRightArrowDown(const char& key);

	void OnUpArrowDown(const char& key);

	void OnDownArrowDown(const char& key);

	void OnKeyDown(const char& key);

	void OnEnterDown(const char& key);

	void OnBackSpaceDown(const char& key);

	void OnKeyDeleteDown(const char& key);

	// Mouse events.
	void OnScrollWheel(const ax::Point& delta);
	void OnMouseLeave(const ax::Point& pos);
	void OnMouseEnterChild(const ax::Point& pos);
	void OnMouseLeaveChild(const ax::Point& pos);

	void OnMouseEnter(const ax::Point& mouse);

	void OnMouseLeftDown(const ax::Point& mouse);
	void OnMouseLeftDoubleClick(const ax::Point& mouse);
	void OnMouseLeftDragging(const ax::Point& mouse);
	void OnMouseLeftUp(const ax::Point& mouse);

	void OnPaint(ax::GC gc);

	ax::Point GetCursorPositionFromMousePos(const ax::Point& m_pos);
};
