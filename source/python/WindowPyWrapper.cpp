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

#include "python/WindowPyWrapper.hpp"
#include <Python/Python.h>
#include <boost/python.hpp>
#include <cstdio>

using namespace boost::python;

namespace ax {
namespace python {

	Window::Window(ax::Window* win)
		: _win(win)
	{
	}

	void Window::SetPosition(const ax::Point& position)
	{
		_win->dimension.SetPosition(position);
	}

	void Window::SetSize(const ax::Size& size)
	{
		_win->dimension.SetSize(size);
	}

	ax::Point Window::GetPosition()
	{
		return _win->dimension.GetRect().position;
	}

	ax::Size Window::GetSize()
	{
		return _win->dimension.GetSize();
	}

	ax::Rect Window::GetDrawingRect()
	{
		// boost::python::call_method<int>();
		return _win->dimension.GetDrawingRect();
	}

	void Window::Update()
	{
		_win->Update();
	}

	void export_python_wrapper_window()
	{
		class_<ax::python::Window>("Window", init<ax::Window*>())
			.def("SetPosition", &ax::python::Window::SetPosition, boost::python::arg("pos"))
			.def("SetSize", &ax::python::Window::SetSize, boost::python::arg("size"))
			.def("GetPosition", &ax::python::Window::GetPosition)
			.def("GetSize", &ax::python::Window::GetSize)
			.def("GetDrawingRect", &ax::python::Window::GetDrawingRect)
			.def("Update", &ax::python::Window::Update);
	}
}
}
