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

#include "atConsole.h"
#include "atConsoleStream.h"

namespace at {
Console::Console(const ax::Rect& rect)
	: _font(0)
{
	win = ax::Window::Create(rect);
	win->event.OnPaint = ax::WBind<ax::GC>(this, &Console::OnPaint);
	win->event.OnResize = ax::WBind<ax::Size>(this, &Console::OnResize);
	win->event.OnScrollWheel = ax::WBind<ax::Size>(this, &Console::OnScrollWheel);
	win->event.OnMouseEnter = ax::WBind<ax::Size>(this, &Console::OnMouseEnter);
	win->event.OnMouseEnterChild = ax::WBind<ax::Size>(this, &Console::OnMouseEnterChild);
	win->event.OnMouseLeave = ax::WBind<ax::Size>(this, &Console::OnMouseLeave);
	win->event.OnMouseLeaveChild = ax::WBind<ax::Size>(this, &Console::OnMouseLeaveChild);

	at::ConsoleStream::GetInstance()->AddConnection(at::ConsoleStream::WRITE_NEW_LINE, GetOnConsoleUpdate());
	at::ConsoleStream::GetInstance()->AddConnection(
		at::ConsoleStream::WRITE_ERROR, GetOnConsoleErrorUpdate());
	at::ConsoleStream::GetInstance()->Write("Console init.");

	_panel = ax::Window::Create(ax::Rect(0, 0, rect.size.x, rect.size.y));
	win->node.Add(ax::Window::Ptr(_panel));

	_txt_panel = ax::Window::Create(ax::Rect(0, 0, rect.size.x, rect.size.y));
	_panel->node.Add(ax::Window::Ptr(_txt_panel));

	_txt_panel->event.OnPaint = ax::WBind<ax::GC>(this, &Console::OnPanelPaint);

	ax::ScrollBar::Info sInfo;
	sInfo.normal = ax::Color(0.80, 0.3);
	sInfo.hover = ax::Color(0.85, 0.3);
	sInfo.clicking = ax::Color(0.82, 0.3);
	sInfo.slider_contour = ax::Color(0.6, 0.2);
	sInfo.contour = ax::Color(0.9, 0.2);
	sInfo.bg_top = ax::Color(0.9, 0.2);
	sInfo.bg_bottom = ax::Color(0.92, 0.2);

	ax::Rect sRect(rect.size.x - 9, 0, 10, rect.size.y);
	_scrollBar = ax::shared<ax::ScrollBar>(sRect, ax::ScrollBar::Events(), sInfo);

	win->node.Add(_scrollBar);

	_panel->property.AddProperty("BlockDrawing");
	_panel->dimension.SetSizeNoShowRect(ax::Size(rect.size.x, rect.size.y));

	_scrollBar->SetWindowHandle(_panel);
	_scrollBar->UpdateWindowSize(_panel->dimension.GetSize());
}

void Console::OnConsoleUpdate(const ax::Event::StringMsg& msg)
{
	ax::StringVector lines = ax::Utils::String::Split(msg.GetMsg(), "\n");

	for(int i = 0; i < lines.size(); i++) {
		bool new_block = false;
		if(i == 0) {
			new_block = true;
		}
		
		_lines.push_back(MessageFormat(new_block, 0, lines[i]));
	}
	
//	for (auto& n : lines) {
//		_lines.push_back(std::pair<int, std::string>(0, n));
//	}

	_panel->Update();
}

void Console::OnConsoleErrorUpdate(const ax::Event::StringMsg& msg)
{
	// Set event to bottom section to flip to console on error.
	win->PushEvent(WRITE_ERROR, new ax::Event::EmptyMsg());

	ax::StringVector lines = ax::Utils::String::Split(msg.GetMsg(), "\n");

	for(int i = 0; i < lines.size(); i++) {
		bool new_block = false;
		if(i == 0) {
			new_block = true;
		}
		
		_lines.push_back(MessageFormat(new_block, 1, lines[i]));
	}

	int panel_height = 10 + int(_lines.size()) * 15;
	_txt_panel->dimension.SetSize(ax::Size(_panel->dimension.GetSize().x, panel_height));
	_panel->dimension.SetSizeNoShowRect(ax::Size(_panel->dimension.GetSize().x, panel_height));
	//	ax::Print("Panel height", panel_height);

	_scrollBar->UpdateWindowSize(_panel->dimension.GetSize());

	if (_txt_panel->dimension.GetSize().y > _panel->dimension.GetShownRect().size.y) {
		_scrollBar->SetZeroToOneValue(1.0);
	}
	//	_panel->Update();
	_txt_panel->Update();
}

void Console::OnMouseEnter(const ax::Point& pos)
{
	win->event.GrabScroll();
}

void Console::OnMouseEnterChild(const ax::Point& pos)
{
	win->event.GrabScroll();
}

void Console::OnMouseLeave(const ax::Point& pos)
{
	if (!win->dimension.GetAbsoluteRect().IsPointInside(pos)) {
		win->event.UnGrabScroll();
	}
}

void Console::OnMouseLeaveChild(const ax::Point& pos)
{
	if (!win->dimension.GetAbsoluteRect().IsPointInside(pos)) {
		win->event.UnGrabScroll();
	}
}

void Console::OnScrollWheel(const ax::Point& delta)
{
	double scroll_value
		= (delta.y / (double)ax::App::GetInstance().GetFrameSize().y) + _scrollBar->GetZeroToOneValue();
	
	_scrollBar->SetZeroToOneValue(scroll_value);
}

void Console::OnResize(const ax::Size& size)
{
	ax::Rect sRect(size.x - 9, 0, 10, size.y);
	_scrollBar->GetWindow()->dimension.SetRect(sRect);
	_panel->dimension.SetShownRect(ax::Rect(0, 0, size.x, size.y));
	_txt_panel->dimension.SetSize(ax::Size(size.x, _txt_panel->dimension.GetSize().y));
	_scrollBar->UpdateWindowSize(_panel->dimension.GetSize());
}

void Console::OnPaint(ax::GC gc)
{
	const ax::Rect rect(win->dimension.GetDrawingRect());
	gc.SetColor(ax::Color(1.0));
	gc.DrawRectangle(rect);
	gc.DrawRectangleContour(rect);
}

void Console::OnPanelPaint(ax::GC gc)
{
	const ax::Rect rect(_txt_panel->dimension.GetDrawingRect());
	gc.SetColor(ax::Color(1.0));
	gc.DrawRectangle(rect);

	const int line_height = 15;

	ax::Point pos(5, 5);
	gc.SetColor(ax::Color(0.0));

	bool block_flip = false;

	for (auto& n : _lines) {
		if(n.new_block) {
			block_flip = !block_flip;
		}
	
		ax::Rect line_rect(rect.position.x, pos.y, rect.size.x, 15);
	
		// Draw bg.
		if(block_flip) {
			gc.DrawRectangleColorFade(line_rect, ax::Color(1.0), ax::Color(0.98));
		}
		else {
			gc.DrawRectangleColorFade(line_rect, ax::Color(0.95), ax::Color(0.90));
		}
	
	
		if (n.type == 0) {
			gc.SetColor(ax::Color(0.0));
		}
		else if (n.type == 1) {
			gc.SetColor(ax::Color(255, 0, 0));
		}

		gc.DrawString(_font, n.msg, pos);
		pos.y += line_height;
	}

	gc.SetColor(ax::Color(1.0));
	gc.DrawRectangleContour(rect);
}
}