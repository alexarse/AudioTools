#include "atHelpBar.h"
#include "editor/atEditor.hpp"

namespace at {
namespace editor {
	void AttachHelpInfo(ax::Window* window, const std::string& help_msg)
	{
		auto old_fct = window->event.OnMouseEnter.GetFunction();

		window->event.OnMouseEnter = ax::WFunc<ax::Point>([old_fct, help_msg](const ax::Point& pos) {
			ax::event::Object* main_obj = at::editor::App::GetInstance()->GetMainEvtObj();
			main_obj->PushEvent(999, new ax::event::StringMsg(help_msg));

			if (old_fct) {
				old_fct(pos);
			}
		});

		auto old_leave_fct = window->event.OnMouseLeave.GetFunction();

		window->event.OnMouseLeave = ax::WFunc<ax::Point>([old_leave_fct](const ax::Point& pos) {

			ax::event::Object* main_obj = at::editor::App::GetInstance()->GetMainEvtObj();
			main_obj->PushEvent(999, new ax::event::StringMsg(""));

			if (old_leave_fct) {
				old_leave_fct(pos);
			}
		});
	}
}
}
