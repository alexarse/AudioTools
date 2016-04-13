/*
 * Copyright (c) 2016 AudioTools - All Rights Reserved
 *
 * This Software may not be distributed in parts or its entirety
 * without prior written agreement by AutioTools.
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
 
#ifndef __MDI_GRID_WINDOW_H__
#define __MDI_GRID_WINDOW_H__

#include <OpenAX/OpenAX.h>

namespace at {
namespace editor {
	/*
	 * GridWindow.
	 * @brief All child ax::Window are suppose to be mdi::Widget.
	 */
	class GridWindow : public ax::Window::Backbone {
	public:
		GridWindow(const ax::Rect& rect);
		
		enum : ax::Event::Id
		{
			UNSELECT_ALL = 999,
			SELECT_WIDGET = 1234//,
//			GRID_HAS_NEW_MAIN_WINDOW = 19281
		};

		/// Set number of pixels between each grid lines.
		void SetGridSpace(const int& space);

		void SaveLayout(const std::string& path, const std::string& script_path);

		std::string OpenLayout(const std::string& path);
		
		void SetBackgroundColor(const ax::Color& color);
		
		void UnSelectAllWidgets();
		
		ax::Window* GetMainWindow();
		
	private:
		int _grid_space;
		std::pair<bool, ax::Rect> _selection;
		ax::Color _bg_color;
		
//		void SetupControl(ax::Window* win);
		////		{
		////			auto m_down_fct = win->event.OnMouseLeftDown.GetFunction();
		////			auto m_drag_fct = win->event.OnMouseLeftDragging.GetFunction();
		////			auto m_up_fct = win->event.OnMouseLeftUp.GetFunction();
		////			auto m_right_down = win->event.OnMouseRightDown.GetFunction();
		////
		////			win->event.OnMouseLeftDown = ax::WFunc<ax::Point>([win, m_down_fct](const ax::Point& pos) {
		////				bool cmd_down = ax::App::GetInstance().GetWindowManager()->IsCmdDown();
		////
		////				if (cmd_down) {
		////					ax::Point c_delta(pos - win->dimension.GetAbsoluteRect().position);
		////					win->resource.Add("click_delta", c_delta);
		////					win->event.GrabMouse();
		////					win->property.AddProperty("edit_click");
		////				}
		////				else {
		////					if (m_down_fct) {
		////						m_down_fct(pos);
		////					}
		////				}
		////			});
		////
		////			win->event.OnMouseLeftDragging = ax::WFunc<ax::Point>([win, m_drag_fct](const ax::Point& pos) {
		////				if (win->event.IsGrabbed()) {
		////					//					win->resource.
		////					if (win->property.HasProperty("edit_click")) {
		////
		////						ax::Point c_delta = win->resource.GetResource("click_delta");
		////						win->dimension.SetPosition(
		////							pos - win->node.GetParent()->dimension.GetAbsoluteRect().position - c_delta);
		////					}
		////				}
		////
		////				if (m_drag_fct) {
		////					m_drag_fct(pos);
		////				}
		////			});
		////
		////			win->event.OnMouseLeftUp = ax::WFunc<ax::Point>([win, m_up_fct](const ax::Point& pos) {
		////
		////				win->property.RemoveProperty("edit_click");
		////
		////				if (m_up_fct) {
		////					m_up_fct(pos);
		////				}
		////
		////				if (win->event.IsGrabbed()) {
		////					win->event.UnGrabMouse();
		////				}
		////			});
		////
		////			win->event.OnMouseRightDown = ax::WFunc<ax::Point>([win, m_right_down](const ax::Point& pos) {
		////				ax::Print("RIGHT DOWN");
		////				//				if (win->event.IsGrabbed()) {
		////				//					win->event.UnGrabMouse();
		////				//				}
		////				//
		////				win->Hide();
		////				win->DeleteWindow();
		////
		////				if (m_right_down) {
		////					m_right_down(pos);
		////				}
		////			});
		////		}

		void OnMouseLeftDown(const ax::Point& pos);
		void OnMouseLeftDragging(const ax::Point& pos);
		void OnMouseLeftUp(const ax::Point& pos);
		void OnPaint(ax::GC gc);
		
		
	};
}
}
#endif // __MDI_GRID_WINDOW_H__
