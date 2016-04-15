//
//  WindowPyWrapper.cpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-15.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#include "WindowPyWrapper.h"
#include <Python/Python.h>
#include <boost/python.hpp>
#include <cstdio>

using namespace boost::python;

namespace ax {
namespace python {

	Window::Window(ax::Window* win)
		: _win(win)
	{
	}

	void Window::SetPosition(const ax::Point& position)
	{
		_win->dimension.SetPosition(position);
	}

	void export_python_wrapper_window()
	{
		class_<ax::python::Window>("Window", init<ax::Window*>())
			.def("SetPosition", &ax::python::Window::SetPosition);
	}
}
}