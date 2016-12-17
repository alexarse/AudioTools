//
//  atEditorPyoDoc.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-19.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef atEditorPyoDoc_hpp
#define atEditorPyoDoc_hpp

#include "editor/atEditorPyDocSeparator.hpp"
#include <axlib/ScrollBar.hpp>
#include <axlib/axlib.hpp>

namespace at {
namespace editor {

	class PyDoc : public ax::Window::Backbone {
	public:
		PyDoc(const ax::Rect& rect);

	private:
		std::vector<PyDocSeparator*> _separators;
		ax::Window* _scroll_panel;
		ax::ScrollBar::Ptr _scrollBar;

		ax::Point AddSeparator(
			const ax::Point& pos, const std::string& name, const std::vector<std::string>& args);

		axEVENT_DECLARATION(ax::event::EmptyMsg, OnNeedResize);

		void OnMouseEnter(const ax::Point& pos);
		void OnMouseLeave(const ax::Point& pos);
		void OnMouseEnterChild(const ax::Point& pos);
		void OnScrollWheel(const ax::Point& delta);
		void OnMouseLeaveChild(const ax::Point& pos);
		void OnResize(const ax::Size& size);
		void OnPaint(ax::GC gc);
	};
}
}

#endif /* atEditorPyoDoc_hpp */
