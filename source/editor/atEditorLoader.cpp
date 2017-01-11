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

#include "editor/atEditorLoader.hpp"
#include "PyoAudio.h"
#include "atCommon.hpp"
#include "atSkin.hpp"
#include "atUniqueNameComponent.h"
#include "atWindowEventsComponent.hpp"
#include "editor/atEditor.hpp"
#include "editor/atEditorMainWindow.hpp"
#include "python/PyUtils.hpp"
#include "python/PyoComponent.hpp"

#include <axlib/Button.hpp>
#include <axlib/Core.hpp>
#include <axlib/DropMenu.hpp>
#include <axlib/Knob.hpp>
#include <axlib/Label.hpp>
#include <axlib/NumberBox.hpp>
#include <axlib/Panel.hpp>
#include <axlib/Slider.hpp>
#include <axlib/Toggle.hpp>
#include <axlib/WidgetLoader.hpp>
#include <axlib/WindowManager.hpp>
#include <axlib/axlib.hpp>

namespace at {
namespace editor {
	Loader::Loader(ax::Window* win)
		: _win(win)
	{
	}

	std::string Loader::OpenLayoutFromXml(ax::Xml& xml)
	{
		ax::Xml::Node top_node = xml.GetNode("Layout");

		if (!top_node.IsValid()) {
			ax::console::Error("Loader not layout node.");
			return "";
		}

		std::string script_path;

		try {
			script_path = top_node.GetAttribute("script");
		}
		catch (ax::Xml::Exception& err) {
			//						ax::console::Error("No pyo node.", err.what());
		}

		ax::Xml::Node node = top_node.GetFirstNode();
		ax::widget::Loader* loader = ax::widget::Loader::GetInstance();

		auto panel_builder = loader->GetBuilder("Panel");
		panel_builder->SetCreateCallback([&](ax::Window* win, ax::Xml::Node& node) {
			std::string builder_name = node.GetAttribute("builder");
			std::string pyo_fct_name;

			ax::Xml::Node pyo_node = node.GetNode("pyo");

			if (pyo_node.IsValid()) {
				pyo_fct_name = pyo_node.GetValue();
			}

			std::string unique_name;
			ax::Xml::Node unique_name_node = node.GetNode(at::component::UNIQUE_NAME);

			if (unique_name_node.IsValid()) {
				unique_name = unique_name_node.GetValue();
			}

			std::vector<std::pair<std::string, std::string>> window_evts_fcts
				= at::WindowEventsComponent::ParseValuesFromWidgetNode(node);

			SetupExistingWidget(win, builder_name, pyo_fct_name, unique_name, "", window_evts_fcts);
		});

		try {
			while (node.IsValid()) {
				const std::string node_name = node.GetName();

				if (node_name == "Widget") {
					std::string buider_name = node.GetAttribute("builder");
					std::string pyo_fct_name;

					ax::Xml::Node pyo_node = node.GetNode("pyo");

					if (pyo_node.IsValid()) {
						pyo_fct_name = pyo_node.GetValue();
					}

					std::string unique_name;
					ax::Xml::Node unique_name_node = node.GetNode(at::component::UNIQUE_NAME);

					if (unique_name_node.IsValid()) {
						unique_name = unique_name_node.GetValue();
					}

					std::string class_name;
					ax::Xml::Node class_name_node = node.GetNode(at::component::CLASS_NAME);

					if (class_name_node.IsValid()) {
						class_name = class_name_node.GetValue();
					}

					std::vector<std::pair<std::string, std::string>> window_evts_fcts
						= at::WindowEventsComponent::ParseValuesFromWidgetNode(node);

					ax::widget::Builder* builder = loader->GetBuilder(buider_name);

					if (builder == nullptr) {
						ax::console::Error("Builder", buider_name, "doesn't exist.");
						node = node.GetNextSibling();
						continue;
					}

					auto obj(builder->Create(node));
					_win->node.Add(obj);
					SetupExistingWidget(obj->GetWindow(), buider_name, pyo_fct_name, unique_name, class_name,
						window_evts_fcts);
				}

				node = node.GetNextSibling();
			}
		}
#warning("Catch this.")
		//		catch (rapidxml::parse_error& err) {
		//			ax::console::Error("Widget menu xml", err.what());
		//		}
		catch (ax::Xml::Exception& err) {
			ax::console::Error("Widget menu xml", err.what());
		}

		return script_path;
	}

