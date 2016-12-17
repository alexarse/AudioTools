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

#include "atCommon.h"
#include "atMenuAttribute.h"
#include "atMenuBoolAttribute.hpp"
#include "atMenuColorAttribute.hpp"
#include "atMenuIntegerAttribute.hpp"
#include "atMenuPathAttribute.hpp"
#include "atMenuPointAttribute.hpp"
#include "atMenuSizeAttribute.hpp"
#include "atUniqueNameComponent.h"
#include "editor/atEditorInspectorMenu.hpp"
#include "python/PyoComponent.hpp"

#include <axlib/WindowManager.hpp>

namespace at {
namespace editor {
	MenuSeparator::MenuSeparator(const ax::Rect& rect, const std::string& name)
		: _name(name)
		, _font("fonts/FreeSansBold.ttf")
	{
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &MenuSeparator::OnPaint);
	}

	void MenuSeparator::OnPaint(ax::GC gc)
	{
		ax::Rect rect(win->dimension.GetDrawingRect());

		gc.SetColor(ax::Color(0.94));
		gc.DrawRectangle(rect);

		gc.SetColor(ax::Color(0.3));
		gc.DrawString(_font, _name, ax::Point(10, 2));

		gc.SetColor(ax::Color(0.94));
		gc.DrawRectangleContour(rect);
	}

