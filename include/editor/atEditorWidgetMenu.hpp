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

#ifndef mdiWidgetMenu_hpp
#define mdiWidgetMenu_hpp

#include <axlib/axlib.hpp>

#include <axlib/Button.hpp>
#include <axlib/ScrollBar.hpp>

#include "editor/atEditorWidgetMenuObj.hpp"
#include "editor/atEditorWidgetMenuSeparator.hpp"

namespace at {
namespace editor {
	class WidgetMenu : public ax::Window::Backbone {
	public:
		WidgetMenu(const ax::Rect& rect);

		enum : ax::event::Id { SMALLER_MENU };

		void SetOnlyMainWindowWidgetSelectable();
		void SetAllSelectable();

		void SetSmall();

		void SetWide();

	private:
		ax::Window* _panel;
		ax::ScrollBar::Ptr _scrollBar;
		std::vector<std::shared_ptr<WidgetMenuObj>> _objs;

		std::vector<WidgetMenuInfo> GetWidgetsInfo();
		//		std::vector<std::string> GetBuilderList(const std::vector<WidgetMenuInfo>& w_info);

		void OnMouseEnter(const ax::Point& pos);
		void OnMouseLeave(const ax::Point& pos);
		void OnMouseEnterChild(const ax::Point& pos);
		void OnMouseLeaveChild(const ax::Point& pos);
		void OnScrollWheel(const ax::Point& delta);
		void OnResize(const ax::Size& size);
		void OnPaint(ax::GC gc);
	};
}
}

#endif /* mdiWidgetMenu_hpp */
