//#include "mdiCtrlToggle.hpp"
//#include "axXml.h"
//
//namespace mdi {
//	namespace ctrl {
//	namespace Toggle {
//		/*
//		 * Toggle builder.
//		 */
//		Toggle::Builder::Builder()
//		{
//		}
//		
//		std::shared_ptr<ax::Window::Backbone> Toggle::Builder::Create(const ax::Point& pos, const std::string& file_path)
//		{
//			ax::Xml xml(file_path);
//			
//			if(!xml.Parse()) {
//				ax::Error("Parsing error.");
//				return nullptr;
//			}
//			
//			ax::Xml::Node control = xml.GetNode("Control");
//			std::string builder_name = control.GetAttribute("builder");
//			std::string obj_name = control.GetAttribute("name");
//			
//			ax::Print(builder_name, obj_name);
//			
//			ax::Size size = ax::Xml::StringToSize(control.GetChildNodeValue("size"));
//			std::string img_path = control.GetChildNodeValue("img_path");
//			std::string label = control.GetChildNodeValue("label");
//			ax::Flag flags = std::stoi(control.GetChildNodeValue("flags"));
//			std::string msg = control.GetChildNodeValue("msg");
//			
//			ax::Print(size.x, size.y);
//			
//			ax::Xml::Node info_node = control.GetNode("info");
//			
//			ax::Toggle::Info tog_info;
//			tog_info.normal = ax::Xml::StringToColor(info_node.GetAttribute("normal"));
//			tog_info.hover = ax::Xml::StringToColor(info_node.GetAttribute("hover"));
//			tog_info.clicking = ax::Xml::StringToColor(info_node.GetAttribute("clicking"));
//			tog_info.font_color = ax::Xml::StringToColor(info_node.GetAttribute("font_color"));
//			
//			tog_info.selected = ax::Xml::StringToColor(info_node.GetAttribute("selected"));
//			tog_info.selected_hover = ax::Xml::StringToColor(info_node.GetAttribute("selected_hover"));
//			tog_info.selected_clicking = ax::Xml::StringToColor(info_node.GetAttribute("selected_clicking"));
//			tog_info.selected_font_color = ax::Xml::StringToColor(info_node.GetAttribute("selected_font_color"));
//			
//			tog_info.contour = ax::Xml::StringToColor(info_node.GetAttribute("contour"));
//			tog_info.font_size = std::stoi(info_node.GetAttribute("font_size"));
//			tog_info.img = info_node.GetAttribute("img");
//			tog_info.single_img = std::stoi(info_node.GetAttribute("single_img"));
//			
//			
//			auto tog = ax::shared<ax::Toggle>(ax::Rect(pos, size), ax::Toggle::Events(), tog_info, img_path, label, flags, msg);
//			SetupControl(tog->GetWindow());
//			return tog;
//		}
//	}
//	}
//}