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

#include "menu/attribute/atMenuAttribute.hpp"
#include <axlib/Label.hpp>
#include <axlib/TextBox.hpp>

namespace at {
namespace inspector {
	MenuAttribute::MenuAttribute(const ax::Rect& rect, const std::string& name, const std::string& value,
		ax::event::Function fct, int label_font_size)
		: _name(name)
		, _value(value)
		, _font("fonts/Lato.ttf")
	{
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &MenuAttribute::OnPaint);

		if (fct) {
			win->AddConnection(Events::ASSIGN_VALUE, fct);
		}

		ax::TextBox::Info txtInfo;
		txtInfo.normal = ax::Color(1.0);
		txtInfo.hover = ax::Color(1.0);
		txtInfo.selected = ax::Color(1.0);
		txtInfo.highlight = ax::Color(0.4f, 0.4f, 0.6f, 0.2f);
		txtInfo.contour = ax::Color(0.88);
		txtInfo.cursor = ax::Color(1.0f, 0.0f, 0.0f);
		txtInfo.selected_shadow = ax::Color(0.8f, 0.8f, 0.8f);
		txtInfo.font_color = ax::Color(0.0);

		ax::Label::Info labelInfo;
		labelInfo.normal = ax::Color(0.98);
		labelInfo.contour = ax::Color(0.88);
		labelInfo.font_color = ax::Color(0.0);
		labelInfo.font_size = label_font_size;
		labelInfo.font_name = "fonts/Lato.ttf";
		labelInfo.alignement = ax::util::Alignement::axALIGN_LEFT;

		ax::Point pos(0, 0);
		win->node.Add(ax::shared<ax::Label>(ax::Rect(pos, ax::Size(90, rect.size.h + 1)), labelInfo, _name));

		ax::TextBox::Events txt_evts;
		txt_evts.enter_click = ax::event::Function([&](ax::event::Msg* msg) {
			ax::TextBox::Msg* tmsg = static_cast<ax::TextBox::Msg*>(msg);
			std::string msg_str = tmsg->GetMsg();

			win->PushEvent(
				Events::ASSIGN_VALUE, new ax::event::SimpleMsg<std::pair<std::string, std::string>>(
										  std::pair<std::string, std::string>(_name, msg_str)));
		});

		win->node.Add(
			ax::shared<ax::TextBox>(ax::Rect(ax::Point(90, 0), ax::Size(rect.size.w - 90, rect.size.h + 1)),
				txt_evts, txtInfo, "", _value));
	}

	void MenuAttribute::OnPaint(ax::GC gc)
	{
		const ax::Rect rect(win->dimension.GetDrawingRect());

		gc.SetColor(ax::Color(0.94));
		gc.DrawRectangle(rect);
		gc.DrawRectangleContour(rect);
	}
}
}
