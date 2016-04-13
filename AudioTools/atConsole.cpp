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
 
#include "atConsole.h"

namespace at {
Console::Console(const ax::Rect& rect)
	: _font(0)
{
	win = ax::Window::Create(rect);
	win->event.OnPaint = ax::WBind<ax::GC>(this, &Console::OnPaint);
	win->event.OnResize = ax::WBind<ax::Size>(this, &Console::OnResize);
	
//	std::streambuf* old = std::cout.rdbuf(_buffer.rdbuf());
}

void Console::OnResize(const ax::Size& size)
{
	
}

void Console::OnPaint(ax::GC gc)
{
	const ax::Rect rect(win->dimension.GetDrawingRect());
	gc.SetColor(ax::Color(1.0));
	gc.DrawRectangle(rect);
	
	ax::StringVector vec = ax::Utils::String::Split(_buffer.str(), "\n");
	gc.SetColor(ax::Color(0.0));
	
	ax::Point pos(5, 5);
	
	if(vec.size() > 10) {
		for(int i = int(vec.size() - 10); i < vec.size(); i++) {
			gc.DrawString(_font, vec[i], pos);
			pos.y += 15;
		}
	}
	else {
		for(auto& n : vec) {
			gc.DrawString(_font, n, pos);
			pos.y += 15;
		}
	}
}
}