#include "atEditor.h"
#include "atHelpBar.h"

namespace at {
namespace editor {
//	void AttachHelpInfo(ax::Button* btn, const std::string& help_msg)
//	{
//		auto old_fct = btn->GetWindow()->event.OnMouseEnter.GetFunction();
//
//		btn->GetWindow()->event.OnMouseEnter
//			= ax::WFunc<ax::Point>([old_fct, help_msg](const ax::Point& pos) {
//				  ax::Event::Object* main_obj = at::editor::App::GetInstance()->GetMainEvtObj();
//				  main_obj->PushEvent(999, new ax::Event::StringMsg(help_msg));
//				  old_fct(pos);
//			  });
//	}
//
//	void AttachHelpInfo(ax::Toggle* tog, const std::string& help_msg)
//	{
//		auto old_fct = tog->GetWindow()->event.OnMouseEnter.GetFunction();
//
//		tog->GetWindow()->event.OnMouseEnter
//			= ax::WFunc<ax::Point>([old_fct, help_msg](const ax::Point& pos) {
//				  ax::Event::Object* main_obj = at::editor::App::GetInstance()->GetMainEvtObj();
//				  main_obj->PushEvent(999, new ax::Event::StringMsg(help_msg));
//				  old_fct(pos);
//			  });
//	}
	
	void AttachHelpInfo(ax::Window* window, const std::string& help_msg)
	{
		auto old_fct = window->event.OnMouseEnter.GetFunction();
		
		window->event.OnMouseEnter
		= ax::WFunc<ax::Point>([old_fct, help_msg](const ax::Point& pos) {
			ax::Event::Object* main_obj = at::editor::App::GetInstance()->GetMainEvtObj();
			main_obj->PushEvent(999, new ax::Event::StringMsg(help_msg));
			
			if(old_fct) {
				old_fct(pos);
			}
		});
		
		auto old_leave_fct = window->event.OnMouseLeave.GetFunction();
		
		window->event.OnMouseLeave
		= ax::WFunc<ax::Point>([old_leave_fct](const ax::Point& pos) {
			
			ax::Event::Object* main_obj = at::editor::App::GetInstance()->GetMainEvtObj();
			main_obj->PushEvent(999, new ax::Event::StringMsg(""));
			
			if(old_leave_fct) {
				old_leave_fct(pos);
			}
		});
	}
}
}