//
//  atEditorWorkspace.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-21.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef atEditorWorkspace_hpp
#define atEditorWorkspace_hpp

#include <axlib/ScrollBar.hpp>
#include <axlib/axlib.hpp>

#include "editor/atEditorWorkspaceObj.hpp"

namespace at {
namespace editor {
	class Workspace : public ax::Window::Backbone {
	public:
		Workspace(const ax::Rect& rect);

	private:
		ax::Font _font;
		ax::Font _font_bold;

		ax::Window* _panel;

		ax::ScrollBar::Ptr _scrollBar;
		std::vector<std::shared_ptr<WorkspaceObj>> _objs;

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

#endif /* atEditorWorkspace_hpp */
