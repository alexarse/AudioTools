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
 
#ifndef __CODE_EDITOR_H__
#define __CODE_EDITOR_H__

#include <OpenAX/OpenAX.h>
#include <OpenAX/ScrollBar.h>
#include <OpenAX/Timer.h>
#include <OpenAX/Button.h>

#include "TextEditor.h"


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
