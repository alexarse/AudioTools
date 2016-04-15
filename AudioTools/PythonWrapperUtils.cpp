//
//  PythonWrapperUtils.cpp
//  PythonLexer
//
//  Created by Alexandre Arsenault on 2016-04-14.
//  Copyright © 2016 OpenAX. All rights reserved.
//

#include "PythonWrapperUtils.h"
#include <Python/Python.h>
#include <cstdio>

#include <OpenAX/Utils.h>
#include <boost/python.hpp>

using namespace boost::python;

namespace ax {
namespace python {
	void export_python_wrapper_utils()
	{
		// ax;:Point.
		class_<ax::Point>("Point", init<int, int>())
			.def(init<>())
			.def_readwrite("x", &ax::Point::x)
			.def_readwrite("y", &ax::Point::y)
			.def(self + self)
			.def(self - self)
			.def(self * self)
			.def(self += self)
			.def(self -= self)
			.def(self *= other<double>());

		// ax::Size.
		//	class_<ax::Size>("Size", init<int, int>())
		//		.def(init<>())
		//		.def_readwrite("x", &ax::Size::x)
		//		.def_readwrite("y", &ax::Size::y)
		//		.def(self + self)
		//		.def(self - self)
		//		.def(self * self)
		//		.def(self += self)
		//		.def(self -= self)
		//		.def(self *= other<double>());

		// ax::Rect.
		class_<ax::Rect>("Rect", init<int, int, int, int>())
			.def_readwrite("position", &ax::Rect::position)
			.def_readwrite("size", &ax::Rect::size);

		// ax::Color.
		class_<ax::Color>("Color", init<int, int, int, int>())
			.def(init<int, int, int>())
			.def(init<float, float, float, float>())
			.def(init<float, float, float>())
			.add_property("r", &ax::Color::GetRed, &ax::Color::SetRed)
			.add_property("g", &ax::Color::GetGreen, &ax::Color::SetGreen)
			.add_property("b", &ax::Color::GetBlue, &ax::Color::SetBlue)
			.add_property("a", &ax::Color::GetAlpha, &ax::Color::SetAlpha);
	}
}
}