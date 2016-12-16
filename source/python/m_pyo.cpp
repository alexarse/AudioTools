//
//  m_pyo.cpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-15.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#include "python/m_pyo.h"
#include "atConsoleStream.h"

std::string handle_pyerror()
{
	using namespace boost;

	PyObject *exc, *val, *tb;

	PyErr_Fetch(&exc, &val, &tb);

	boost::python::handle<> hexc(exc), hval(boost::python::allow_null(val)),
		htb(boost::python::allow_null(tb));

	boost::python::object traceback(boost::python::import("traceback"));
	boost::python::object formatted_list, formatted;

	if (!tb) {
		boost::python::object format_exception_only(traceback.attr("format_exception_only"));
		formatted_list = format_exception_only(hexc, hval);
	}
	else {
		boost::python::object format_exception(traceback.attr("format_exception"));
		formatted_list = format_exception(hexc, hval, htb);
	}

	formatted = boost::python::str("").join(formatted_list);

	return boost::python::extract<std::string>(formatted);
}

int pyo_exec_file(PyThreadState* interp, const char* file, char* msg, int add)
{
	int err = 0;
	PyEval_AcquireThread(interp);

	std::string catcher = "import sys\n"
						  "class StdoutCatcher:\n"
						  "\tdef __init__(self):\n"
						  "\t\tself.data = ''\n"
						  "\tdef write(self, stuff):\n"
						  "\t\tself.data = self.data + stuff\n"
						  "catcher = StdoutCatcher()\n"
						  "sys.stdout = catcher\n";

	try {
		boost::python::object main_module = boost::python::import("__main__");
		boost::python::object globals = main_module.attr("__dict__");
		boost::python::object catcher_ignore = boost::python::exec(catcher.c_str(), globals);
		ax::python::InitWrapper();

		globals["widgets"] = boost::python::ptr(ax::python::Widgets::GetInstance().get());

		boost::python::object ignored = boost::python::exec_file(file, globals);

		boost::python::object catcher_obj = main_module.attr("catcher");
		boost::python::object output_obj = catcher_obj.attr("data");
		std::string mm = boost::python::extract<std::string>(output_obj);

		if (!mm.empty()) {
			at::ConsoleStream::GetInstance()->Write(mm);
		}
	}
	catch (boost::python::error_already_set const&) {
		std::string msg;

		if (PyErr_Occurred()) {
			msg = handle_pyerror();
		}

		PyErr_Clear();

		if (!msg.empty()) {
			at::ConsoleStream::GetInstance()->Error(msg);
		}
	}

	PyEval_ReleaseThread(interp);
	return err;
}

int pyo_exec_statement(PyThreadState* interp, char* msg, int debug)
{
	int err = 0;

	PyEval_AcquireThread(interp);

	std::string catcher = "import sys\n"
						  "class StdoutCatcher:\n"
						  "\tdef __init__(self):\n"
						  "\t\tself.data = ''\n"
						  "\tdef write(self, stuff):\n"
						  "\t\tself.data = self.data + stuff\n"
						  "catcher = StdoutCatcher()\n"
						  "sys.stdout = catcher\n";

	try {
		boost::python::object main_module = boost::python::import("__main__");
		boost::python::object globals = main_module.attr("__dict__");
		boost::python::object catcher_ignore = boost::python::exec(catcher.c_str(), globals);

		globals["widgets"] = boost::python::ptr(ax::python::Widgets::GetInstance().get());
		boost::python::object ignored = boost::python::exec(msg, globals);

		boost::python::object catcher_obj = main_module.attr("catcher");
		boost::python::object output_obj = catcher_obj.attr("data");
		std::string mm = boost::python::extract<std::string>(output_obj);

		if (!mm.empty()) {
			at::ConsoleStream::GetInstance()->Write(mm);
		}
	}
	catch (boost::python::error_already_set const&) {
		std::string msg;

		if (PyErr_Occurred()) {
			msg = handle_pyerror();
		}

		PyErr_Clear();

		if (!msg.empty()) {
			at::ConsoleStream::GetInstance()->Error(msg);
		}
	}

	PyEval_ReleaseThread(interp);

	return err;
}

std::string pyo_GetClassBriefDoc(PyThreadState* interp, const std::string& class_name)
{
	std::string output;
	PyEval_AcquireThread(interp);

	try {
		boost::python::object main_module = boost::python::import("__main__");
		boost::python::object globals = main_module.attr("__dict__");
		std::string content("from pyo import *;\ndef GetClassBrief(name):\n\treturn "
							"inspect.getdoc(globals()[name]).split('\\n\\n')[0];");

		boost::python::exec(content.c_str(), globals);
		boost::python::object brief = globals["GetClassBrief"](class_name.c_str());

		output = boost::python::extract<std::string>(brief);
	}
	catch (boost::python::error_already_set const&) {
		std::string msg;

		if (PyErr_Occurred()) {
			msg = handle_pyerror();
		}

		PyErr_Clear();

		if (!msg.empty()) {
			at::ConsoleStream::GetInstance()->Error(msg);
		}
	}

	PyEval_ReleaseThread(interp);

	return output;
}
