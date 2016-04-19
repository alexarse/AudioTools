//
//  m_pyo.cpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-15.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#include "atConsoleStream.h"
#include "m_pyo.h"

std::string handle_pyerror()
{
	//	using namespace boost::python;
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

	try {
		boost::python::object main_module = boost::python::import("__main__");
		boost::python::object globals = main_module.attr("__dict__");

		ax::python::InitWrapper();

		globals["widgets"] = boost::python::ptr(ax::python::Widgets::GetInstance().get());

		boost::python::object ignored = boost::python::exec_file(file, globals);
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

	try {
		boost::python::object main_module = boost::python::import("__main__");
		boost::python::object globals = main_module.attr("__dict__");

		globals["widgets"] = boost::python::ptr(ax::python::Widgets::GetInstance().get());
		boost::python::object ignored = boost::python::exec(msg, globals);
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