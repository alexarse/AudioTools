#include "PythonWrapper.h"
#include "atEditor.h"
#include "atEditorMainWindow.h"
#include "PanelPyWrapper.h"

BOOST_PYTHON_MODULE(ax)
{
	ax::python::export_python_wrapper_utils();
	ax::python::export_python_wrapper_panel();

	boost::python::class_<ax::python::Widgets>("Widgets").def("Get", &ax::python::Widgets::Get);
}

namespace ax {
namespace python {
	std::shared_ptr<ax::python::Widgets> Widgets::instance = nullptr;
	
	std::shared_ptr<ax::python::Widgets> Widgets::GetInstance()
	{
		if(instance == nullptr) {
			instance = std::shared_ptr<ax::python::Widgets>(new Widgets());
		}
		
		return instance;
	}
	
	Widgets::Widgets()
	{
		_pt = boost::shared_ptr<ax::Point>(new ax::Point(14, 642));
	}

	boost::python::object Widgets::Get(const std::string& widget_name)
	{
		ax::Window* win = at::editor::App::GetInstance()->GetMainWindow()->GetWidgetsByName(widget_name);
	
		if(win == nullptr) {
			return boost::python::object();
		}
		
		widget::Component* widget = static_cast<widget::Component*>(win->component.Get("Widget").get());
		
		if(widget->GetBuilderName() == "Panel") {
			ax::Panel* panel = static_cast<ax::Panel*>(win->backbone.get());
			ax::python::Panel* py_panel = new ax::python::Panel(panel);
			return boost::python::object(boost::python::ptr(py_panel));
		}
//		ax::Panel::Info* info = static_cast<ax::Panel::Info*>(widget->GetInfo());

		
	
		return boost::python::object(boost::python::ptr(_pt.get()));
	}
	
	void InitWrapper()
	{
		initax();
	}
}
}