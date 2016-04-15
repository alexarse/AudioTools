//
//  ButtonPyWrapper.cpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-15.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#include "ButtonPyWrapper.h"
#include <Python/Python.h>
#include <boost/python.hpp>
#include <cstdio>

using namespace boost::python;

namespace ax {
namespace python {

	Button::Button(ax::Button* btn)
		: ax::python::Window(btn->GetWindow())
		, _btn(btn)
	{
	}

	void Button::SetBackgroundColor(const ax::Color& color)
	{
		widget::Component* widget
			= static_cast<widget::Component*>(_btn->GetWindow()->component.Get("Widget").get());
		ax::Button::Info* info = static_cast<ax::Button::Info*>(widget->GetInfo());

		info->normal = color;
		widget->ReloadInfo();
	}

	void Button::SetContourColor(const ax::Color& color)
	{
		//			widget::Component* widget
		//			= static_cast<widget::Component*>(_panel->GetWindow()->component.Get("Widget").get());
		//			ax::Panel::Info* info = static_cast<ax::Panel::Info*>(widget->GetInfo());
		//
		//			info->contour = color;
		//			widget->ReloadInfo();
	}

	void export_python_wrapper_button()
	{
		class_<ax::python::Button, boost::python::bases<ax::python::Window>>("Button", init<ax::Button*>())
			.def("SetBackgroundColor", &ax::python::Button::SetBackgroundColor)
			.def("SetContourColor", &ax::python::Button::SetContourColor);
	}
}
}