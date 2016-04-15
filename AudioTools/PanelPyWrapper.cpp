#include "PanelPyWrapper.h"
#include <Python/Python.h>
#include <boost/python.hpp>
#include <cstdio>

#include <OpenAX/OpenAX.h>
#include <OpenAX/Panel.h>

using namespace boost::python;

namespace ax {
namespace python {

	Panel::Panel(ax::Panel* panel)
		: _panel(panel)
	{
	}

	void Panel::SetBackgroundColor(const ax::Color& color)
	{
		widget::Component* widget
			= static_cast<widget::Component*>(_panel->GetWindow()->component.Get("Widget").get());
		ax::Panel::Info* info = static_cast<ax::Panel::Info*>(widget->GetInfo());

		info->background = color;
		widget->ReloadInfo();
	}

	void Panel::SetContourColor(const ax::Color& color)
	{
		widget::Component* widget
			= static_cast<widget::Component*>(_panel->GetWindow()->component.Get("Widget").get());
		ax::Panel::Info* info = static_cast<ax::Panel::Info*>(widget->GetInfo());

		info->contour = color;
		widget->ReloadInfo();
	}

	void export_python_wrapper_panel()
	{
		class_<ax::python::Panel>("Panel", init<ax::Panel*>())
			.def("SetBackgroundColor", &ax::python::Panel::SetBackgroundColor)
			.def("SetContourColor", &ax::python::Panel::SetContourColor);
	}
}
}