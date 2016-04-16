//
//  m_pyo.cpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-15.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#include "m_pyo.h"
#include "atConsoleStream.h"

std::string handle_pyerror()
{
	using namespace boost::python;
	using namespace boost;

	PyObject *exc, *val, *tb;
	object formatted_list, formatted;
	PyErr_Fetch(&exc, &val, &tb);
	handle<> hexc(exc), hval(allow_null(val)), htb(allow_null(tb));
	
	object traceback(import("traceback"));
	
	if (!tb) {
		object format_exception_only(traceback.attr("format_exception_only"));
		formatted_list = format_exception_only(hexc, hval);
	}
	else {
		object format_exception(traceback.attr("format_exception"));
		formatted_list = format_exception(hexc, hval, htb);
	}
	
	formatted = str("").join(formatted_list);
	
	return extract<std::string>(formatted);
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
		
		if(!msg.empty()) {
			at::ConsoleStream::GetInstance()->Write(msg);
		}
	}
	
	PyEval_ReleaseThread(interp);

	return err;
}