	std::string Loader::OpenLayoutContent(const std::string& content, bool clear)
	{
		if (content.empty()) {
			return "";
		}

		if (clear) {
			_win->node.GetChildren().clear();
		}

		ax::Xml xml;

		if (!xml.Parse(content)) {
			ax::console::Error("parsing widget menu.");
			return "";
		}

		return OpenLayoutFromXml(xml);
	}

	std::string Loader::OpenLayout(const std::string& path, bool clear)
	{
		if (path.empty()) {
			return "";
		}

		if (clear) {
			_win->node.GetChildren().clear();
		}

		ax::Xml xml(path.c_str());

		if (!xml.Parse()) {
			ax::console::Error("parsing widget menu.");
			return "";
		}

		return OpenLayoutFromXml(xml);
	}

	void Loader::SetupExistingWidget(ax::Window* widget, const std::string& builder_name,
		const std::string& pyo_fct, const std::string& unique_name, const std::string& class_name,
		const std::vector<std::pair<std::string, std::string>>& window_events)
	{
		/// @todo Do this dynamically (with a map or something). Ready for widget plugins.

		if (builder_name == "Button") {
			widget->property.AddProperty("Resizable");
			SetupEditWidget(widget);
			SetupPyoComponent(widget, pyo_fct);
			SetupButtonPyoEvent(widget);
			SetupUniqueNameComponent(widget, unique_name);
			SetupClassNameComponent(widget, class_name);
		}
		else if (builder_name == "Toggle") {
			widget->property.AddProperty("Resizable");
			SetupEditWidget(widget);
			SetupPyoComponent(widget, pyo_fct);
			SetupTogglePyoEvent(widget);
			SetupUniqueNameComponent(widget, unique_name);
			SetupClassNameComponent(widget, class_name);
		}
		else if (builder_name == "Panel") {
			widget->property.AddProperty("Resizable");
			SetupEditWidget(widget);

			// Needs to be called after SetupEditWidget.
			at::WindowEventsComponent::SetupWindowEventsWidget(widget);

			// Add MainWindow property.
			ax::Panel* panel = static_cast<ax::Panel*>(widget->backbone.get());
			if (panel->GetName() == "MainWindow") {
				widget->property.AddProperty("MainWindow");
			}

			widget->property.AddProperty("BlockDrawing");
			SetupUniqueNameComponent(widget, unique_name);
			SetupClassNameComponent(widget, class_name);
			SetupWindowEventsComponent(widget, window_events);
		}
		else if (builder_name == "Knob") {
			widget->property.AddProperty("Resizable");
			SetupEditWidget(widget);
			SetupPyoComponent(widget, pyo_fct);
			SetupKnobPyoEvent(widget);
			SetupUniqueNameComponent(widget, unique_name);
			SetupClassNameComponent(widget, class_name);
		}
		else if (builder_name == "Slider") {
			widget->property.AddProperty("Resizable");
			SetupEditWidget(widget);
			SetupPyoComponent(widget, pyo_fct);
			SetupSliderPyoEvent(widget);
			SetupUniqueNameComponent(widget, unique_name);
			SetupClassNameComponent(widget, class_name);
		}
		else if (builder_name == "Label") {
			widget->property.AddProperty("Resizable");
			SetupEditWidget(widget);
			SetupUniqueNameComponent(widget, unique_name);
			SetupClassNameComponent(widget, class_name);
		}
		else if (builder_name == "NumberBox") {
			widget->property.AddProperty("Resizable");
			SetupEditWidget(widget);
			SetupPyoComponent(widget, pyo_fct);
			SetupNumberBoxPyoEvent(widget);
			SetupUniqueNameComponent(widget, unique_name);
			SetupClassNameComponent(widget, class_name);
		}
		else if (builder_name == "Sprite") {
			widget->property.AddProperty("Resizable");
			SetupEditWidget(widget);
			SetupUniqueNameComponent(widget, unique_name);
			SetupClassNameComponent(widget, class_name);
		}
	}

