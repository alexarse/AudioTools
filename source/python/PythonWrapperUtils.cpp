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

#include "python/PythonWrapperUtils.hpp"
#include <Python/Python.h>
#include <axlib/axlib.hpp>
#include <boost/python.hpp>
#include <cstdio>

using namespace boost::python;

namespace ax {
namespace python {
	void export_python_wrapper_utils()
	{
		// ax;:Point.
		class_<ax::Point>("Point", init<int, int>())
			.def(init<>())
			.def_readwrite("x", &ax::Point::x)
			.def_readwrite("y", &ax::Point::y)
			.def(self + self)
			.def(self - self)
			.def(self * self)
			.def(self += self)
			.def(self -= self)
			.def(self *= other<double>());

		// ax::Size.
		class_<ax::Size>("Size", init<int, int>())
			.def(init<>())
			.def_readwrite("w", &ax::Size::w)
			.def_readwrite("h", &ax::Size::h)
			.def(self + self)
			.def(self - self)
			.def(self * self)
			.def(self += self)
			.def(self -= self)
			.def(self *= other<double>());

		// ax::Rect.
		class_<ax::Rect>("Rect", init<int, int, int, int>())
			.def_readwrite("position", &ax::Rect::position)
			.def_readwrite("size", &ax::Rect::size);

		// ax::Color.
		class_<ax::Color>("Color", init<int, int, int, int>())
			.def(init<int, int, int>())
			.def(init<float, float, float, float>())
			.def(init<float, float, float>())
			.add_property("r", &ax::Color::GetRed, &ax::Color::SetRed)
			.add_property("g", &ax::Color::GetGreen, &ax::Color::SetGreen)
			.add_property("b", &ax::Color::GetBlue, &ax::Color::SetBlue)
			.add_property("a", &ax::Color::GetAlpha, &ax::Color::SetAlpha);

		// ax::GC.
		//		class_<ax::GC>("GC", init<>())
		////		.def("SetColor", &ax::GC::SetColor)
		//		.def("DrawRectangle", &ax::GC::DrawRectangle);
	}
}
}
