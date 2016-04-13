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
 
#ifndef __TEXT_EDITOR_LOGIC_H__
#define __TEXT_EDITOR_LOGIC_H__

#include <OpenAX/OpenAX.h>
#include <OpenAX/ScrollBar.h>
#include <OpenAX/Timer.h>

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
