//#ifndef __MDI_CTRL_BUTTON_HPP__
//#define __MDI_CTRL_BUTTON_HPP__
//
//#include "mdiControl.hpp"
//#include "axButton.h"
//#include "axLib.h"
//
//namespace mdi {
//namespace ctrl {
//	namespace Button {
//		/*
//		 * Button builder.
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
//				ax::Button* btn = static_cast<ax::Button*>(bbone.get());
//
//				ax::WidgetComponent* widget_comp
//					= static_cast<ax::WidgetComponent*>(win->component.Get("Widget").get());
//				ax::Button::Info* info = static_cast<ax::Button::Info*>(widget_comp->GetInfo());
//
//				rapidxml::xml_node<>* ctrl = doc.allocate_node(rapidxml::node_element, "Control");
//				ctrl->append_attribute(doc.allocate_attribute("builder", "Button"));
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
//
//				{
//					std::string value_str(info->normal.ToString());
//					char* value = doc.allocate_string(value_str.data(), value_str.size());
//
//					xml_info->append_attribute(doc.allocate_attribute("normal", value, 0, value_str.size()));
//				}
//
//				// Background color hover.
//				{
//					std::string value_str(info->hover.ToString());
//					char* value = doc.allocate_string(value_str.data(), value_str.size());
//
//					xml_info->append_attribute(doc.allocate_attribute("hover", value, 0, value_str.size()));
//				}
//
//				// Background color clicking.
//				{
//					std::string value_str(info->clicking.ToString());
//					char* value = doc.allocate_string(value_str.data(), value_str.size());
//
//					xml_info->append_attribute(
//						doc.allocate_attribute("clicking", value, 0, value_str.size()));
//				}
//
//				// Background color selected.
//				{
//					std::string value_str(info->selected.ToString());
//					char* value = doc.allocate_string(value_str.data(), value_str.size());
//
//					xml_info->append_attribute(
//						doc.allocate_attribute("selected", value, 0, value_str.size()));
//				}
//
//				// Contour color.
//				{
//					std::string value_str(info->contour.ToString());
//					char* value = doc.allocate_string(value_str.data(), value_str.size());
//
//					xml_info->append_attribute(doc.allocate_attribute("contour", value, 0, value_str.size()));
//				}
//
//				// Font color.
//				{
//					std::string value_str(info->font_color.ToString());
//					char* value = doc.allocate_string(value_str.data(), value_str.size());
//
//					xml_info->append_attribute(
//						doc.allocate_attribute("font_color", value, 0, value_str.size()));
//				}
//
//				// Corner radius.
//				{
//					std::string value_str(std::to_string(info->round_corner_radius));
//					char* value = doc.allocate_string(value_str.data(), value_str.size());
//
//					xml_info->append_attribute(
//						doc.allocate_attribute("round_corner_radius", value, 0, value_str.size()));
//				}
//
//				// Image path.
//				{
//					std::string value_str(btn->GetImagePath());
//					char* value = doc.allocate_string(value_str.data(), value_str.size());
//
//					ctrl->append_node(
//						doc.allocate_node(rapidxml::node_element, "img_path", value, 0, value_str.size()));
//				}
//
//				// Label.
//				{
//					std::string value_str(btn->GetLabel());
//					char* value = doc.allocate_string(value_str.data(), value_str.size());
//
//					ctrl->append_node(
//						doc.allocate_node(rapidxml::node_element, "label", value, 0, value_str.size()));
//				}
//
//				// Flags.
//				{
//					ax::Flag flags = btn->GetFlags();
//					ax::StringVector atts;
//
//					if (ax::IsFlag(ax::Button::Flags::SINGLE_IMG, flags)) {
//						atts.push_back("SINGLE_IMG");
//					}
//
//					if (ax::IsFlag(ax::Button::Flags::IMG_RESIZE, flags)) {
//						atts.push_back("IMG_RESIZE");
//					}
//
//					if (ax::IsFlag(ax::Button::Flags::CAN_SELECTED, flags)) {
//						atts.push_back("CAN_SELECTED");
//					}
//
//					std::string value_str;
//
//					if (atts.empty()) {
//					}
//					else {
//						value_str = atts[0];
//						for (int i = 1; i < atts.size(); i++) {
//							value_str += ",";
//							value_str += atts[i];
//						}
//					}
//
//					char* value = doc.allocate_string(value_str.data(), value_str.size());
//
//					ctrl->append_node(
//						doc.allocate_node(rapidxml::node_element, "flags", value, 0, value_str.size()));
//				}
//
//				// Msg.
//				{
//					std::string value_str(btn->GetMsg());
//					char* value = doc.allocate_string(value_str.data(), value_str.size());
//
//					ctrl->append_node(
//						doc.allocate_node(rapidxml::node_element, "msg", value, 0, value_str.size()));
//				}
//			}
//		};
//	}
//}
//}
//
//#endif // __MDI_CTRL_BUTTON_HPP__
