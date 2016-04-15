#ifndef PythonWrapper_hpp
#define PythonWrapper_hpp

#include <Python/Python.h>
#include <cstdio>

#include <OpenAX/Utils.h>
#include <boost/python.hpp>

#include "PythonWrapperUtils.h"

namespace ax {
namespace python {
	/*
	 * ax::python::Widgets.
	 */
	class Widgets {
	public:
		Widgets();

		boost::python::object Get(const std::string& widget_name);

		static std::shared_ptr<ax::python::Widgets> GetInstance();
		static std::shared_ptr<ax::python::Widgets> instance;

	private:
		boost::shared_ptr<ax::Point> _pt;
	};

	void InitWrapper();
}
}

#endif /* PythonWrapper_hpp */
