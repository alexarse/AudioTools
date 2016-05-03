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

#include <OpenAX/DropMenu.h>
#include <OpenAX/OpenAX.h>

namespace at {
namespace editor {
	/*
	 * GridWindow.
	 */
	class GridWindow : public ax::Window::Backbone {
	public:
		GridWindow(const ax::Rect& rect);

		enum : ax::Event::Id {
			UNSELECT_ALL = 999,
			SELECT_WIDGET = 1234,
			SELECT_MULTIPLE_WIDGET,
			DROP_WIDGET_MENU = 128973,
			SAVE_PANEL_TO_WORKSPACE = 84710,
			DELETE_SELECTED_WIDGET,
			DUPLICATE_SELECTED_WIDGET,
			ARROW_MOVE_SELECTED_WIDGET,
			
			BEGIN_DRAGGING_WIDGET,
			DONE_DRAGGING_WIDGET,
			
			DELETE_SELECTED_WIDGET_FROM_RIGHT_CLICK,
			DUPLICATE_SELECTED_WIDGET_FROM_RIGHT_CLICK
		};

		/// Set number of pixels between each grid lines.
		void SetGridSpace(const int& space);

		void SaveLayout(const std::string& path, const std::string& script_path);

		std::string OpenLayout(const std::string& path);

		void SetBackgroundColor(const ax::Color& color);

		void UnSelectAllWidgets();

		ax::Window* GetMainWindow();

		ax::Window* GetWidgetByName(const std::string& name);

		typedef std::pair<ax::Point, ax::Window*> PosAndWindow;

	private:
		int _grid_space;
		std::pair<bool, ax::Rect> _selection;
		ax::Color _bg_color;
		bool _right_click_menu;
		bool _draw_grid_over_children;
		
		std::vector<ax::FloatPoint> _lines_array;

		axEVENT_DECLARATION(ax::Event::SimpleMsg<PosAndWindow>, OnDropWidgetMenu);
		axEVENT_DECLARATION(ax::DropMenu::Msg, OnMenuChoice);
		axEVENT_DECLARATION(ax::Event::EmptyMsg, OnWidgetIsDragging);
		axEVENT_DECLARATION(ax::Event::EmptyMsg, OnWidgetDoneDragging);

		void OnGlobalClick(const ax::Window::Event::GlobalClick& gclick);
		void OnResize(const ax::Size& size);
		void OnAssignToWindowManager(const int& v);
		void OnPaintOverChildren(ax::GC gc);
		void OnKeyDown(const char& c);
		
		void OnLeftArrowDown(const char& c);
		void OnRightArrowDown(const char& c);
		void OnUpArrowDown(const char& c);
		void OnDownArrowDown(const char& c);

		
		void OnBackSpaceDown(const char& c);
		void OnMouseLeftDown(const ax::Point& pos);
		void OnMouseLeftDragging(const ax::Point& pos);
		void OnMouseLeftUp(const ax::Point& pos);
		void OnPaint(ax::GC gc);
	};
}
}
#endif // __MDI_GRID_WINDOW_H__
