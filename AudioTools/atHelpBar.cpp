#include "atEditor.h"
#include "atHelpBar.h"

namespace at {
namespace editor {
	void AttachHelpInfo(ax::Button* btn, const std::string& help_msg)
	{
		auto old_fct = btn->GetWindow()->event.OnMouseEnter.GetFunction();

		btn->GetWindow()->event.OnMouseEnter
			= ax::WFunc<ax::Point>([old_fct, help_msg](const ax::Point& pos) {
				  ax::Event::Object* main_obj = at::editor::App::GetInstance()->GetMainEvtObj();
				  main_obj->PushEvent(999, new ax::Event::StringMsg(help_msg));
				  old_fct(pos);
			  });
	}

	void AttachHelpInfo(ax::Toggle* tog, const std::string& help_msg)
	{
		auto old_fct = tog->GetWindow()->event.OnMouseEnter.GetFunction();

		tog->GetWindow()->event.OnMouseEnter
			= ax::WFunc<ax::Point>([old_fct, help_msg](const ax::Point& pos) {
				  ax::Event::Object* main_obj = at::editor::App::GetInstance()->GetMainEvtObj();
				  main_obj->PushEvent(999, new ax::Event::StringMsg(help_msg));
				  old_fct(pos);
			  });
	}
}
}