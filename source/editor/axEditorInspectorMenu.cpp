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

#include "atCommon.hpp"
#include "atUniqueNameComponent.h"
#include "atWindowEventsComponent.hpp"
#include "editor/atEditorInspectorMenu.hpp"
#include "menu/attribute/atMenuAttribute.hpp"
#include "menu/attribute/atMenuBoolAttribute.hpp"
#include "menu/attribute/atMenuColorAttribute.hpp"
#include "menu/attribute/atMenuIntegerAttribute.hpp"
#include "menu/attribute/atMenuPathAttribute.hpp"
#include "menu/attribute/atMenuPointAttribute.hpp"
#include "menu/attribute/atMenuRangeAttribute.hpp"
#include "menu/attribute/atMenuSizeAttribute.hpp"
#include "python/PyoComponent.hpp"

#include <axlib/WindowManager.hpp>
#include <fst/print.h>

#include "editor/atEditor.hpp"
#include "editor/atEditorGridWindow.hpp"
#include "editor/atEditorMainWindow.hpp"

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

	// Get Attribute builder function.
	template <typename T>
	inline std::pair<ax::widget::ParamType,
		std::function<std::shared_ptr<ax::Window::Backbone>(const ax::Rect& rect, const std::string& name,
			const std::string& value, ax::event::Function fct)>>
	CreateBuilderPair(ax::widget::ParamType type)
	{
		return std::make_pair(
			type, [](const ax::Rect& rect, const std::string& name, const std::string& value,
					  ax::event::Function fct) { return std::make_shared<T>(rect, name, value, fct); });
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

		{
			using namespace inspector;
			_att_builder_map.insert(CreateBuilderPair<ColorAttribute>(ax::widget::ParamType::COLOR));
			_att_builder_map.insert(CreateBuilderPair<BoolAttribute>(ax::widget::ParamType::BOOLEAN));
			_att_builder_map.insert(CreateBuilderPair<PointAttribute>(ax::widget::ParamType::POINT));
			_att_builder_map.insert(CreateBuilderPair<SizeAttribute>(ax::widget::ParamType::SIZE));
			_att_builder_map.insert(CreateBuilderPair<RangeAttribute>(ax::widget::ParamType::RANGE));
			_att_builder_map.insert(CreateBuilderPair<IntegerAttribute>(ax::widget::ParamType::INTEGER));
			_att_builder_map.insert(CreateBuilderPair<PathAttribute>(ax::widget::ParamType::FILEPATH));
		}
	}

	void InspectorMenu::SetWidgetHandle(ax::Window* handle)
	{
		if (!_has_grid_window_connection) {
			_has_grid_window_connection = true;
			ax::Window* gw = editor::App::GetInstance()->GetMainWindow()->GetGridWindow()->GetWindow();
			gw->AddConnection(GridWindow::DRAGGING_WIDGET, GetOnDraggingWidget());
			gw->AddConnection(GridWindow::WIDGET_RESIZE, GetOnWidgetResize());
			gw->AddConnection(GridWindow::ARROW_MOVE_SELECTED_WIDGET, GetOnArrowMoveSelectedWidget());
		}

		fst::print(ptrace);

		// Clear old content.
		RemoveHandle();
		_selected_handle = handle;

		if (!_selected_handle) {
			win->Update();
			return;
		}

		fst::print(ptrace, "Has selected handle");
		const ax::Rect rect(win->dimension.GetRect());
		const ax::Size separator_size(rect.size.w, 20);
		const ax::Size att_size(rect.size.w, 20);

		ax::Point att_pos(0, 0);

		win->node.Add(ax::shared<MenuSeparator>(ax::Rect(att_pos, separator_size), "Node"));
		att_pos.y += separator_size.h;

		// Unique name attributes.
		if (_selected_handle->component.Has(at::component::UNIQUE_NAME)) {
			at::UniqueNameComponent::Ptr comp
				= _selected_handle->component.Get<at::UniqueNameComponent>(at::component::UNIQUE_NAME);

			auto menu = ax::shared<at::inspector::MenuAttribute>(
				ax::Rect(att_pos, att_size), "unique_name", comp->GetName(), GetOnUniqueName());

			win->node.Add(menu);
			att_pos.y += att_size.h;
		}

		// Class name attributes.
		if (_selected_handle->component.Has(at::component::CLASS_NAME)) {
			at::UniqueNameComponent::Ptr comp
				= _selected_handle->component.Get<at::UniqueNameComponent>(at::component::CLASS_NAME);

			auto menu = ax::shared<at::inspector::MenuAttribute>(
				ax::Rect(att_pos, att_size), "class", comp->GetName(), GetOnClassName());

			win->node.Add(menu);
			att_pos.y += att_size.h;
		}

		// Add widget separator.
		win->node.Add(ax::shared<MenuSeparator>(ax::Rect(att_pos, separator_size), "Widget"));
		att_pos.y += separator_size.h;

		ax::widget::Component::Ptr widget = _selected_handle->component.Get<ax::widget::Component>("Widget");

		// Builder attributes.
		std::vector<std::pair<std::string, std::string>> atts_pair = widget->GetBuilderAttributes();
		fst::print("Builder attributes size :", (int)atts_pair.size());

		std::map<std::string, std::string> atts_map;

		for (auto& n : atts_pair) {
			atts_map.insert(n);
			fst::print("att pair :", n.first, n.second);
		}

		std::vector<ax::widget::ParamInfo> builder_atts_info = widget->GetBuilderAttributesInfo();

		for (auto& n : builder_atts_info) {
			fst::print("widget param type :", (int)n.first);
			std::string value = atts_map[n.second];

			auto it = _att_builder_map.find(n.first);

			if (it != _att_builder_map.end()) {
				auto bb = it->second(ax::Rect(att_pos, att_size), n.second, value, GetOnWidgetUpdate());
				if (n.second == "position") {
					_widget_build_pos_att = bb.get();
				}
				else if (n.second == "size") {
					_widget_build_size_att = bb.get();
				}

				win->node.Add(bb);
			}
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

			auto it = _att_builder_map.find(n.first);

			if (it != _att_builder_map.end()) {
				win->node.Add(it->second(ax::Rect(att_pos, att_size), n.second, value, GetOnInfoUpdate()));
			}
			else {
				win->node.Add(ax::shared<at::inspector::MenuAttribute>(
					ax::Rect(att_pos, att_size), n.second, value, GetOnWidgetUpdate()));
			}

			att_pos.y += att_size.h;
		}

		// Python attributes.
		if (_selected_handle->component.Has("pyo")) {
			pyo::Component::Ptr pyo_comp = _selected_handle->component.Get<pyo::Component>("pyo");

			win->node.Add(ax::shared<MenuSeparator>(ax::Rect(att_pos, separator_size), "Pyo"));

			att_pos.y += separator_size.h;

			auto menu = ax::shared<at::inspector::MenuAttribute>(
				ax::Rect(att_pos, att_size), "callback", pyo_comp->GetFunctionName(), GetOnPyoCallback());
			win->node.Add(menu);

			att_pos.y += att_size.h;
		}

		// WindowEvents attributes.
		if (_selected_handle->component.Has(at::component::WINDOW_EVENTS)) {
			win->node.Add(ax::shared<MenuSeparator>(ax::Rect(att_pos, separator_size), "Window Events"));
			att_pos.y += separator_size.h;

			auto comp
				= _selected_handle->component.Get<at::WindowEventsComponent>(at::component::WINDOW_EVENTS);

			const std::vector<std::pair<std::string, std::string>> w_evt_fcts = comp->GetFunctionsValue();

			for (auto& n : w_evt_fcts) {
				win->node.Add(std::make_shared<at::inspector::MenuAttribute>(
					ax::Rect(att_pos, att_size), n.first, n.second, GetOnWindowEvents(), 10));
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
		_widget_build_pos_att = nullptr;
		_widget_build_size_att = nullptr;

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

		if (msg.GetMsg().first == "unique_name") {
			if (!_selected_handle->component.Has(at::component::UNIQUE_NAME)) {
				return;
			}

			/// @todo Check string before.
			at::UniqueNameComponent::Ptr comp
				= _selected_handle->component.Get<at::UniqueNameComponent>(at::component::UNIQUE_NAME);
			comp->SetName(msg.GetMsg().second);
			return;
		}
	}

	void InspectorMenu::OnClassName(const ax::event::SimpleMsg<std::pair<std::string, std::string>>& msg)
	{
		if (_selected_handle == nullptr) {
			return;
		}

		if (msg.GetMsg().first == "class") {
			if (!_selected_handle->component.Has(at::component::CLASS_NAME)) {
				return;
			}

			/// @todo Check string before.
			at::UniqueNameComponent::Ptr comp
				= _selected_handle->component.Get<at::UniqueNameComponent>(at::component::CLASS_NAME);
			comp->SetName(msg.GetMsg().second);
			return;
		}
	}

	void InspectorMenu::OnWindowEvents(const ax::event::SimpleMsg<std::pair<std::string, std::string>>& msg)
	{
		ax::console::Print("WindowEvents change");
		if (_selected_handle == nullptr) {
			return;
		}

		if (!_selected_handle->component.Has(at::component::WINDOW_EVENTS)) {
			return;
		}

		_selected_handle->component.Get<at::WindowEventsComponent>(at::component::WINDOW_EVENTS)
			->SetFunctionValue(msg.GetMsg());
	}

	void InspectorMenu::OnWidgetUpdate(const ax::event::SimpleMsg<std::pair<std::string, std::string>>& msg)
	{
		if (_selected_handle == nullptr) {
			return;
		}

		if (_selected_handle->component.Has("Widget")) {
			ax::widget::Component::Ptr widget
				= _selected_handle->component.Get<ax::widget::Component>("Widget");

			// ax::console::Print("WidgetUpdate :", msg.GetMsg().first, msg.GetMsg().second);
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

	void InspectorMenu::OnDraggingWidget(const ax::event::EmptyMsg& msg)
	{
		if (_selected_handle == nullptr) {
			return;
		}

		if (_widget_build_pos_att == nullptr) {
			return;
		}

		if (!_selected_handle->component.Has("Widget")) {
			return;
		}

		at::inspector::PointAttribute* p_att
			= static_cast<at::inspector::PointAttribute*>(_widget_build_pos_att);
		ax::widget::Component::Ptr widget = _selected_handle->component.Get<ax::widget::Component>("Widget");
		const std::vector<std::pair<std::string, std::string>> atts = widget->GetBuilderAttributes();

		for (auto& n : atts) {
			if (n.first == "position") {
				p_att->SetValue(n.second);
			}
		}
	}

	void InspectorMenu::OnWidgetResize(const ax::event::EmptyMsg& msg)
	{
		if (_selected_handle == nullptr) {
			return;
		}

		if (_widget_build_pos_att == nullptr) {
			return;
		}

		if (!_selected_handle->component.Has("Widget")) {
			return;
		}

		at::inspector::SizeAttribute* s_att
			= static_cast<at::inspector::SizeAttribute*>(_widget_build_size_att);
		ax::widget::Component::Ptr widget = _selected_handle->component.Get<ax::widget::Component>("Widget");
		const std::vector<std::pair<std::string, std::string>> atts = widget->GetBuilderAttributes();

		for (auto& n : atts) {
			if (n.first == "size") {
				s_att->SetValue(n.second);
			}
		}
	}

	void InspectorMenu::OnArrowMoveSelectedWidget(const ax::event::SimpleMsg<ax::util::Direction>& msg)
	{
		if (_selected_handle == nullptr) {
			return;
		}

		if (_widget_build_pos_att == nullptr) {
			return;
		}

		if (!_selected_handle->component.Has("Widget")) {
			return;
		}

		at::inspector::PointAttribute* p_att
			= static_cast<at::inspector::PointAttribute*>(_widget_build_pos_att);
		ax::widget::Component::Ptr widget = _selected_handle->component.Get<ax::widget::Component>("Widget");
		const std::vector<std::pair<std::string, std::string>> atts = widget->GetBuilderAttributes();

		for (auto& n : atts) {
			if (n.first == "position") {
				p_att->SetValue(n.second);
			}
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
