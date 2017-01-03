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

#ifndef __AT_CONSOLE_H__
#define __AT_CONSOLE_H__

#include <axlib/Button.hpp>
#include <axlib/ScrollBar.hpp>
#include <axlib/Timer.hpp>
#include <axlib/axlib.hpp>

#include "editor/TextEditor.hpp"

#include <fstream>
#include <set>

namespace at {
class Console : public ax::Window::Backbone {
public:
	Console(const ax::Rect& rect);

	enum Events : ax::event::Id { WRITE_ERROR };

	void Clear();

private:
	ax::Font _font;

	struct MessageFormat {
		MessageFormat(bool n, int t, const std::string& m)
			: new_block(n)
			, type(t)
			, msg(m)
		{
		}

		bool new_block;
		int type;
		std::string msg;
	};

	std::vector<MessageFormat> _lines;
	ax::Window* _panel;
	ax::Window* _txt_panel;
	ax::ScrollBar::Ptr _scrollBar;

	axEVENT_DECLARATION(ax::event::StringMsg, OnConsoleUpdate);
	axEVENT_DECLARATION(ax::event::StringMsg, OnConsoleErrorUpdate);

	void OnMouseEnter(const ax::Point& pos);
	void OnMouseLeave(const ax::Point& pos);
	void OnMouseEnterChild(const ax::Point& pos);
	void OnMouseLeaveChild(const ax::Point& pos);
	void OnScrollWheel(const ax::Point& delta);

	void OnResize(const ax::Size& size);
	void OnPaint(ax::GC gc);
	void OnPanelPaint(ax::GC gc);
};
}

#endif // __AT_CONSOLE_H__
