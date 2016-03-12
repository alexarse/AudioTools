////
////  mdiWidget.cpp
////  eosMidiController
////
////  Created by Alexandre Arsenault on 2016-02-19.
////  Copyright © 2016 ax. All rights reserved.
////
//
//#include "mdiControl.hpp"
//
//namespace mdi {
//namespace ctrl {
//	void SetupControl(ax::Window* win)
//	{
//		auto m_down_fct = win->event.OnMouseLeftDown.GetFunction();
//		auto m_drag_fct = win->event.OnMouseLeftDragging.GetFunction();
//		auto m_up_fct = win->event.OnMouseLeftUp.GetFunction();
//		auto m_right_down = win->event.OnMouseRightDown.GetFunction();
//
//		win->event.OnMouseLeftDown = ax::WFunc<ax::Point>([win, m_down_fct](const ax::Point& pos) {
//			bool cmd_down = ax::App::GetInstance().GetWindowManager()->IsCmdDown();
//
//			if (cmd_down) {
//				ax::Point c_delta(pos - win->dimension.GetAbsoluteRect().position);
//				win->resource.Add("click_delta", c_delta);
//				win->event.GrabMouse();
//				win->property.AddProperty("edit_click");
//			}
//			else {
//				if (m_down_fct) {
//					m_down_fct(pos);
//				}
//			}
//		});
//
//		win->event.OnMouseLeftDragging = ax::WFunc<ax::Point>([win, m_drag_fct](const ax::Point& pos) {
////			if (win->event.IsGrabbed()) {
//				//					win->resource.
//				if (win->property.HasProperty("edit_click")) {
//
//					ax::Point c_delta = win->resource.GetResource("click_delta");
//					win->dimension.SetPosition(
//						pos - win->node.GetParent()->dimension.GetAbsoluteRect().position - c_delta);
//				} else {
////			}
//
//			if (m_drag_fct) {
//				m_drag_fct(pos);
//			}
//			}
//		});
//
//		win->event.OnMouseLeftUp = ax::WFunc<ax::Point>([win, m_up_fct](const ax::Point& pos) {
//
//			if(win->property.HasProperty("edit_click")) {
//				win->property.RemoveProperty("edit_click");
//				
//				if (win->event.IsGrabbed()) {
//					win->event.UnGrabMouse();
//				}
//			}
//			else {
//				if (m_up_fct) {
//					m_up_fct(pos);
//				}
//			}
//			
//		});
//
//		win->event.OnMouseRightDown = ax::WFunc<ax::Point>([win, m_right_down](const ax::Point& pos) {
//			ax::Print("RIGHT DOWN");
//			//				if (win->event.IsGrabbed()) {
//			//					win->event.UnGrabMouse();
//			//				}
//			//
//			win->Hide();
//			win->DeleteWindow();
//
//			if (m_right_down) {
//				m_right_down(pos);
//			}
//		});
//	}
//}
//}