////
////  mdiCtrlKnob.cpp
////  eosMidiController
////
////  Created by Alexandre Arsenault on 2016-02-19.
////  Copyright © 2016 ax. All rights reserved.
////
//
//#include "axXml.h"
//#include "mdiCtrlKnob.hpp"
//
//namespace mdi {
//namespace ctrl {
//	namespace Knob {
//		/*
//		 * Knob builder.
//		 */
//		Knob::Builder::Builder()
//		{
//		}
//
//		std::shared_ptr<ax::Window::Backbone> Knob::Builder::Create(
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
//			double value = std::stod(control.GetChildNodeValue("value"));
//			ax::Flag flags = std::stoi(control.GetChildNodeValue("flags"));
//			std::string msg = control.GetChildNodeValue("msg");
//
//			ax::Xml::Node info_node = control.GetNode("info");
//
//			ax::Knob::Info k_info;
//			k_info.bgColorNormal = ax::Xml::StringToColor(info_node.GetAttribute("bgColorNormal"));
//			k_info.bgColorHover = ax::Xml::StringToColor(info_node.GetAttribute("bgColorHover"));
//			k_info.bgColorClicked = ax::Xml::StringToColor(info_node.GetAttribute("bgColorClicked"));
//
//			k_info.img_path = info_node.GetAttribute("img_path");
//			k_info.selected_img_path = info_node.GetAttribute("selected_img_path");
//			k_info.knob_size = ax::Xml::StringToSize(info_node.GetAttribute("knob_size"));
//			k_info.n_knob = std::stoi(info_node.GetAttribute("n_knob"));
//
//			auto knob
//				= ax::shared<ax::Knob>(ax::Rect(pos, size), ax::Knob::Events(), k_info, flags, value, msg);
//
//			//			ax::StringVector info_labels = { "bgColorNormal", "bgColorHover", "bgColorClicked",
//			//"img_path",
//			//				"selected_img_path", "knob_size", "n_knob" };
//
//			knob->GetWindow()->component.Add(
//				"Control", ax::shared<mdi::ctrl::Knob::Control>(knob->GetWindow(), "Knob"));
//
//			SetupControl(knob->GetWindow());
//			return knob;
//		}
//
//		std::shared_ptr<ax::Window::Backbone> Knob::Builder::Create(rapidxml::xml_node<>* node)
//		{
//			ax::Xml::Node control(node);
//			std::string builder_name = control.GetAttribute("builder");
//
//			ax::Print(builder_name);
//
//			ax::Size pos = ax::Xml::StringToSize(control.GetChildNodeValue("position"));
//			ax::Size size = ax::Xml::StringToSize(control.GetChildNodeValue("size"));
//			double value = std::stod(control.GetChildNodeValue("value"));
//			ax::Flag flags = std::stoi(control.GetChildNodeValue("flags"));
//			std::string msg = control.GetChildNodeValue("msg");
//
//			ax::Xml::Node info_node = control.GetNode("info");
//
//			ax::Knob::Info k_info;
//			k_info.bgColorNormal = ax::Xml::StringToColor(info_node.GetAttribute("bgColorNormal"));
//			k_info.bgColorHover = ax::Xml::StringToColor(info_node.GetAttribute("bgColorHover"));
//			k_info.bgColorClicked = ax::Xml::StringToColor(info_node.GetAttribute("bgColorClicked"));
//
//			k_info.img_path = info_node.GetAttribute("img_path");
//			k_info.selected_img_path = info_node.GetAttribute("selected_img_path");
//			k_info.knob_size = ax::Xml::StringToSize(info_node.GetAttribute("knob_size"));
//			k_info.n_knob = std::stoi(info_node.GetAttribute("n_knob"));
//
//			auto knob
//				= ax::shared<ax::Knob>(ax::Rect(pos, size), ax::Knob::Events(), k_info, flags, value, msg);
//
//			//			ax::StringVector info_labels = { "bgColorNormal", "bgColorHover", "bgColorClicked",
//			//"img_path",
//			//				"selected_img_path", "knob_size", "n_knob" };
//
//			knob->GetWindow()->component.Add(
//				"Control", ax::shared<mdi::ctrl::Knob::Control>(knob->GetWindow(), "Knob"));
//			SetupControl(knob->GetWindow());
//
//			return knob;
//		}
//	}
//}
//}