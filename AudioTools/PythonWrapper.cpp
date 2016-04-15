#include "PythonWrapper.h"
#include "atEditor.h"
#include "atEditorMainWindow.h"
#include "PanelPyWrapper.h"
#include "ButtonPyWrapper.h"
#include "WindowPyWrapper.h"

BOOST_PYTHON_MODULE(ax)
{
	// Create ax::Window python wrapper.
	ax::python::export_python_wrapper_window();
	
	ax::python::export_python_wrapper_utils();
	
	// Create ax::Panel python wrapper.
	ax::python::export_python_wrapper_panel();
	
	// Create ax::Button python wrapper.
	ax::python::export_python_wrapper_button();

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
		
		const std::string builder_name(widget->GetBuilderName());
		
		if(builder_name == "Panel") {
			ax::Panel* panel = static_cast<ax::Panel*>(win->backbone.get());
			return boost::python::object(ax::python::Panel(panel));
		}
		else if(builder_name == "Button") {
			ax::Button* btn = static_cast<ax::Button*>(win->backbone.get());
			return boost::python::object(ax::python::Button(btn));
		}
		
	
		return boost::python::object(boost::python::ptr(_pt.get()));
	}
	
	void InitWrapper()
	{
		initax();
	}
}
}