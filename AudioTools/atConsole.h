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

#ifndef __AT_CONSOLE_H__
#define __AT_CONSOLE_H__

#include <OpenAX/OpenAX.h>
#include <OpenAX/ScrollBar.h>
#include <OpenAX/Timer.h>
#include <OpenAX/Button.h>

#include "TextEditor.h"

#include <fstream>
#include <set>

namespace at {
class Console : public ax::Window::Backbone {
public:
	Console(const ax::Rect& rect);
	
	enum Events : ax::Event::Id { WRITE_ERROR };
	
private:
	ax::Font _font;
	std::vector<std::pair<int, std::string>> _lines;

	axEVENT_DECLARATION(ax::Event::StringMsg, OnConsoleUpdate);
//	axEVENT_DECLARATION(ax::Event::SimpleMsg<int>, OnConsoleErrorUpdate);
	axEVENT_DECLARATION(ax::Event::StringMsg, OnConsoleErrorUpdate);
	void OnResize(const ax::Size& size);
	void OnPaint(ax::GC gc);
};
}

#endif // __AT_CONSOLE_H__
