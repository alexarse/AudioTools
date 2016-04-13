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

#ifndef __AT_EDITOR_BOTTOM_SECTION_H__
#define __AT_EDITOR_BOTTOM_SECTION_H__

#include <OpenAX/OpenAX.h>

#include <OpenAX/Button.h>
#include <OpenAX/ScrollBar.h>
#include <OpenAX/Timer.h>

#include "TextEditor.h"
#include "atConsole.h"

namespace at {
namespace editor {
	class BottomSection : public ax::Window::Backbone {
	public:
		BottomSection(const ax::Rect& rect);

		enum : ax::Event::Id { RESIZE };
		
		bool OpenFile(const std::string& path);
		void SaveFile(const std::string& path);
		std::string GetScriptPath() const;

	private:
		// Resize elements.
		ax::Point _delta_resize_click;
		ax::Rect _resize_click_old_rect;
		bool _has_resize_cursor = false;
		
		std::string _file_path;
		ax::Font _font;
		
		bool _is_txt_edit;
		ax::Button* _console_btn;
		ax::Button* _txt_btn;
		
		TextEditor* _txt_editor;
		Console* _console;
		
		static const int MINIMUM_HEIGHT = 200;
		static const int TOP_BAR_HEIGHT = 25;
		
		axEVENT_DECLARATION(ax::Button::Msg, OnTextEditor);
		axEVENT_DECLARATION(ax::Button::Msg, OnConsole);
	
		void OnMouseLeftDoubleClick(const ax::Point& pos);
		void OnMouseLeave(const ax::Point& pos);
		void OnMouseMotion(const ax::Point& pos);
		void OnMouseLeftDown(const ax::Point& pos);
		void OnMouseLeftDragging(const ax::Point& pos);
		void OnMouseLeftUp(const ax::Point& pos);
		void OnResize(const ax::Size& size);
		void OnPaint(ax::GC gc);
	};
}
}
#endif // __AT_EDITOR_BOTTOM_SECTION_H__
