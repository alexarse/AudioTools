//#include "axXml.h"
//#include "mdiCtrlSlider.hpp"
//#include <axLib/axSlider.h>
//
//namespace mdi {
//namespace ctrl {
//	namespace Slider {
//		/*
//		 * Button builder.
//		 */
//		Slider::Builder::Builder()
//		{
//		}
//
//		std::shared_ptr<ax::Window::Backbone> Slider::Builder::Create(
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
//			std::string builder_name = control.GetAttribute("builder");
//			std::string obj_name = control.GetAttribute("name");
//
//			ax::Print(builder_name, obj_name);
//
//			ax::Size size = ax::Xml::StringToSize(control.GetChildNodeValue("size"));
//
//			ax::StringVector flags_strs = ax::Utils::String::Split(control.GetChildNodeValue("flags"), ",");
//
//			ax::Flag flags = 0;
//
//			for (auto& n : flags_strs) {
//				ax::Print("OPT : ", n);
//				if (n == "VERTICAL") {
//					flags |= axSLIDER_FLAG_VERTICAL;
//				}
//				else if (n == "CLICK_ANYWHERE") {
//					flags |= axSLIDER_FLAG_CLICK_ANYWHERE;
//				}
//				else if (n == "RELEASE_ON_LEAVE") {
//					flags |= axSLIDER_FLAG_RELEASE_ON_LEAVE;
//				}
//				else if (n == "LEFT_CLICK_ENTER") {
//					flags |= axSLIDER_FLAG_LEFT_CLICK_ENTER;
//				}
//				else if (n == "RIGHT_ALIGN") {
//					flags |= axSLIDER_FLAG_RIGHT_ALIGN;
//				}
//				else if (n == "MIDDLE_ALIGN") {
//					flags |= axSLIDER_FLAG_MIDDLE_ALIGN;
//				}
//				else if (n == "BACK_SLIDER") {
//					flags |= axSLIDER_FLAG_BACK_SLIDER;
//				}
//				else if (n == "NO_SLIDER_LINE") {
//					flags |= axSLIDER_FLAG_NO_SLIDER_LINE;
//				}
//			}
//
//			ax::Print(size.x, size.y);
//
//			ax::Xml::Node info_node = control.GetNode("info");
//
//			ax::Slider::Info sld_info;
//			sld_info.bgColorNormal = ax::Xml::StringToColor(info_node.GetAttribute("bgColorNormal"));
//			sld_info.bgColorHover = ax::Xml::StringToColor(info_node.GetAttribute("bgColorHover"));
//			sld_info.bgColorClicked = ax::Xml::StringToColor(info_node.GetAttribute("bgColorClicked"));
//			sld_info.sliderColorNormal = ax::Xml::StringToColor(info_node.GetAttribute("sliderColorNormal"));
//			sld_info.sliderColorClicked
//				= ax::Xml::StringToColor(info_node.GetAttribute("sliderColorClicked"));
//			sld_info.sliderContourColor
//				= ax::Xml::StringToColor(info_node.GetAttribute("sliderContourColor"));
//			sld_info.contourColor = ax::Xml::StringToColor(info_node.GetAttribute("contourColor"));
//
//			sld_info.backSliderColor = ax::Xml::StringToColor(info_node.GetAttribute("backSliderColor"));
//			sld_info.backSliderContourColor
//				= ax::Xml::StringToColor(info_node.GetAttribute("backSliderContourColor"));
//
//			sld_info.contour_round_radius = std::stoi(info_node.GetAttribute("contour_round_radius"));
//			sld_info.slider_width = std::stoi(info_node.GetAttribute("slider_width"));
//
//			sld_info.btn_size = ax::Xml::StringToSize(info_node.GetAttribute("btn_size"));
//			sld_info.img_path = info_node.GetAttribute("img_path");
//
//			auto slider = ax::shared<ax::Slider>(ax::Rect(pos, size), ax::Slider::Events(), sld_info, flags);
//			SetupControl(slider->GetWindow());
//			return slider;
//		}
//	}
//}
//}