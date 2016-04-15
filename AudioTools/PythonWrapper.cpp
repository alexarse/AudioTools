#include "PythonWrapper.h"
#include "atEditor.h"

//using namespace boost::python;

BOOST_PYTHON_MODULE(ax)
{
	ax::python::export_python_wrapper_utils();

	boost::python::class_<ax::python::Widgets>("Widgets").def("Get", &ax::python::Widgets::Get);
}

namespace ax {
namespace python {
	void InitWrapper()
	{
		initax();
	}

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
		return boost::python::object(boost::python::ptr(_pt.get()).get());
	}
}
}