	void Loader::SetupEditWidget(ax::Window* win)
	{
		ax::Window* gwin = _win;

		// OnMouseLeftDown.
		auto m_down_fct = win->event.OnMouseLeftDown.GetFunction();
		win->event.OnMouseLeftDown = ax::WFunc<ax::Point>([gwin, win, m_down_fct](
			const ax::Point& pos) { AssignOnMouseLeftDown(gwin, win, m_down_fct, pos); });

		// OnMouseLeftDragging.
		auto m_drag_fct = win->event.OnMouseLeftDragging.GetFunction();
		win->event.OnMouseLeftDragging = ax::WFunc<ax::Point>([gwin, win, m_drag_fct](
			const ax::Point& pos) { AssignOnMouseLeftDragging(gwin, win, m_drag_fct, pos); });

		// OnMouseLeftUp.
		auto m_up_fct = win->event.OnMouseLeftUp.GetFunction();
		win->event.OnMouseLeftUp = ax::WFunc<ax::Point>(
			[gwin, win, m_up_fct](const ax::Point& pos) { AssignOnMouseLeftUp(gwin, win, m_up_fct, pos); });

		// OnMouseRightDown.
		auto m_right_down = win->event.OnMouseRightDown.GetFunction();
		win->event.OnMouseRightDown = ax::WFunc<ax::Point>([gwin, win, m_right_down](
			const ax::Point& pos) { AssignOnMouseRightDown(gwin, win, m_right_down, pos); });

		// OnMouseMotion.
		auto m_motion = win->event.OnMouseMotion.GetFunction();
		win->event.OnMouseMotion = ax::WFunc<ax::Point>(
			[gwin, win, m_motion](const ax::Point& pos) { AssignOnMouseMotion(gwin, win, m_motion, pos); });

		// OnMouseLeave event.
		auto m_leave = win->event.OnMouseLeave.GetFunction();
		win->event.OnMouseLeave = ax::WFunc<ax::Point>(
			[gwin, win, m_leave](const ax::Point& pos) { AssignOnMouseLeave(gwin, win, m_leave, pos); });

		// OnPaintOverChildren.
		win->event.OnPaintOverChildren = ax::WFunc<ax::GC>([win](ax::GC gc) {
			if (win->property.HasProperty("current_editing_widget")) {

				ax::Rect rect(win->dimension.GetDrawingRect());
				rect.position -= ax::Point(2, 2);
				rect.size += ax::Size(4, 4);

				const ax::Color color(at::Skin::GetInstance()->data.common_at_yellow);

				gc.SetColor(color, 0.7);
				gc.DrawRectangleContour(rect);

				gc.SetColor(color, 1.0);
				gc.DrawRectangleContour(rect.GetInteriorRect(ax::Point(1, 1)));
			}
		});
	}

	void Loader::SetupPyoComponent(ax::Window* win, const std::string& fct_name)
	{
		auto comp = pyo::Component::Ptr(new pyo::Component(win));
		comp->SetFunctionName(fct_name);
		win->component.Add("pyo", comp);
	}

	void Loader::SetupUniqueNameComponent(ax::Window* win, const std::string& name)
	{
		win->component.Add(at::component::UNIQUE_NAME, std::make_shared<at::UniqueNameComponent>(win, name));
	}

	void Loader::SetupClassNameComponent(ax::Window* win, const std::string& name)
	{
		win->component.Add(at::component::CLASS_NAME, std::make_shared<at::UniqueNameComponent>(win, name));
	}

	void Loader::SetupWindowEventsComponent(
		ax::Window* win, const std::vector<std::pair<std::string, std::string>>& window_events)
	{
		auto comp = at::WindowEventsComponent::Ptr(new at::WindowEventsComponent(win));
		comp->SetFunctionsValue(window_events);
		win->component.Add(at::component::WINDOW_EVENTS, comp);
	}

	void Loader::SetupButtonPyoEvent(ax::Window* win)
	{
		win->AddConnection(ax::Button::Events::BUTTON_CLICK, ax::event::Function([win](ax::event::Msg* msg) {
			if (win->component.Has("pyo")) {
				pyo::Component::Ptr comp = win->component.Get<pyo::Component>("pyo");
				const std::string fct_name = comp->GetFunctionName();

				if (!fct_name.empty()) {
					ax::Button* btn = static_cast<ax::Button*>(win->backbone.get());
					ax::python::CallFuncStrParam(fct_name, btn->GetMsg());
				}
			}
		}));
	}

