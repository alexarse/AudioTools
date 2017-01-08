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

#include "menu/attribute/atMenuBoolAttribute.hpp"
#include <axlib/Button.hpp>
#include <axlib/ColorPicker.hpp>
#include <axlib/Label.hpp>
#include <axlib/TextBox.hpp>
#include <axlib/Toggle.hpp>
#include <axlib/WindowManager.hpp>
#include <axlib/Xml.hpp>

namespace at {
namespace inspector {
	BoolAttribute::BoolAttribute(
		const ax::Rect& rect, const std::string& name, const std::string& value, ax::event::Function fct)
		: _name(name)
	{
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &BoolAttribute::OnPaint);

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

		ax::Toggle::Info tog_info;
		tog_info.normal = ax::Color(0.0, 0.0);
		tog_info.hover = ax::Color(0.0, 0.0);
		tog_info.clicking = ax::Color(0.0, 0.0);
		tog_info.font_color = ax::Color(0.0, 0.0);
		tog_info.selected = ax::Color(0.0, 0.0);
		tog_info.selected_hover = ax::Color(0.0, 0.0);
		tog_info.selected_clicking = ax::Color(0.0, 0.0);
		tog_info.selected_font_color = ax::Color(0.0, 0.0);

		tog_info.contour = ax::Color(0.0, 0.0);

		tog_info.font_size = 12;

		tog_info.img = "resources/bool_toggle.png";
		tog_info.single_img = false;

		auto tog = ax::shared<ax::Toggle>(ax::Rect(95, 4, 13, 13), GetOnToggleClick(), tog_info);
		win->node.Add(tog);

		tog->SetSelected((bool)std::stoi(value));
	}

	void BoolAttribute::OnToggleClick(const ax::Toggle::Msg& msg)
	{
		int selected = (int)msg.GetSelected();
		win->PushEvent(
			Events::ASSIGN_VALUE, new ax::event::SimpleMsg<std::pair<std::string, std::string>>(
									  std::pair<std::string, std::string>(_name, std::to_string(selected))));
	}

	void BoolAttribute::OnPaint(ax::GC gc)
	{
		const ax::Rect rect(win->dimension.GetDrawingRect());

		gc.SetColor(ax::Color(0.94));
		gc.DrawRectangle(rect);

		gc.SetColor(ax::Color(1.0));
		gc.DrawRectangle(rect.GetInteriorRect(ax::Point(1, 1)));

		gc.SetColor(ax::Color(0.94));
		gc.DrawRectangleContour(rect);
	}
}
}
