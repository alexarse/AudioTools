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

#include "menu/attribute/atMenuColorAttribute.hpp"
#include "menu/attribute/atMenuAttribute.hpp"
#include <axlib/Button.hpp>
#include <axlib/ColorPicker.hpp>
#include <axlib/Label.hpp>
#include <axlib/TextBox.hpp>
#include <axlib/WindowManager.hpp>
#include <axlib/Xml.hpp>

namespace at {
namespace inspector {
	ColorAttribute::ColorAttribute(
		const ax::Rect& rect, const std::string& name, const std::string& value, ax::event::Function fct)
		: _name(name)
		, _color(ax::Color::FromString(value))
		, _font("fonts/Lato.ttf")
	{
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &ColorAttribute::OnPaint);
		win->event.OnMouseLeftDown = ax::WBind<ax::Point>(this, &ColorAttribute::OnMouseLeftDown);

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
	}

	void ColorAttribute::OnColorSelect(const ax::ColorPicker::Msg& msg)
	{
		win->PushEvent(
			Events::ASSIGN_VALUE, new ax::event::SimpleMsg<std::pair<std::string, std::string>>(
									  std::pair<std::string, std::string>(_name, msg.GetMsg().ToString())));

		_color = msg.GetMsg();
		win->Update();

		ax::App& app(ax::App::GetInstance());
		app.GetPopupManager()->Clear();
		app.GetWindowManager()->UnGrabKey();
		app.GetWindowManager()->UnGrabMouse();
		app.UpdateAll();
	}

	void ColorAttribute::OnColorCancel(const ax::ColorPicker::Msg& msg)
	{
		ax::App& app(ax::App::GetInstance());
		app.GetPopupManager()->Clear();
		app.GetWindowManager()->UnGrabKey();
		app.GetWindowManager()->UnGrabMouse();
		app.UpdateAll();

		//		ax::App& app(ax::App::GetInstance());
		//		auto w = app.GetPopupManager()->GetWindowTree()->GetTopLevel();
		//		app.GetPopupManager()->SetPastWindow(nullptr);
		//		app.GetPopupManager()->UnGrabKey();
		//		app.GetPopupManager()->UnGrabMouse();
		//
		//		w->event.UnGrabKey();
		//		w->event.UnGrabMouse();
		//
		//		w->backbone = nullptr;
		//
		//		app.GetPopupManager()->GetWindowTree()->GetNodeVector().clear();
		//		app.GetPopupManager()->SetPastWindow(nullptr);
	}

	void ColorAttribute::OnMouseLeftDown(const ax::Point& pos)
	{
		// Create color picker.
		ax::Slider::Info sld_info;
		sld_info.img_path = "resources/sliderPlain.png";
		sld_info.btn_size = ax::Size(12, 12);
		sld_info.slider_width = 4;
		sld_info.contour_round_radius = 0;
		sld_info.bgColorNormal = ax::Color(0.97);
		sld_info.bgColorHover = sld_info.bgColorNormal;
		sld_info.bgColorClicked = sld_info.bgColorNormal;

		sld_info.sliderColorNormal = ax::Color(0.801);
		sld_info.sliderColorHover = sld_info.sliderColorNormal;
		sld_info.sliderColorClicked = sld_info.sliderColorNormal;
		sld_info.sliderContourColor = ax::Color(0.901);

		sld_info.contourColor = ax::Color(0.88);
		sld_info.backSliderColor = ax::Color(0.0);
		sld_info.backSliderContourColor = ax::Color(0.0);

		ax::TextBox::Info txtInfo;
		txtInfo.normal = ax::Color(0.97);
		txtInfo.hover = txtInfo.normal;
		txtInfo.selected = txtInfo.normal;
		txtInfo.highlight = ax::Color(0.4f, 0.4f, 0.6f, 0.2f);
		txtInfo.contour = ax::Color(0.88);
		txtInfo.cursor = ax::Color(1.0f, 0.0f, 0.0f);
		txtInfo.selected_shadow = ax::Color(0.8f, 0.8f, 0.8f);
		txtInfo.font_color = ax::Color(0.0);

		ax::Button::Info btn_info;
		btn_info.normal = ax::Color(0.97);
		btn_info.hover = ax::Color(0.99);
		btn_info.clicking = ax::Color(0.96);
		btn_info.selected = btn_info.normal;
		btn_info.contour = ax::Color(0.88);
		btn_info.font_color = ax::Color(0.0);
		btn_info.corner_radius = 0;

		ax::ColorPicker::Info cp_info;
		cp_info.btn_info = btn_info;
		cp_info.sld_info = sld_info;
		cp_info.txt_info = txtInfo;

		ax::ColorPicker::Events cp_evts;
		cp_evts.select = GetOnColorSelect();
		cp_evts.cancel = GetOnColorCancel();

		auto c_picker = ax::shared<ax::ColorPicker>(
			ax::Point(win->dimension.GetAbsoluteRect().position.x - 120, 30), cp_evts, cp_info, _color);

		ax::App& app(ax::App::GetInstance());
		app.GetPopupManager()->Clear();
		app.AddPopupTopLevel(c_picker);

		//		ax::App::GetInstance().GetPopupManager()->GetWindowTree()->AddTopLevel(
		//			std::shared_ptr<ax::Window>(c_picker->GetWindow()));
		//
		//		c_picker->GetWindow()->backbone = c_picker;
	}

	void ColorAttribute::OnPaint(ax::GC gc)
	{
		const ax::Rect rect(win->dimension.GetDrawingRect());

		const ax::Rect color_rect(rect);

		// Draw color rectangle background.
		int line_index = 0;
		for (int y = color_rect.position.y; y < color_rect.position.y + color_rect.size.h; y += 5) {
			for (int x = color_rect.position.x; x < color_rect.position.x + color_rect.size.w - 5; x += 10) {
				int xx = x;
				int sx = 5;

				if (line_index % 2) {
					gc.SetColor(ax::Color(0.95));
					gc.DrawRectangle(ax::Rect(xx, y, 5, 5));
					gc.SetColor(ax::Color(0.99));
					gc.DrawRectangle(ax::Rect(xx + 5, y, sx, 5));
				}
				else {
					gc.SetColor(ax::Color(0.99));
					gc.DrawRectangle(ax::Rect(xx, y, 5, 5));
					gc.SetColor(ax::Color(0.95));
					gc.DrawRectangle(ax::Rect(xx + 5, y, sx, 5));
				}
			}

			line_index++;
		}

		gc.SetColor(_color);
		gc.DrawRectangle(ax::Rect(rect.position, ax::Size(rect.size.w, rect.size.h + 1)));

		gc.SetColor(ax::Color(0.88));
		gc.DrawRectangleContour(ax::Rect(rect.position, ax::Size(rect.size.w, rect.size.h + 1)));

		gc.SetColor(ax::Color(0.88));
		gc.DrawLine(ax::Point(91, 0), ax::Point(91, rect.size.h + 1));
	}
}
}
