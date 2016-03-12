////
////  mdiCtrlKnob.hpp
////  eosMidiController
////
////  Created by Alexandre Arsenault on 2016-02-19.
////  Copyright © 2016 ax. All rights reserved.
////
//
//#ifndef mdiCtrlKnob_hpp
//#define mdiCtrlKnob_hpp
//
//#include "mdiControl.hpp"
//#include "axKnob.h"
//#include "axLib.h"
//
//namespace mdi {
//namespace ctrl {
//	namespace Knob {
//		/*
//		 * Knob builder.
//		 */
//		class Builder : public mdi::ctrl::Builder {
//		public:
//			Builder();
//
//			virtual std::shared_ptr<ax::Window::Backbone> Create(
//				const ax::Point& pos, const std::string& file_path);
//
//			virtual std::shared_ptr<ax::Window::Backbone> Create(rapidxml::xml_node<>* node);
//		};
//
//		class Control : public mdi::ctrl::Control {
//		public:
//			typedef std::shared_ptr<Control> Ptr;
//
//			Control(ax::Window* win, const std::string& name)
//				: mdi::ctrl::Control(win, name)
//			{
//			}
//
//			virtual void Save(rapidxml::xml_node<>* node)
//			{
//				rapidxml::xml_document<>& doc = *node->document();
//				ax::Window* win = GetWindow();
//				std::shared_ptr<ax::Window::Backbone> bbone = win->backbone;
//				ax::Knob* knob = static_cast<ax::Knob*>(bbone.get());
//
//				ax::WidgetComponent* widget_comp
//					= static_cast<ax::WidgetComponent*>(win->component.Get("Widget").get());
//				ax::Knob::Info* info = static_cast<ax::Knob::Info*>(widget_comp->GetInfo());
//
//				rapidxml::xml_node<>* ctrl = doc.allocate_node(rapidxml::node_element, "Control");
//				ctrl->append_attribute(doc.allocate_attribute("builder", "Knob"));
//				node->append_node(ctrl);
//
//				ax::Rect rect = win->dimension.GetRect();
//
//				// Position.
//				{
//					std::string value_str(
//						std::to_string(rect.position.x) + ", " + std::to_string(rect.position.y));
//					char* value = doc.allocate_string(value_str.data(), value_str.size());
//					ctrl->append_node(
//						doc.allocate_node(rapidxml::node_element, "position", value, 0, value_str.size()));
//				}
//
//				// Size.
//				{
//					std::string value_str(std::to_string(rect.size.x) + ", " + std::to_string(rect.size.y));
//					char* value = doc.allocate_string(value_str.data(), value_str.size());
//
//					ctrl->append_node(
//						doc.allocate_node(rapidxml::node_element, "size", value, 0, value_str.size()));
//				}
//
//				// Info node.
//				rapidxml::xml_node<>* xml_info = doc.allocate_node(rapidxml::node_element, "info");
//				ctrl->append_node(xml_info);
//				{ // Background color normal.
//				  { std::string value_str(info->bgColorNormal.ToString());
//				char* value = doc.allocate_string(value_str.data(), value_str.size());
//
//				xml_info->append_attribute(
//					doc.allocate_attribute("bgColorNormal", value, 0, value_str.size()));
//			}
//
//			// Background color hover.
//			{
//				std::string value_str(info->bgColorHover.ToString());
//				char* value = doc.allocate_string(value_str.data(), value_str.size());
//
//				xml_info->append_attribute(
//					doc.allocate_attribute("bgColorHover", value, 0, value_str.size()));
//			}
//
//			// Background color clicking.
//			{
//				std::string value_str(info->bgColorClicked.ToString());
//				char* value = doc.allocate_string(value_str.data(), value_str.size());
//
//				xml_info->append_attribute(
//					doc.allocate_attribute("bgColorClicked", value, 0, value_str.size()));
//			}
//
//			// Image path.
//			{
//				std::string value_str(info->img_path);
//				char* value = doc.allocate_string(value_str.data(), value_str.size());
//
//				xml_info->append_attribute(doc.allocate_attribute("img_path", value, 0, value_str.size()));
//			}
//
//			// Image path selected.
//			{
//				std::string value_str(info->selected_img_path);
//				char* value = doc.allocate_string(value_str.data(), value_str.size());
//
//				xml_info->append_attribute(
//					doc.allocate_attribute("selected_img_path", value, 0, value_str.size()));
//			}
//
//			// Knob size.
//			{
//				std::string value_str(std::string(
//					std::to_string(info->knob_size.x) + ", " + std::to_string(info->knob_size.y)));
//				char* value = doc.allocate_string(value_str.data(), value_str.size());
//
//				xml_info->append_attribute(doc.allocate_attribute("knob_size", value, 0, value_str.size()));
//			}
//
//			// Knob size.
//			{
//				std::string value_str(std::to_string(info->n_knob));
//				char* value = doc.allocate_string(value_str.data(), value_str.size());
//
//				xml_info->append_attribute(doc.allocate_attribute("n_knob", value, 0, value_str.size()));
//			}
//		}
//
//		// Value.
//		{
//			std::string value_str("0.0");
//			char* value = doc.allocate_string(value_str.data(), value_str.size());
//
//			ctrl->append_node(doc.allocate_node(rapidxml::node_element, "value", value, 0, value_str.size()));
//		}
//
//		// Flags.
//		{
//			std::string value_str(std::to_string((unsigned int)knob->GetFlags()));
//			char* value = doc.allocate_string(value_str.data(), value_str.size());
//
//			ctrl->append_node(doc.allocate_node(rapidxml::node_element, "flags", value, 0, value_str.size()));
//		}
//
//		// Msg.
//		{
//			std::string value_str(knob->GetMsg());
//			char* value = doc.allocate_string(value_str.data(), value_str.size());
//
//			ctrl->append_node(doc.allocate_node(rapidxml::node_element, "msg", value, 0, value_str.size()));
//		}
//	}
//};
//}
//}
//}
//
//#endif /* mdiCtrlKnob_hpp */
