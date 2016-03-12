//#include "axXml.h"
//#include "mdiCtrlButton.hpp"
//#include <axLib/axButton.h>
//#include <axLib/axNumberBox.h>
//#include <axLib/axWindowManager.h>
//
//namespace mdi {
//namespace ctrl {
//	namespace Button {
//		/*
//		 * Button builder.
//		 */
//		Button::Builder::Builder()
//		{
//		}
//
////		void SetupControl(ax::Window* win)
////		{
////			auto m_down_fct = win->event.OnMouseLeftDown.GetFunction();
////			auto m_drag_fct = win->event.OnMouseLeftDragging.GetFunction();
////			auto m_up_fct = win->event.OnMouseLeftUp.GetFunction();
////			auto m_right_down = win->event.OnMouseRightDown.GetFunction();
////
////			win->event.OnMouseLeftDown = ax::WFunc<ax::Point>([win, m_down_fct](const ax::Point& pos) {
////				bool cmd_down = ax::App::GetInstance().GetWindowManager()->IsCmdDown();
////
////				if (cmd_down) {
////					ax::Point c_delta(pos - win->dimension.GetAbsoluteRect().position);
////					win->resource.Add("click_delta", c_delta);
////					win->event.GrabMouse();
////					win->property.AddProperty("edit_click");
////				}
////				else {
////					if (m_down_fct) {
////						m_down_fct(pos);
////					}
////				}
////			});
////
////			win->event.OnMouseLeftDragging = ax::WFunc<ax::Point>([win, m_drag_fct](const ax::Point& pos) {
////				if (win->event.IsGrabbed()) {
////					//					win->resource.
////					if (win->property.HasProperty("edit_click")) {
////
////						ax::Point c_delta = win->resource.GetResource("click_delta");
////						win->dimension.SetPosition(
////							pos - win->node.GetParent()->dimension.GetAbsoluteRect().position - c_delta);
////					}
////				}
////
////				if (m_drag_fct) {
////					m_drag_fct(pos);
////				}
////			});
////
////			win->event.OnMouseLeftUp = ax::WFunc<ax::Point>([win, m_up_fct](const ax::Point& pos) {
////
////				win->property.RemoveProperty("edit_click");
////
////				if (m_up_fct) {
////					m_up_fct(pos);
////				}
////
////				if (win->event.IsGrabbed()) {
////					win->event.UnGrabMouse();
////				}
////			});
////
////			win->event.OnMouseRightDown = ax::WFunc<ax::Point>([win, m_right_down](const ax::Point& pos) {
////				ax::Print("RIGHT DOWN");
////				//				if (win->event.IsGrabbed()) {
////				//					win->event.UnGrabMouse();
////				//				}
////				//
////				win->Hide();
////				win->DeleteWindow();
////
////				if (m_right_down) {
////					m_right_down(pos);
////				}
////			});
////		}
//
//		std::shared_ptr<ax::Window::Backbone> Button::Builder::Create(
//			const ax::Point& pos, const std::string& file_path)
//		{
//			ax::Xml xml(file_path);
//
//			if (!xml.Parse()) {
//				ax::Error("Parsing error.");
//				return nullptr;
//			}
//
//			ax::Xml::Node control = xml.GetNode("Control");
//
//			std::string builder_name = control.GetAttribute("builder");
//			std::string obj_name = control.GetAttribute("name");
//
//			ax::Print(builder_name, obj_name);
//
//			ax::Size size = ax::Xml::StringToSize(control.GetChildNodeValue("size"));
//			std::string img_path = control.GetChildNodeValue("img_path");
//			std::string label = control.GetChildNodeValue("label");
//
//			ax::StringVector flags_strs = ax::Utils::String::Split(control.GetChildNodeValue("flags"), ",");
//
//			ax::Flag flags = 0;
//
//			for (auto& n : flags_strs) {
//
//				if (n == "SINGLE_IMG") {
//					ax::Print("OPT : ", n);
//					flags |= ax::Button::Flags::SINGLE_IMG;
//				}
//				else if (n == "IMG_RESIZE") {
//					flags |= ax::Button::Flags::IMG_RESIZE;
//				}
//				else if (n == "CAN_SELECTED") {
//					flags |= ax::Button::Flags::CAN_SELECTED;
//				}
//			}
//
//			std::string msg = control.GetChildNodeValue("msg");
//
//			ax::Print(size.x, size.y);
//
//			ax::Xml::Node info_node = control.GetNode("info");
//
//			ax::Button::Info btn_info;
//			btn_info.normal = ax::Xml::StringToColor(info_node.GetAttribute("normal"));
//			btn_info.hover = ax::Xml::StringToColor(info_node.GetAttribute("hover"));
//			btn_info.clicking = ax::Xml::StringToColor(info_node.GetAttribute("clicking"));
//			btn_info.selected = ax::Xml::StringToColor(info_node.GetAttribute("selected"));
//			btn_info.contour = ax::Xml::StringToColor(info_node.GetAttribute("contour"));
//			btn_info.font_color = ax::Xml::StringToColor(info_node.GetAttribute("font_color"));
//			btn_info.round_corner_radius = std::stoi(info_node.GetAttribute("round_corner_radius"));
//
//			auto btn = ax::shared<ax::Button>(
//				ax::Rect(pos, size), ax::Button::Events(), btn_info, img_path, label, flags, msg);
//
//			btn->GetWindow()->component.Add(
//				"Control", ax::shared<mdi::ctrl::Button::Control>(btn->GetWindow(), "Button"));
//
//			SetupControl(btn->GetWindow());
//			return btn;
//		}
//
//		std::shared_ptr<ax::Window::Backbone> Button::Builder::Create(rapidxml::xml_node<>* node)
//		{
//			ax::Xml::Node control(node);
//			std::string builder_name = control.GetAttribute("builder");
//
//			ax::Print(builder_name);
//
//			ax::Point pos = ax::Xml::StringToSize(control.GetChildNodeValue("position"));
//			ax::Size size = ax::Xml::StringToSize(control.GetChildNodeValue("size"));
//			std::string img_path = control.GetChildNodeValue("img_path");
//			std::string label = control.GetChildNodeValue("label");
//
//			ax::StringVector flags_strs = ax::Utils::String::Split(control.GetChildNodeValue("flags"), ",");
//
//			ax::Flag flags = 0;
//
//			for (auto& n : flags_strs) {
//
//				if (n == "SINGLE_IMG") {
//					ax::Print("OPT : ", n);
//					flags |= ax::Button::Flags::SINGLE_IMG;
//				}
//				else if (n == "IMG_RESIZE") {
//					flags |= ax::Button::Flags::IMG_RESIZE;
//				}
//				else if (n == "CAN_SELECTED") {
//					flags |= ax::Button::Flags::CAN_SELECTED;
//				}
//			}
//
//			std::string msg = control.GetChildNodeValue("msg");
//
//			ax::Xml::Node info_node = control.GetNode("info");
//
//			ax::Button::Info btn_info;
//			btn_info.normal = ax::Xml::StringToColor(info_node.GetAttribute("normal"));
//			btn_info.hover = ax::Xml::StringToColor(info_node.GetAttribute("hover"));
//			btn_info.clicking = ax::Xml::StringToColor(info_node.GetAttribute("clicking"));
//			btn_info.selected = ax::Xml::StringToColor(info_node.GetAttribute("selected"));
//			btn_info.contour = ax::Xml::StringToColor(info_node.GetAttribute("contour"));
//			btn_info.font_color = ax::Xml::StringToColor(info_node.GetAttribute("font_color"));
//			btn_info.round_corner_radius = std::stoi(info_node.GetAttribute("round_corner_radius"));
//
//			auto btn = ax::shared<ax::Button>(
//				ax::Rect(pos, size), ax::Button::Events(), btn_info, img_path, label, flags, msg);
//
//			btn->GetWindow()->component.Add(
//				"Control", ax::shared<mdi::ctrl::Button::Control>(btn->GetWindow(), "Button"));
//
//			SetupControl(btn->GetWindow());
//			return btn;
//		}
//	}
//}
//}