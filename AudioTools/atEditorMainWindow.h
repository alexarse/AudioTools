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

#ifndef __AX_EDITOR_MAIN_WINDOW_H__
#define __AX_EDITOR_MAIN_WINDOW_H__

#include "atEditorWidgetMenu.h"
#include "atEditorGridWindow.h"
#include "atEditorWidgetMenu.h"
#include "atEditorStatusBar.h"
#include "atEditorInspectorMenu.h"
#include "atEditorBottomSection.h"

class CodeEditor;

namespace at {
	namespace editor {

class MainWindow : public ax::Window::Backbone {
public:
	MainWindow(const ax::Rect& rect);
	
	std::vector<ax::Window*> GetSelectedWindows() const;
	ax::Window* GetWidgetsByName(const std::string& name);
	
	void DeleteCurrentWidgets();
	
private:
	//	std::shared_ptr<StatusBar> _statusBar;
	static const int STATUS_BAR_HEIGHT = 30;
	static const int INSPECTOR_MENU_WIDTH = 250;
	static const int WIDGET_MENU_DROPPED_WIDTH = 85;
	static const int WIDGET_MENU_WIDTH = 250;
	static const int BOTTOM_BAR_HEIGHT = 18;
	
	StatusBar* _statusBar;
	std::shared_ptr<WidgetMenu> _widgetMenu;
	std::shared_ptr<GridWindow> _gridWindow;
	std::shared_ptr<InspectorMenu> _inspectorMenu;
	BottomSection* _bottom_section;
	
	std::vector<ax::Window*> _selected_windows;
	
	
	bool _has_tmp_widget;
	std::string _tmp_widget_builder_name;
	
	typedef std::pair<ax::StringPair, ax::Point> ObjMsg;
	
	struct ViewInfo {
		ax::Point old_main_window_position;
		bool left_menu_shown;
		bool right_menu_shown;
		bool editor_shown;
		ax::Size old_frame_size;
	};
	
	ViewInfo _view_info;
	bool _view_mode = false;
	
	axEVENT_DECLARATION(ax::Event::StringMsg, OnSaveLayout);
	axEVENT_DECLARATION(ax::Event::StringMsg, OnOpenLayout);
	axEVENT_DECLARATION(ax::Event::SimpleMsg<int>, OnViewLayout);
	axEVENT_DECLARATION(ax::Event::SimpleMsg<int>, OnReloadScript);
	axEVENT_DECLARATION(ax::Event::SimpleMsg<int>, OnStopScript);
	axEVENT_DECLARATION(ax::Button::Msg, OnBackToEditor);
	
	axEVENT_DECLARATION(ax::Event::SimpleMsg<int>, OnResizeCodeEditor);
	axEVENT_DECLARATION(ax::Toggle::Msg, OnToggleLeftPanel);
	axEVENT_DECLARATION(ax::Toggle::Msg, OnToggleBottomPanel);
	axEVENT_DECLARATION(ax::Toggle::Msg, OnToggleRightPanel);
	
	axEVENT_ACCESSOR(ax::Event::SimpleMsg<ObjMsg>, OnCreateDraggingWidget);
	axEVENT_ACCESSOR(ax::Event::SimpleMsg<ax::Point>, OnDraggingWidget);
	axEVENT_ACCESSOR(ax::Event::SimpleMsg<ax::Point>, OnReleaseObjWidget);
	
	axEVENT_DECLARATION(ax::Event::SimpleMsg<ax::Window*>, OnSelectWidget);
	axEVENT_DECLARATION(ax::Event::SimpleMsg<int>, OnUnSelectAllWidget);
	
	axEVENT_DECLARATION(ax::Button::Msg, OnSmallerLeftMenu);
	
	void OnCreateDraggingWidget(const ax::Event::SimpleMsg<ObjMsg>& msg);
	void OnDraggingWidget(const ax::Event::SimpleMsg<ax::Point>& msg);
	void OnReleaseObjWidget(const ax::Event::SimpleMsg<ax::Point>& msg);
	
	void OnResize(const ax::Size& size);
	
	void OnPaint(ax::GC gc);
};
}
}

#endif // __AX_EDITOR_MAIN_WINDOW_H__