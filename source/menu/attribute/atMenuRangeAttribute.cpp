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

#include "menu/attribute/atMenuRangeAttribute.hpp"
#include <axlib/Button.hpp>
#include <axlib/ColorPicker.hpp>
#include <axlib/Label.hpp>
#include <axlib/TextBox.hpp>
#include <axlib/Toggle.hpp>
#include <axlib/WindowManager.hpp>
#include <axlib/Xml.hpp>

namespace at {
namespace inspector {
	RangeAttribute::RangeAttribute(
		const ax::Rect& rect, const std::string& name, const std::string& value, ax::event::Function fct)
		: _name(name)
		, _font("fonts/Lato.ttf")
	{
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &RangeAttribute::OnPaint);

		if (fct) {
			win->AddConnection(Events::ASSIGN_VALUE, fct);
		}

		ax::Label::Info labelInfo;
		labelInfo.normal = ax::Color(0.98);
		labelInfo.contour = ax::Color(0.88);
		labelInfo.font_color = ax::Color(0.0);
		labelInfo.font_size = 12;
		labelInfo.font_name = "fonts/Lato.ttf";
		labelInfo.alignement = ax::util::Alignement::axALIGN_LEFT;

		ax::Point pos(0, 0);
		win->node.Add(ax::shared<ax::Label>(ax::Rect(pos, ax::Size(90, 25)), labelInfo, _name));

		ax::TextBox::Info txtInfo;
		txtInfo.normal = ax::Color(1.0);
		txtInfo.hover = ax::Color(1.0);
		txtInfo.selected = ax::Color(1.0);
		txtInfo.highlight = ax::Color(0.4f, 0.4f, 0.6f, 0.2f);
		txtInfo.contour = ax::Color(0.88);
		txtInfo.cursor = ax::Color(1.0f, 0.0f, 0.0f);
		txtInfo.selected_shadow = ax::Color(0.8f, 0.8f, 0.8f);
		txtInfo.font_color = ax::Color(0.0);

		ax::NumberScroll::Info scroll_info;
		scroll_info.up_btn = "resources/drop_up.png";
		scroll_info.down_btn = "resources/drop_down.png";

		// Txt box.
		scroll_info.txt_info.normal = ax::Color(1.0);
		scroll_info.txt_info.hover = ax::Color(1.0);
		scroll_info.txt_info.selected = ax::Color(1.0);
		scroll_info.txt_info.highlight = ax::Color(0.4f, 0.4f, 0.6f, 0.2f);
		scroll_info.txt_info.contour = ax::Color(0.88);
		scroll_info.txt_info.cursor = ax::Color(1.0f, 0.0f, 0.0f);
		scroll_info.txt_info.selected_shadow = ax::Color(0.8f, 0.8f, 0.8f);
		scroll_info.txt_info.font_color = ax::Color(0.0);

		// Button.
		scroll_info.btn_info.normal = ax::Color(0.92);
		scroll_info.btn_info.hover = ax::Color(0.94);
		scroll_info.btn_info.clicking = ax::Color(0.90);
		scroll_info.btn_info.selected = scroll_info.btn_info.normal;
		scroll_info.btn_info.contour = ax::Color(0.88);
		scroll_info.btn_info.font_color = ax::Color(0.0, 0.0);

		auto size_values = ax::util::String::Split(value, ",");
		int w_value = std::stoi(size_values[0]);
		int h_value = std::stoi(size_values[1]);

		auto w_scroll = ax::shared<ax::NumberScroll>(ax::Rect(110, 0, 60, rect.size.h + 1), GetOnLeftChange(),
			scroll_info, w_value, ax::util::Control::Type::INTEGER, ax::util::Range2D<double>(0.0, 10000.0),
			1.0);

		_left_scroll = w_scroll.get();
		win->node.Add(w_scroll);

		auto h_scroll = ax::shared<ax::NumberScroll>(ax::Rect(190, 0, 60, rect.size.h + 1),
			GetOnRightChange(), scroll_info, h_value, ax::util::Control::Type::INTEGER,
			ax::util::Range2D<double>(0.0, 10000.0), 1.0);

		_right_scroll = h_scroll.get();
		win->node.Add(h_scroll);
	}

	void RangeAttribute::OnLeftChange(const ax::NumberScroll::Msg& msg)
	{
		std::string w_str = std::to_string((int)msg.GetValue());
		std::string h_str = std::to_string((int)_right_scroll->GetValue());
		std::string out_str(w_str + ", " + h_str);

		win->PushEvent(Events::ASSIGN_VALUE, new ax::event::SimpleMsg<std::pair<std::string, std::string>>(
												 std::pair<std::string, std::string>(_name, out_str)));
	}

	void RangeAttribute::OnRightChange(const ax::NumberScroll::Msg& msg)
	{
		std::string w_str = std::to_string((int)_left_scroll->GetValue());
		std::string h_str = std::to_string((int)msg.GetValue());
		std::string out_str(w_str + ", " + h_str);

		win->PushEvent(Events::ASSIGN_VALUE, new ax::event::SimpleMsg<std::pair<std::string, std::string>>(
												 std::pair<std::string, std::string>(_name, out_str)));
	}

	void RangeAttribute::OnPaint(ax::GC gc)
	{
		const ax::Rect rect(win->dimension.GetDrawingRect());

		gc.SetColor(ax::Color(0.96));
		gc.DrawRectangle(rect);

		gc.SetColor(ax::Color(0.0));
		gc.DrawString(_font, "l :", ax::Point(93, 3));

		gc.DrawString(_font, "r :", ax::Point(175, 3));

		gc.SetColor(ax::Color(0.88));
		gc.DrawRectangleContour(ax::Rect(rect.position, ax::Size(rect.size.w, rect.size.h + 1)));

		gc.SetColor(ax::Color(0.88));
		gc.DrawLine(ax::Point(91, 0), ax::Point(91, rect.size.h + 1));
	}
}
}