	void Loader::SetupTogglePyoEvent(ax::Window* win)
	{
		win->AddConnection(ax::Toggle::Events::BUTTON_CLICK, ax::event::Function([win](ax::event::Msg* msg) {
			if (win->component.Has("pyo")) {
				pyo::Component::Ptr comp = win->component.Get<pyo::Component>("pyo");
				const std::string fct_name = comp->GetFunctionName();

				if (!fct_name.empty()) {
					ax::python::CallFuncNoParam(fct_name);
				}
			}
		}));
	}

	void Loader::SetupKnobPyoEvent(ax::Window* win)
	{
		win->AddConnection(0, ax::event::Function([win](ax::event::Msg* msg) {
			if (win->component.Has("pyo")) {
				pyo::Component::Ptr comp = win->component.Get<pyo::Component>("pyo");
				const std::string fct_name = comp->GetFunctionName();

				if (!fct_name.empty()) {
					ax::Knob::Msg* kmsg = static_cast<ax::Knob::Msg*>(msg);
					ax::python::CallFuncRealParam(fct_name, kmsg->GetValue());
				}
			}
		}));
	}

	void Loader::SetupSliderPyoEvent(ax::Window* win)
	{
		win->AddConnection(0, ax::event::Function([win](ax::event::Msg* msg) {
			if (win->component.Has("pyo")) {
				pyo::Component::Ptr comp = win->component.Get<pyo::Component>("pyo");
				const std::string fct_name = comp->GetFunctionName();

				if (!fct_name.empty()) {
					ax::Slider::Msg* kmsg = static_cast<ax::Slider::Msg*>(msg);
					ax::python::CallFuncRealParam(fct_name, 1.0 - kmsg->GetValue());
				}
			}
		}));
	}

	void Loader::SetupNumberBoxPyoEvent(ax::Window* win)
	{
		win->AddConnection(0, ax::event::Function([win](ax::event::Msg* msg) {
			if (win->component.Has("pyo")) {
				pyo::Component::Ptr comp = win->component.Get<pyo::Component>("pyo");
				const std::string fct_name = comp->GetFunctionName();

				if (!fct_name.empty()) {
					ax::NumberBox::Msg* kmsg = static_cast<ax::NumberBox::Msg*>(msg);
					ax::python::CallFuncRealParam(fct_name, kmsg->GetValue());
				}
			}
		}));
	}

	void Loader::AssignOnMouseLeftDown(
		ax::Window* gwin, ax::Window* win, std::function<void(ax::Point)> fct, const ax::Point& pos)
	{
		ax::Point c_delta(pos - win->dimension.GetAbsoluteRect().position);

		bool cmd_down = ax::App::GetInstance().GetWindowManager()->IsCmdDown();

		if (cmd_down) {
			win->resource.Add("click_delta", c_delta);
			win->event.GrabMouse();
			win->property.AddProperty("edit_click");

			gwin->PushEvent(
				at::editor::GridWindow::SELECT_WIDGET, new ax::event::SimpleMsg<ax::Window*>(win));

			return;
		}

		if (win->property.HasProperty("current_editing_widget") && win->property.HasProperty("Resizable")) {

			bool top = c_delta.y < 4;
			bool bottom = c_delta.y > win->dimension.GetShownRect().size.h - 4;
			bool right = c_delta.x > win->dimension.GetShownRect().size.w - 4;
			bool left = c_delta.x < 4;

			if (right && bottom) {
				win->property.AddProperty("ResizeBottomRight");
			}
			else if (right && top) {
				win->property.AddProperty("ResizeTopRight");
			}
			else if (left && top) {
				win->property.AddProperty("ResizeTopLeft");
			}
			else if (left && bottom) {
				win->property.AddProperty("ResizeBottomLeft");
			}
			else if (right) {
				win->property.AddProperty("ResizeRight");
			}
			else if (bottom) {
				win->property.AddProperty("ResizeBottom");
			}
			else if (left) {
				win->property.AddProperty("ResizeLeft");
			}
			else if (top) {
				win->property.AddProperty("ResizeTop");
			}

			win->resource.Add("click_delta", c_delta);
			win->event.GrabMouse();
			win->property.AddProperty("edit_click");
			gwin->PushEvent(
				at::editor::GridWindow::SELECT_WIDGET, new ax::event::SimpleMsg<ax::Window*>(win));
		}
		else {
			if (fct) {
				fct(pos);
			}
		}
	}

