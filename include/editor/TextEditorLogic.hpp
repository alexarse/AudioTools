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

#include <fstream>
#include <set>

class TextEditorLogic {
public:
	struct SelectionRectangle {
		bool active;
		ax::Point left;
		ax::Point right;
	};

	TextEditorLogic();

	bool OpenFile(const std::string& file_path);

	bool SaveFile(const std::string& file_path);

	std::vector<std::string>& GetFileData();

	const std::vector<std::string>& GetFileData() const;

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

	int GetLineLength(unsigned int index)
	{
		return _file_data[index].size();
	}

	bool IsSelected() const
	{
		return _selection_rectangle.active;
	}

	SelectionRectangle GetSelectionRectangle() const
	{
		return _selection_rectangle;
	}

	void UnselectRectangle();

	void SelectCurrentLine();

	void SelectCurrentWord();

	void BeginSelectCursor();

	void ContinueSelectCursor(const ax::Point& pos);

	void EndSelectCursor(const ax::Point& pos);

	void RemoveSelectedText();

	void SelectAll();

	std::string GetSelectedContent() const;

private:
	std::string _file_path;
	ax::Point _cursor_pos;
	SelectionRectangle _selection_rectangle;
	std::vector<std::string> _file_data;

	void AssignSelectionPos(const ax::Point& pos);
};
