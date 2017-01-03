/*
 * Copyright (c) 2016 AudioTools - All Rights Reserved
 *
 * This Software may not be distributed in parts or its entirety
 * without prior written agreement by AudioTools.
 *
 * Neither the name of the AudioTools nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY AUDIOTOOLS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL AUDIOTOOLS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Written by Alexandre Arsenault <alx.arsenault@gmail.com>
 */

#include "python/NumberBoxPyWrapper.hpp"
#include <Python/Python.h>
#include <boost/python.hpp>
#include <cstdio>

using namespace boost::python;

namespace ax {
namespace python {

	NumberBox::NumberBox(ax::NumberBox* nbox)
		: ax::python::Window(nbox->GetWindow())
		, _number_box(nbox)
	{
	}

	double NumberBox::GetValue() const
	{
		return _number_box->GetValue();
	}

	void NumberBox::SetValue(double value)
	{
		_number_box->SetValue(value);
	}

	void export_python_wrapper_number_box()
	{
		class_<ax::python::NumberBox, boost::python::bases<ax::python::Window>>(
			"NumberBox", init<ax::NumberBox*>())
			.def("SetValue", &ax::python::NumberBox::SetValue)
			.def("GetValue", &ax::python::NumberBox::GetValue);
	}
}
}
