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

#include "PyoAudio.h"
#include "PyoComponent.h"
#include "atEditor.h"
#include "atEditorLoader.h"
#include "atEditorMainWindow.h"

#include <OpenAX/Button.h>
#include <OpenAX/Knob.h>
#include <OpenAX/Label.h>
#include <OpenAX/Panel.h>
#include <OpenAX/Slider.h>
#include <OpenAX/Toggle.h>
#include <OpenAX/WidgetLoader.h>
#include <OpenAX/WindowManager.h>

namespace at {
namespace editor {
	Loader::Loader(ax::Window* win)
		: _win(win)
	{
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
			ax::Error("parsing widget menu.");
			return "";
		}

		ax::Xml::Node top_node = xml.GetNode("Layout");

		if (!top_node.IsValid()) {
			ax::Error("Loader not layout node.");
			return "";
		}

		std::string script_path;

		try {
			script_path = top_node.GetAttribute("script");
		}
		catch (ax::Xml::Exception& err) {
			//						ax::Error("No pyo node.", err.what());
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

			SetupExistingWidget(win, builder_name, pyo_fct_name);
		});

		try {
			while (node.IsValid()) {
				std::string node_name = node.GetName();
//				ax::Print("Node name :", node_name);

				if (node_name == "Widget") {
					std::string buider_name = node.GetAttribute("builder");
					std::string pyo_fct_name;

					ax::Xml::Node pyo_node = node.GetNode("pyo");

					if (pyo_node.IsValid()) {
						pyo_fct_name = pyo_node.GetValue();
					}

					ax::widget::Builder* builder = loader->GetBuilder(buider_name);

					if (builder == nullptr) {
						ax::Error("Builder", buider_name, "doesn't exist.");
						node = node.GetNextSibling();
						continue;
					}

					auto obj(builder->Create(node));
					_win->node.Add(obj);
					SetupExistingWidget(obj->GetWindow(), buider_name, pyo_fct_name);
				}

				node = node.GetNextSibling();
			}
		}
		catch (rapidxml::parse_error& err) {
			ax::Error("Widget menu xml", err.what());
		}
		catch (ax::Xml::Exception& err) {
			ax::Error("Widget menu xml", err.what());
		}