	/*
	 * InspectorMenu.
	 */
	InspectorMenu::InspectorMenu(const ax::Rect& rect)
		: _selected_handle(nullptr)
		, _font("fonts/Lato.ttf")
		, _font_bold("fonts/FreeSansBold.ttf")
		, _has_multiple_widget_selected(false)
	{
		// Create window.
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &InspectorMenu::OnPaint);
	}

	void InspectorMenu::SetWidgetHandle(ax::Window* handle)
	{
		// Clear old content.
		RemoveHandle();
		_selected_handle = handle;

		if (_selected_handle) {
			ax::Rect rect(win->dimension.GetRect());

			ax::Size separator_size(rect.size.w, 20);

			ax::Point att_pos(0, 0);
			ax::Size att_size(rect.size.w, 20);

			win->node.Add(ax::shared<MenuSeparator>(ax::Rect(att_pos, separator_size), "Unique Name"));

			// Unique name attributes.
			if (_selected_handle->component.Has("unique_name")) {
				at::UniqueNameComponent::Ptr comp
					= _selected_handle->component.Get<at::UniqueNameComponent>("unique_name");

				att_pos.y += separator_size.h;

				std::string name = comp->GetName();

				auto menu = ax::shared<at::inspector::MenuAttribute>(
					ax::Rect(att_pos, att_size), "name", name, GetOnUniqueName());

				win->node.Add(menu);
				att_pos.y += att_size.h;
			}

			// Add widget separator.
			win->node.Add(ax::shared<MenuSeparator>(ax::Rect(att_pos, separator_size), "Widget"));
			att_pos.y += separator_size.h;

			ax::widget::Component::Ptr widget
				= _selected_handle->component.Get<ax::widget::Component>("Widget");

			// Builder attributes.
			std::vector<std::pair<std::string, std::string>> atts_pair = widget->GetBuilderAttributes();
			std::map<std::string, std::string> atts_map;

			for (auto& n : atts_pair) {
				atts_map.insert(n);
			}

			std::vector<ax::widget::ParamInfo> builder_atts_info = widget->GetBuilderAttributesInfo();

			for (auto& n : builder_atts_info) {
				std::string value = atts_map[n.second];

				if (n.first == ax::widget::ParamType::COLOR) {
					win->node.Add(ax::shared<at::inspector::ColorAttribute>(
						ax::Rect(att_pos, att_size), n.second, value, GetOnWidgetUpdate()));
				}
				else if (n.first == ax::widget::ParamType::BOOLEAN) {
					win->node.Add(ax::shared<at::inspector::BoolAttribute>(
						ax::Rect(att_pos, att_size), n.second, value, GetOnWidgetUpdate()));
				}
				else if (n.first == ax::widget::ParamType::POINT) {
					win->node.Add(ax::shared<at::inspector::PointAttribute>(
						ax::Rect(att_pos, att_size), n.second, value, GetOnWidgetUpdate()));
				}
				else if (n.first == ax::widget::ParamType::SIZE) {
					win->node.Add(ax::shared<at::inspector::SizeAttribute>(
						ax::Rect(att_pos, att_size), n.second, value, GetOnWidgetUpdate()));
				}
				else if (n.first == ax::widget::ParamType::INTEGER) {
					win->node.Add(ax::shared<at::inspector::IntegerAttribute>(
						ax::Rect(att_pos, att_size), n.second, value, GetOnWidgetUpdate()));
				}
				else if (n.first == ax::widget::ParamType::FILEPATH) {
					win->node.Add(ax::shared<at::inspector::PathAttribute>(
						ax::Rect(att_pos, att_size), n.second, value, GetOnWidgetUpdate()));
				}
				// Text attribute.
				else {
					win->node.Add(ax::shared<at::inspector::MenuAttribute>(
						ax::Rect(att_pos, att_size), n.second, value, GetOnWidgetUpdate()));
				}

				att_pos.y += att_size.h;
			}

			win->node.Add(ax::shared<MenuSeparator>(ax::Rect(att_pos, separator_size), "Info"));

			att_pos.y += separator_size.h;

			// Widget info attributes.
			ax::widget::Info* info = widget->GetInfo();
			std::vector<ax::widget::ParamInfo> info_atts = info->GetParametersInfo();

			for (auto& n : info_atts) {
				std::string value = info->GetAttributeValue(n.second);

				if (n.first == ax::widget::ParamType::COLOR) {
					win->node.Add(ax::shared<at::inspector::ColorAttribute>(
						ax::Rect(att_pos, att_size), n.second, value, GetOnInfoUpdate()));
				}
				else if (n.first == ax::widget::ParamType::BOOLEAN) {
					win->node.Add(ax::shared<at::inspector::BoolAttribute>(
						ax::Rect(att_pos, att_size), n.second, value, GetOnInfoUpdate()));
				}
				else if (n.first == ax::widget::ParamType::POINT) {
					win->node.Add(ax::shared<at::inspector::PointAttribute>(
						ax::Rect(att_pos, att_size), n.second, value, GetOnInfoUpdate()));
				}
				else if (n.first == ax::widget::ParamType::SIZE) {
					win->node.Add(ax::shared<at::inspector::SizeAttribute>(
						ax::Rect(att_pos, att_size), n.second, value, GetOnInfoUpdate()));
				}
				else if (n.first == ax::widget::ParamType::INTEGER) {
					win->node.Add(ax::shared<at::inspector::IntegerAttribute>(
						ax::Rect(att_pos, att_size), n.second, value, GetOnInfoUpdate()));
				}
				else if (n.first == ax::widget::ParamType::FILEPATH) {
					win->node.Add(ax::shared<at::inspector::PathAttribute>(
						ax::Rect(att_pos, att_size), n.second, value, GetOnInfoUpdate()));
				}
				else {
					win->node.Add(ax::shared<at::inspector::MenuAttribute>(
						ax::Rect(att_pos, att_size), n.second, value, GetOnInfoUpdate()));
				}

				att_pos.y += att_size.h;
			}

			// Python attributes.
			if (_selected_handle->component.Has("pyo")) {
				pyo::Component::Ptr pyo_comp = _selected_handle->component.Get<pyo::Component>("pyo");

				win->node.Add(ax::shared<MenuSeparator>(ax::Rect(att_pos, separator_size), "Pyo"));

				att_pos.y += separator_size.h;

				std::string fct_name = pyo_comp->GetFunctionName();

				auto menu = ax::shared<at::inspector::MenuAttribute>(
					ax::Rect(att_pos, att_size), "callback", fct_name, GetOnPyoCallback());
				win->node.Add(menu);

				att_pos.y += att_size.h;
			}
		}
		win->Update();
	}

	void InspectorMenu::RemoveHandle()
	{
		ax::App::GetInstance().GetWindowManager()->UnGrabKey();

		if (_selected_handle != nullptr) {
			win->node.GetChildren().clear();
		}
		_selected_handle = nullptr;

		win->Update();
	}

	void InspectorMenu::OnPyoCallback(const ax::event::SimpleMsg<std::pair<std::string, std::string>>& msg)
	{
		ax::console::Print("Pyocallback");
		if (_selected_handle == nullptr) {
			return;
		}

		if (msg.GetMsg().first == "callback") {
			if (!_selected_handle->component.Has("pyo")) {
				return;
			}

			/// @todo Check string before.
			pyo::Component::Ptr pyo_comp = _selected_handle->component.Get<pyo::Component>("pyo");
			pyo_comp->SetFunctionName(msg.GetMsg().second);
			return;
		}
	}

	void InspectorMenu::OnUniqueName(const ax::event::SimpleMsg<std::pair<std::string, std::string>>& msg)
	{
		ax::console::Print("Unique name change");
		if (_selected_handle == nullptr) {
			return;
		}

		if (msg.GetMsg().first == "name") {
			if (!_selected_handle->component.Has("unique_name")) {
				return;
			}

			/// @todo Check string before.
			at::UniqueNameComponent::Ptr comp
				= _selected_handle->component.Get<at::UniqueNameComponent>("unique_name");
			comp->SetName(msg.GetMsg().second);
			return;
		}
	}

	void InspectorMenu::OnWidgetUpdate(const ax::event::SimpleMsg<std::pair<std::string, std::string>>& msg)
	{
		ax::console::Print("Pyocallback");
		if (_selected_handle == nullptr) {
			return;
		}

		if (_selected_handle->component.Has("Widget")) {
			ax::widget::Component::Ptr widget
				= _selected_handle->component.Get<ax::widget::Component>("Widget");

			widget->SetBuilderAttributes(std::vector<std::pair<std::string, std::string>>{ msg.GetMsg() });
		}
	}

	void InspectorMenu::OnInfoUpdate(const ax::event::SimpleMsg<std::pair<std::string, std::string>>& msg)
	{
		ax::console::Print("Pyocallback");
		if (_selected_handle == nullptr) {
			return;
		}
		ax::widget::Component::Ptr widget = _selected_handle->component.Get<ax::widget::Component>("Widget");

		widget->SetInfo(std::vector<std::pair<std::string, std::string>>{ msg.GetMsg() });
		widget->ReloadInfo();
	}

	void InspectorMenu::SetMultipleWidgetSelected(bool on)
	{
		if (_has_multiple_widget_selected != on) {
			_has_multiple_widget_selected = on;

			if (on) {
				RemoveHandle();
			}

			win->Update();
		}
	}

	void InspectorMenu::OnPaint(ax::GC gc)
	{
		ax::Rect rect(win->dimension.GetDrawingRect());

		gc.SetColor(ax::Color(255, 255, 255));
		gc.DrawRectangle(rect);

		gc.SetColor(ax::Color(0.3));

		if (_has_multiple_widget_selected) {
			gc.DrawString(_font_bold, "Multiple widgets selected.", ax::Point(15, 20));
		}
		else {
			// No widget selected mode.
			gc.DrawString(_font_bold, "No widget selected.", ax::Point(15, 20));
			gc.DrawString(_font, "Command + click over a widget on the", ax::Point(15, 40));
			gc.DrawString(_font, "grid window to select a widget.", ax::Point(15, 52));
		}

		gc.SetColor(ax::Color(0.7));
		gc.DrawRectangleContour(rect);
	}
}
}
