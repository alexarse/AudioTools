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

#pragma once

#include "editor/atEditorBottomSection.hpp"
#include "editor/atEditorGridWindow.hpp"
#include "editor/atEditorLeftSideMenu.hpp"
#include "editor/atEditorRightSideMenu.hpp"
#include "editor/atEditorStatusBar.hpp"
#include "editor/atEditorWidgetMenu.hpp"
#include "widget/atMidiFeedback.hpp"

#include "dialog/atSaveWorkDialog.hpp"
#include "project/atProjectManager.hpp"

#include "atMainWindowProjectHandler.h"
#include "atMainWindowViewHandler.h"
#include "atMainWindowWidgetHandler.h"
#include "editor/GridSnapProxy.hpp"

class CodeEditor;

namespace at {
namespace editor {

	class MainWindow : public ax::Window::Backbone {
	public:
		MainWindow(const ax::Rect& rect, const std::string& proj_path = "");

		std::vector<ax::Window*> GetSelectedWindows() const;
		ax::Window* GetWidgetsByName(const std::string& name);

		static const int STATUS_BAR_HEIGHT = 30;
		static const int INSPECTOR_MENU_WIDTH = 250;
		static const int WIDGET_MENU_DROPPED_WIDTH = 85;
		static const int WIDGET_MENU_WIDTH = 250;
		static const int BOTTOM_BAR_HEIGHT = 18;

		enum MainWindowEvents : ax::event::Id { HAS_WIDGET_ON_GRID = 38923 };

		inline GridWindow* GetGridWindow()
		{
			return _gridWindow.get();
		}

		GridSnapProxy GetGridSnapProxy() const
		{
			return GridSnapProxy(_gridWindow.get());
		}

		MainWindowProjectHandler* GetProjectHandler()
		{
			return &_project_handler;
		}

	private:
		ax::Font _font;

		StatusBar* _statusBar;
		std::shared_ptr<GridWindow> _gridWindow;
		LeftSideMenu* _left_menu;
		RightSideMenu* _right_menu;
		BottomSection* _bottom_section;
		at::MidiFeedback* _midi_feedback;
		bool _need_to_save_widget_img_on_paint = false;

		std::vector<ax::Window*> _selected_windows;

		std::string _help_bar_str;

		typedef std::pair<std::pair<std::string, std::string>, ax::Point> ObjMsg;

		at::ProjectManager _project;

		friend class MainWindowViewHandler;
		MainWindowViewHandler _view_handler;

		friend class MainWindowWidgetHandler;
		MainWindowWidgetHandler _widget_handler;

		friend class MainWindowProjectHandler;
		MainWindowProjectHandler _project_handler;

		axEVENT_DECLARATION(ax::event::SimpleMsg<int>, OnReloadScript);
		axEVENT_DECLARATION(ax::event::SimpleMsg<int>, OnStopScript);

		axEVENT_DECLARATION(ax::event::EmptyMsg, OnSavePanelToWorkspace);
		axEVENT_DECLARATION(at::SaveWorkPanel::Msg, OnAcceptSavePanelToWorkpace);
		axEVENT_DECLARATION(ax::event::EmptyMsg, OnCancelSavePanelToWorkpace);

		axEVENT_DECLARATION(ax::event::EmptyMsg, OnRemoveWidgetFromRightClickMenu);
		axEVENT_DECLARATION(ax::event::EmptyMsg, OnDuplicateWidgetFromRightClickMenu);
		axEVENT_DECLARATION(ax::event::EmptyMsg, OnSnapToGridWidgetFromRightClickMenu);

		axEVENT_DECLARATION(ax::event::StringMsg, OnHelpBar);

		void OnGlobalKey(const char& c);
		void OnAssignToWindowManager(const int& v);

		void OnPaint(ax::GC gc);
		void OnPaintOverChildren(ax::GC gc);
	};
}
}
