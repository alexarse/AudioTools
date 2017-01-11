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

#include "python/PythonWrapper.hpp"
#include "editor/atEditor.hpp"
#include "editor/atEditorMainWindow.hpp"
#include "python/ButtonPyWrapper.hpp"
#include "python/GCPyWrapper.hpp"
#include "python/KnobPyWrapper.hpp"
#include "python/NumberBoxPyWrapper.hpp"
#include "python/PanelPyWrapper.hpp"
#include "python/SpritePyWrapper.hpp"
#include "python/WindowPyWrapper.hpp"

namespace ax {
namespace python {
	boost::python::object GetWidgetByName(const std::string& widget_name)
	{
		ax::Window* win = at::editor::App::GetInstance()->GetMainWindow()->GetWidgetsByName(widget_name);

		if (win == nullptr) {
			return boost::python::object();
		}

		widget::Component::Ptr widget = win->component.Get<widget::Component>("Widget");
		const std::string builder_name(widget->GetBuilderName());

		/// @todo Do this dynamically.

		if (builder_name == "Panel") {
			ax::Panel* panel = static_cast<ax::Panel*>(win->backbone.get());
			return boost::python::object(ax::python::Panel(panel));
		}
		else if (builder_name == "Button") {
			ax::Button* btn = static_cast<ax::Button*>(win->backbone.get());
			return boost::python::object(ax::python::Button(btn));
		}
		else if (builder_name == "NumberBox") {
			return boost::python::object(ax::python::NumberBox(win->GetBackbone<ax::NumberBox>()));
		}
		else if (builder_name == "Knob") {
			ax::Knob* knob = static_cast<ax::Knob*>(win->backbone.get());
			return boost::python::object(ax::python::Knob(knob));
		}
		else if (builder_name == "Sprite") {
			ax::Sprite* sprite = static_cast<ax::Sprite*>(win->backbone.get());
			return boost::python::object(ax::python::Sprite(sprite));
		}

		return boost::python::object();
	}

	std::string OpenFileDialog()
	{
		return ax::App::GetInstance().OpenFileDialog();
	}
}
}
BOOST_PYTHON_MODULE(ax)
{
	// Create ax::Window python wrapper.
	ax::python::export_python_wrapper_window();

	ax::python::export_python_wrapper_utils();

	// Create ax::Panel python wrapper.
	ax::python::export_python_wrapper_panel();

	// Create ax::Button python wrapper.
	ax::python::export_python_wrapper_button();

	ax::python::export_python_wrapper_sprite();

	ax::python::export_python_wrapper_number_box();

	ax::python::export_python_wrapper_knob();

	boost::python::class_<ax::python::Widgets>("Widgets").def("Get", &ax::python::Widgets::Get);

	//
	boost::python::def("OpenFileDialog", ax::python::OpenFileDialog);
	boost::python::def("GetWidgetByName", ax::python::GetWidgetByName, boost::python::arg("name"));

	ax::python::export_python_wrapper_gc();
}

namespace ax {
namespace python {
	std::shared_ptr<ax::python::Widgets> Widgets::instance = nullptr;

	std::shared_ptr<ax::python::Widgets> Widgets::GetInstance()
	{
		if (instance == nullptr) {
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

		if (win == nullptr) {
			return boost::python::object();
		}

		widget::Component::Ptr widget = win->component.Get<widget::Component>("Widget");
		const std::string builder_name(widget->GetBuilderName());

		/// @todo Do this dynamically.

		if (builder_name == "Panel") {
			ax::Panel* panel = static_cast<ax::Panel*>(win->backbone.get());
			return boost::python::object(ax::python::Panel(panel));
		}
		else if (builder_name == "Button") {
			ax::Button* btn = static_cast<ax::Button*>(win->backbone.get());
			return boost::python::object(ax::python::Button(btn));
		}
		else if (builder_name == "NumberBox") {
			return boost::python::object(ax::python::NumberBox(win->GetBackbone<ax::NumberBox>()));
		}
		else if (builder_name == "Knob") {
			ax::Knob* knob = static_cast<ax::Knob*>(win->backbone.get());
			return boost::python::object(ax::python::Knob(knob));
		}
		else if (builder_name == "Sprite") {
			ax::Sprite* sprite = static_cast<ax::Sprite*>(win->backbone.get());
			return boost::python::object(ax::python::Sprite(sprite));
		}

		return boost::python::object(boost::python::ptr(_pt.get()));
	}

	void InitWrapper()
	{
		initax();
	}
}
}