		return script_path;
	}

	void Loader::SetupExistingWidget(
		ax::Window* widget, const std::string& builder_name, const std::string& pyo_fct)
	{
		if (builder_name == "Button") {
			widget->property.AddProperty("Resizable");
			SetupEditWidget(widget);
			SetupPyoComponent(widget, pyo_fct);
			SetupButtonPyoEvent(widget);
		}
		else if (builder_name == "Toggle") {
			widget->property.AddProperty("Resizable");
			SetupEditWidget(widget);
			SetupPyoComponent(widget, pyo_fct);
			SetupTogglePyoEvent(widget);
		}
		else if (builder_name == "Panel") {
			widget->property.AddProperty("Resizable");
			SetupEditWidget(widget);

			// Add MainWindow property.
			ax::Panel* panel = static_cast<ax::Panel*>(widget->backbone.get());
			if (panel->GetName() == "MainWindow") {
				widget->property.AddProperty("MainWindow");
			}

			widget->property.AddProperty("BlockDrawing");
		}
		else if (builder_name == "Knob") {
			widget->property.AddProperty("Resizable");
			SetupEditWidget(widget);
			SetupPyoComponent(widget, pyo_fct);
			SetupKnobPyoEvent(widget);
		}
		else if (builder_name == "Slider") {
			widget->property.AddProperty("Resizable");
			SetupEditWidget(widget);
			SetupPyoComponent(widget, pyo_fct);
			SetupSliderPyoEvent(widget);
		}
		else if (builder_name == "Label") {
			widget->property.AddProperty("Resizable");
			SetupEditWidget(widget);
		}
	}

	void Loader::SetupEditWidget(ax::Window* win)
	{
		ax::Window* gwin = _win;
		
		auto m_down_fct = win->event.OnMouseLeftDown.GetFunction();
		win->event.OnMouseLeftDown = ax::WFunc<ax::Point>([gwin, win, m_down_fct](const ax::Point& pos) {
			bool cmd_down = ax::App::GetInstance().GetWindowManager()->IsCmdDown();

			if (cmd_down) {
				ax::Point c_delta(pos - win->dimension.GetAbsoluteRect().position);
				win->resource.Add("click_delta", c_delta);
				win->event.GrabMouse();
				win->property.AddProperty("edit_click");

				if (win->property.HasProperty("Resizable")) {
					
					if (c_delta.x > win->dimension.GetShownRect().size.x - 4) {
						win->property.AddProperty("ResizeRight");
					}
					else if (c_delta.y > win->dimension.GetShownRect().size.y - 4) {
						win->property.AddProperty("ResizeBottom");
					}
					else if (c_delta.x < 4) {
						win->property.AddProperty("ResizeLeft");
					}
				}

				/// @todo Change event id to enum.
				gwin->PushEvent(1234, new ax::Event::SimpleMsg<ax::Window*>(win));
			}
			
			// Call widget callback.
			else {
				if (m_down_fct) {
					m_down_fct(pos);
				}
			}
		});
		
		
		
		auto m_drag_fct = win->event.OnMouseLeftDragging.GetFunction();
		win->event.OnMouseLeftDragging = ax::WFunc<ax::Point>([win, m_drag_fct](const ax::Point& pos) {

			// Editing.
			if (win->property.HasProperty("edit_click")) {
				if (win->event.IsGrabbed()) {
					ax::Point c_delta = win->resource.GetResource("click_delta");

					// Right resize.
					if (win->property.HasProperty("ResizeRight")) {
						int size_y = win->dimension.GetSize().y;
						int size_x = pos.x - win->dimension.GetAbsoluteRect().position.x;
						win->dimension.SetSize(ax::Size(size_x, size_y));
					}
					// Bottom resize.
					else if (win->property.HasProperty("ResizeBottom")) {
						int size_x = win->dimension.GetSize().x;
						int size_y = pos.y - win->dimension.GetAbsoluteRect().position.y;
						win->dimension.SetSize(ax::Size(size_x, size_y));
					}
					// Left resize.
					else if (win->property.HasProperty("ResizeLeft")) {
						ax::Rect abs_rect(win->dimension.GetAbsoluteRect());
						int size_x = abs_rect.position.x + abs_rect.size.x - pos.x;
						int size_y = abs_rect.size.y;
						int pos_y = win->dimension.GetRect().position.y;
						int pos_x = pos.x - win->node.GetParent()->dimension.GetAbsoluteRect().position.x;
						win->dimension.SetSize(ax::Size(size_x, size_y));
						win->dimension.SetPosition(ax::Point(pos_x, pos_y));
					}
					// Moving widget.
					else {
						win->dimension.SetPosition(
							pos - win->node.GetParent()->dimension.GetAbsoluteRect().position - c_delta);
					}
				}
			}
			
			// Call widget callback.
			else {
				if (m_drag_fct) {
					m_drag_fct(pos);
				}
			}
		});

		auto m_up_fct = win->event.OnMouseLeftUp.GetFunction();
		win->event.OnMouseLeftUp = ax::WFunc<ax::Point>([win, m_up_fct](const ax::Point& pos) {

			// Editing.
			if (win->property.HasProperty("edit_click")) {

				win->property.RemoveProperty("edit_click");
				win->property.RemoveProperty("ResizeLeft");
				win->property.RemoveProperty("ResizeRight");
				win->property.RemoveProperty("ResizeBottom");

				if (win->event.IsGrabbed()) {
					win->event.UnGrabMouse();
				}
			}

			// Call widget callback.
			else {
				if (m_up_fct) {
					m_up_fct(pos);
				}
			}
		});

		auto m_right_down = win->event.OnMouseRightDown.GetFunction();
		win->event.OnMouseRightDown = ax::WFunc<ax::Point>([gwin, win, m_right_down](const ax::Point& pos) {

			if (ax::App::GetInstance().GetWindowManager()->IsCmdDown()) {
				
				win->Hide();
				
				ax::Window* parent = win->node.GetParent();

				if (parent == nullptr) {
					return;
				}

				auto& children = parent->node.GetChildren();
				ax::Window::Ptr current_win;

				int index = -1;
				for (int i = 0; i < children.size(); i++) {
					if (children[i]->GetId() == win->GetId()) {
						current_win = children[i];
						index = i;
						break;
					}
				}

				if (current_win && index != -1) {
					win->event.UnGrabMouse();
					ax::App::GetInstance().GetWindowManager()->ReleaseMouseHover();
					children.erase(children.begin() + index);

					/// @todo Remove from inspector menu.
					gwin->PushEvent(1234, new ax::Event::SimpleMsg<ax::Window*>(nullptr));
				}

				return;
			}

			// Call widget callback.
			if (m_right_down) {
				m_right_down(pos);
			}
		});
		
//		auto m_key_delete_down = win->event.OnBackSpaceDown.GetFunction();
//		win->event.OnBackSpaceDown = ax::WFunc<char>([gwin, win, m_key_delete_down](const char& c) {
//			ax::Print("Backspace down");
//			
//			if (ax::App::GetInstance().GetWindowManager()->IsCmdDown()) {
//				win->Hide();
//				
//				ax::Window* parent = win->node.GetParent();
//				
//				if (parent == nullptr) {
//					return;
//				}
//				
//				auto& children = parent->node.GetChildren();
//				ax::Window::Ptr current_win;
//				
//				int index = -1;
//				for (int i = 0; i < children.size(); i++) {
//					if (children[i]->GetId() == win->GetId()) {
//						current_win = children[i];
//						index = i;
//						break;
//					}
//				}
//				
//				if (current_win && index != -1) {
//					win->event.UnGrabMouse();
//					win->event.UnGrabKey();
//					ax::App::GetInstance().GetWindowManager()->ReleaseMouseHover();
//					children.erase(children.begin() + index);
//					
//					/// @todo Remove from inspector menu.
//					gwin->PushEvent(1234, new ax::Event::SimpleMsg<ax::Window*>(nullptr));
//				}
//
//			}
//		});

		win->event.OnPaintOverFrameBuffer = ax::WFunc<ax::GC>([win](ax::GC gc) {
			if (win->property.HasProperty("current_editing_widget")) {
				const ax::Rect rect(win->dimension.GetDrawingRect());
				ax::Color color(255, 0, 0);

				gc.SetColor(color, 0.1);
				gc.DrawRectangleContour(rect);

				gc.SetColor(color, 0.2);
				gc.DrawRectangleContour(rect.GetInteriorRect(ax::Point(0, 0)));

				gc.SetColor(color, 0.4);
				gc.DrawRectangleContour(rect.GetInteriorRect(ax::Point(1, 1)));

				gc.SetColor(color, 0.5);
				gc.DrawRectangleContour(rect.GetInteriorRect(ax::Point(2, 2)));
			}
		});
	}

	
	void PythonCallEmpty(const std::string& fct_name)
	{
		std::string fct_call = fct_name + "();\n";
		PyoAudio::GetInstance()->ProcessString(fct_call);
	}
	
	void PythonCallReal(const std::string& fct_name, double value)
	{
		std::string fct_call = fct_name + "(";
		fct_call += std::to_string(value) + ");\n";
		PyoAudio::GetInstance()->ProcessString(fct_call);
	}
	
	void Loader::SetupPyoComponent(ax::Window* win, const std::string& fct_name)
	{
		auto comp = pyo::Component::Ptr(new pyo::Component(win));
		comp->SetFunctionName(fct_name);
		win->component.Add("pyo", comp);
	}

	void Loader::SetupButtonPyoEvent(ax::Window* win)
	{
		win->AddConnection(ax::Button::Events::BUTTON_CLICK, ax::Event::Function([win](ax::Event::Msg* msg) {
							   if (win->component.Has("pyo")) {
								   pyo::Component::Ptr comp = win->component.Get<pyo::Component>("pyo");
								   const std::string fct_name = comp->GetFunctionName();

								   if (!fct_name.empty()) {
								   	   PythonCallEmpty(fct_name);
								   }
							   }
						   }));
	}

	void Loader::SetupTogglePyoEvent(ax::Window* win)
	{
		win->AddConnection(ax::Toggle::Events::BUTTON_CLICK, ax::Event::Function([win](ax::Event::Msg* msg) {
							   if (win->component.Has("pyo")) {
								   pyo::Component::Ptr comp = win->component.Get<pyo::Component>("pyo");
								   const std::string fct_name = comp->GetFunctionName();

								   if (!fct_name.empty()) {
									   PythonCallEmpty(fct_name);
								   }
							   }
						   }));
	}

	void Loader::SetupKnobPyoEvent(ax::Window* win)
	{
		win->AddConnection(0, ax::Event::Function([win](ax::Event::Msg* msg) {
							   if (win->component.Has("pyo")) {
								   pyo::Component::Ptr comp = win->component.Get<pyo::Component>("pyo");
								   const std::string fct_name = comp->GetFunctionName();

								   if (!fct_name.empty()) {
									   ax::Knob::Msg* kmsg = static_cast<ax::Knob::Msg*>(msg);
									   PythonCallReal(fct_name, kmsg->GetValue());
								   }
							   }
						   }));
	}

	void Loader::SetupSliderPyoEvent(ax::Window* win)
	{
		win->AddConnection(0, ax::Event::Function([win](ax::Event::Msg* msg) {
							   if (win->component.Has("pyo")) {
								   pyo::Component::Ptr comp = win->component.Get<pyo::Component>("pyo");
								   const std::string fct_name = comp->GetFunctionName();

								   if (!fct_name.empty()) {
									   ax::Slider::Msg* kmsg = static_cast<ax::Slider::Msg*>(msg);
									   PythonCallReal(fct_name, 1.0 - kmsg->GetValue());
								   }
							   }
						   }));
	}
}
}