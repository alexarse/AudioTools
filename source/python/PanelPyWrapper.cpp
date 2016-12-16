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

#include "python/PanelPyWrapper.hpp"
#include <Python/Python.h>
#include <boost/python.hpp>
#include <cstdio>

#include <OpenAX/OpenAX.h>
#include <OpenAX/Panel.h>

using namespace boost::python;

namespace ax {
namespace python {

	Panel::Panel(ax::Panel* panel)
		: _panel(panel)
	{
	}

	void Panel::SetBackgroundColor(const ax::Color& color)
	{
		widget::Component* widget
			= static_cast<widget::Component*>(_panel->GetWindow()->component.Get("Widget").get());
		ax::Panel::Info* info = static_cast<ax::Panel::Info*>(widget->GetInfo());

		info->background = color;
		widget->ReloadInfo();
	}

	void Panel::SetContourColor(const ax::Color& color)
	{
		widget::Component* widget
			= static_cast<widget::Component*>(_panel->GetWindow()->component.Get("Widget").get());
		ax::Panel::Info* info = static_cast<ax::Panel::Info*>(widget->GetInfo());

		info->contour = color;
		widget->ReloadInfo();
	}

	void export_python_wrapper_panel()
	{
		class_<ax::python::Panel>("Panel", init<ax::Panel*>())
			.def("SetBackgroundColor", &ax::python::Panel::SetBackgroundColor)
			.def("SetContourColor", &ax::python::Panel::SetContourColor);
	}
}
}