	void Loader::AssignOnMouseLeftDragging(
		ax::Window* gwin, ax::Window* win, std::function<void(ax::Point)> fct, const ax::Point& position)
	{
		// Editing.
		if (win->property.HasProperty("edit_click")) {
			ax::Point pos = position;

			at::editor::GridSnapProxy gsp
				= at::editor::App::GetInstance()->GetMainWindow()->GetGridSnapProxy();

			if (gsp.IsSnapActive()) {
				const ax::Point gw_abs_pos(gwin->dimension.GetAbsoluteRect().position);
				pos = gsp.FindClosestPosition(pos - gw_abs_pos) + gw_abs_pos;
			}

			if (win->event.IsGrabbed()) {

				ax::Point c_delta = win->resource.GetResource("click_delta");

				// Right resize.
				if (win->property.HasProperty("ResizeRight")) {
					int size_y = win->dimension.GetSize().h;
					int size_x = pos.x - win->dimension.GetAbsoluteRect().position.x;
					win->dimension.SetSize(ax::Size(size_x, size_y));

					gwin->PushEvent(at::editor::GridWindow::WIDGET_RESIZE, new ax::event::EmptyMsg());
				}
				else if (win->property.HasProperty("ResizeBottomRight")) {
					int size_y = pos.y - win->dimension.GetAbsoluteRect().position.y;
					int size_x = pos.x - win->dimension.GetAbsoluteRect().position.x;
					win->dimension.SetSize(ax::Size(size_x, size_y));

					gwin->PushEvent(at::editor::GridWindow::WIDGET_RESIZE, new ax::event::EmptyMsg());
				}
				else if (win->property.HasProperty("ResizeTopRight")) {
					ax::Rect abs_rect(win->dimension.GetAbsoluteRect());
					int size_x = pos.x - win->dimension.GetAbsoluteRect().position.x;
					int size_y = abs_rect.position.y + abs_rect.size.h - pos.y;
					int pos_y = pos.y - win->node.GetParent()->dimension.GetAbsoluteRect().position.y;
					int pos_x = win->dimension.GetRect().position.x;
					win->dimension.SetRect(ax::Rect(pos_x, pos_y, size_x, size_y));

					ax::Point dd(0, abs_rect.position.y - pos.y);
					std::vector<std::shared_ptr<ax::Window>>& children = win->node.GetChildren();
					for (auto& n : children) {
						ax::Point w_pos = n->dimension.GetRect().position;
						n->dimension.SetPosition(w_pos + dd);
					}

					gwin->PushEvent(at::editor::GridWindow::WIDGET_RESIZE, new ax::event::EmptyMsg());
				}
				// Bottom resize.
				else if (win->property.HasProperty("ResizeBottom")) {
					int size_x = win->dimension.GetSize().w;
					int size_y = pos.y - win->dimension.GetAbsoluteRect().position.y;
					win->dimension.SetSize(ax::Size(size_x, size_y));

					gwin->PushEvent(at::editor::GridWindow::WIDGET_RESIZE, new ax::event::EmptyMsg());
				}
				// Left resize.
				else if (win->property.HasProperty("ResizeLeft")) {
					ax::Rect abs_rect(win->dimension.GetAbsoluteRect());
					int size_x = abs_rect.position.x + abs_rect.size.w - pos.x;
					int size_y = abs_rect.size.h;
					int pos_y = win->dimension.GetRect().position.y;
					int pos_x = pos.x - win->node.GetParent()->dimension.GetAbsoluteRect().position.x;
					win->dimension.SetRect(ax::Rect(pos_x, pos_y, size_x, size_y));

					ax::Point dd(abs_rect.position.x - pos.x, 0);
					std::vector<std::shared_ptr<ax::Window>>& children = win->node.GetChildren();
					for (auto& n : children) {
						ax::Point w_pos = n->dimension.GetRect().position;
						n->dimension.SetPosition(w_pos + dd);
					}

					gwin->PushEvent(at::editor::GridWindow::WIDGET_RESIZE, new ax::event::EmptyMsg());
				}
				else if (win->property.HasProperty("ResizeBottomLeft")) {
					ax::Rect abs_rect(win->dimension.GetAbsoluteRect());
					int size_x = abs_rect.position.x + abs_rect.size.w - pos.x;
					int size_y = pos.y - win->dimension.GetAbsoluteRect().position.y;
					int pos_y = win->dimension.GetRect().position.y;
					int pos_x = pos.x - win->node.GetParent()->dimension.GetAbsoluteRect().position.x;
					win->dimension.SetRect(ax::Rect(pos_x, pos_y, size_x, size_y));

					ax::Point dd(abs_rect.position.x - pos.x, 0);
					std::vector<std::shared_ptr<ax::Window>>& children = win->node.GetChildren();
					for (auto& n : children) {
						ax::Point w_pos = n->dimension.GetRect().position;
						n->dimension.SetPosition(w_pos + dd);
					}

					gwin->PushEvent(at::editor::GridWindow::WIDGET_RESIZE, new ax::event::EmptyMsg());
				}
				// Top resize.
				else if (win->property.HasProperty("ResizeTop")) {
					ax::Rect abs_rect(win->dimension.GetAbsoluteRect());
					int size_x = abs_rect.size.w;
					int size_y = abs_rect.position.y + abs_rect.size.h - pos.y;
					int pos_y = pos.y - win->node.GetParent()->dimension.GetAbsoluteRect().position.y;
					int pos_x = win->dimension.GetRect().position.x;
					win->dimension.SetRect(ax::Rect(pos_x, pos_y, size_x, size_y));

					ax::Point dd(0, abs_rect.position.y - pos.y);
					std::vector<std::shared_ptr<ax::Window>>& children = win->node.GetChildren();
					for (auto& n : children) {
						ax::Point w_pos = n->dimension.GetRect().position;
						n->dimension.SetPosition(w_pos + dd);
					}

					gwin->PushEvent(at::editor::GridWindow::WIDGET_RESIZE, new ax::event::EmptyMsg());
				}
				else if (win->property.HasProperty("ResizeTopLeft")) {
					ax::Rect abs_rect(win->dimension.GetAbsoluteRect());
					int size_x = abs_rect.position.x + abs_rect.size.w - pos.x;
					int size_y = abs_rect.position.y + abs_rect.size.h - pos.y;
					int pos_y = pos.y - win->node.GetParent()->dimension.GetAbsoluteRect().position.y;
					int pos_x = pos.x - win->node.GetParent()->dimension.GetAbsoluteRect().position.x;
					win->dimension.SetRect(ax::Rect(pos_x, pos_y, size_x, size_y));

					ax::Point dd(abs_rect.position.x - pos.x, abs_rect.position.y - pos.y);
					std::vector<std::shared_ptr<ax::Window>>& children = win->node.GetChildren();
					for (auto& n : children) {
						ax::Point w_pos = n->dimension.GetRect().position;
						n->dimension.SetPosition(w_pos + dd);
					}

					gwin->PushEvent(at::editor::GridWindow::WIDGET_RESIZE, new ax::event::EmptyMsg());
				}
				// Moving widget.
				else {
					ax::Point w_position
						= position - win->node.GetParent()->dimension.GetAbsoluteRect().position - c_delta;

					if (gsp.IsSnapActive()) {
						const ax::Point gw_abs_pos(gwin->dimension.GetAbsoluteRect().position);
						w_position = gsp.FindClosestPosition(position - c_delta - gw_abs_pos) + gw_abs_pos
							- win->node.GetParent()->dimension.GetAbsoluteRect().position;
					}

					win->dimension.SetPosition(w_position);

					if (!win->property.HasProperty("first_time_dragging")) {
						win->property.AddProperty("first_time_dragging");
						gwin->PushEvent(
							at::editor::GridWindow::BEGIN_DRAGGING_WIDGET, new ax::event::EmptyMsg());
					}
					else {
						gwin->PushEvent(at::editor::GridWindow::DRAGGING_WIDGET, new ax::event::EmptyMsg());
					}
				}
			}
		}

		else {
			// Call widget callback.
			if (fct) {
				fct(position);
			}
		}
	}

