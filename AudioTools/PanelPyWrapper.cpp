#include "PanelPyWrapper.h"
#include <cstdio>
#include <Python/Python.h>
#include <boost/python.hpp>

#include <OpenAX/OpenAX.h>
#include <OpenAX/Panel.h>

using namespace boost::python;

//Panel(const ax::Rect& rect, const Panel::Info& info, const std::string& bg_img,
//	  const std::string& name = "", ax::Flag flags = 0);
//
//ax::Window::Backbone* GetCopy();
//
//ax::Flag GetFlags() const
//{
//	return _flags;
//}
//
//std::string GetName() const
//{
//	return _name;
//}
//
//void SetName(const std::string& name)
//{
//	_name = name;
//}
//
//std::string GetBackgroundImagePath() const
//{
//	return _bg_img_path;
//}

namespace ax {
namespace python {
	void export_python_wrapper_panel()
	{
		// ax;:Panel.
//		class_<ax::Panel>("Panel", init<>())
//			.def_readwrite("x", &ax::Point::x)
//			.def_readwrite("y", &ax::Point::y)
//			.def(self + self)
//			.def(self - self)
//			.def(self * self)
//			.def(self += self)
//			.def(self -= self)
//			.def(self *= other<double>());
	}
}
}