	void Loader::AssignOnMouseLeftUp(
		ax::Window* gwin, ax::Window* win, std::function<void(ax::Point)> fct, const ax::Point& pos)
	{
		// Editing.
		if (win->property.HasProperty("edit_click")) {
			win->property.RemoveProperty("edit_click");
			win->property.RemoveProperty("ResizeLeft");
			win->property.RemoveProperty("ResizeRight");
			win->property.RemoveProperty("ResizeBottom");
			win->property.RemoveProperty("ResizeTop");

			win->property.RemoveProperty("ResizeTopLeft");
			win->property.RemoveProperty("ResizeTopRight");
			win->property.RemoveProperty("ResizeBottomLeft");
			win->property.RemoveProperty("ResizeBottomRight");

			win->property.RemoveProperty("first_time_dragging");

			if (win->event.IsGrabbed()) {
				win->event.UnGrabMouse();
			}

			gwin->PushEvent(at::editor::GridWindow::DONE_DRAGGING_WIDGET, new ax::event::EmptyMsg());
		}

		// Call widget callback.
		else {
			if (fct) {
				fct(pos);
			}
		}
	}

	void Loader::AssignOnMouseMotion(
		ax::Window* gwin, ax::Window* win, std::function<void(ax::Point)> fct, const ax::Point& pos)
	{
		if (win->property.HasProperty("current_editing_widget")) {
			const ax::Point c_delta(pos - win->dimension.GetAbsoluteRect().position);
			ax::App& app(ax::App::GetInstance());

			if (app.GetWindowManager()->IsCmdDown()) {
				app.GetCore()->SetCursor(ax::core::Core::Cursor::NORMAL);
				return;
			}

			if (win->property.HasProperty("Resizable")) {
				bool top = c_delta.y < 4;
				bool bottom = c_delta.y > win->dimension.GetShownRect().size.h - 4;
				bool right = c_delta.x > win->dimension.GetShownRect().size.w - 4;
				bool left = c_delta.x < 4;

				if ((right && bottom) || (top && left)) {
					app.GetCore()->SetCursor(ax::core::Core::Cursor::RESIZE_TOP_LEFT_DOWN_RIGHT);
				}
				else if ((bottom && left) || (top && right)) {
					app.GetCore()->SetCursor(ax::core::Core::Cursor::RESIZE_BOTTOM_LEFT_TOP_RIGHT);
				}
				else if (right || left) {
					app.GetCore()->SetCursor(ax::core::Core::Cursor::RESIZE_LEFT_RIGHT);
				}
				else if (bottom || top) {
					app.GetCore()->SetCursor(ax::core::Core::Cursor::RESIZE_UP_DOWN);
				}
				else {
					app.GetCore()->SetCursor(ax::core::Core::Cursor::MOVE);
				}
			}
		}
		else {
			if (fct) {
				fct(pos);
			}
		}
	}

	void Loader::AssignOnMouseRightDown(
		ax::Window* gwin, ax::Window* win, std::function<void(ax::Point)> fct, const ax::Point& pos)
	{
		if (win->property.HasProperty("current_editing_widget")) {
			win->property.AddProperty("edit_click");

			ax::App::GetInstance().GetCore()->SetCursor(ax::core::Core::Cursor::NORMAL);

			gwin->PushEvent(at::editor::GridWindow::DROP_WIDGET_MENU,
				new ax::event::SimpleMsg<std::pair<ax::Point, ax::Window*>>(
					std::pair<ax::Point, ax::Window*>(pos, win)));

			return;
		}

		// Call widget callback.
		if (fct) {
			fct(pos);
		}
	}

	void Loader::AssignOnMouseLeave(
		ax::Window* gwin, ax::Window* win, std::function<void(ax::Point)> fct, const ax::Point& pos)
	{
		if (win->property.HasProperty("edit_click")) {
			//				ax::console::Print("Mouse leave has -> edit click");
		}
		else {
			// Set normal cursor.
			ax::App::GetInstance().GetCore()->SetCursor(ax::core::Core::Cursor::NORMAL);

			if (fct) {
				fct(pos);
			}
		}
	}
}
}
