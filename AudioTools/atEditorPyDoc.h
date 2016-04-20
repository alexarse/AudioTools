//
//  atEditorPyoDoc.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-19.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef atEditorPyoDoc_hpp
#define atEditorPyoDoc_hpp

#include <OpenAX/OpenAX.h>
#include <OpenAX/ScrollBar.h>
#include "atEditorPyDocSeparator.h"

namespace at {
namespace editor {

	class PyDoc : public ax::Window::Backbone {
	public:
		PyDoc(const ax::Rect& rect);

	private:
		std::vector<PyDocSeparator*> _separators;
		ax::Window* _scroll_panel;
		ax::ScrollBar::Ptr _scrollBar;
		
		ax::Point AddSeparator(const ax::Point& pos, const std::string& name, const ax::StringVector& args);
	
		axEVENT_DECLARATION(ax::Event::EmptyMsg, OnNeedResize);
	
		void OnPaint(ax::GC gc);
	};
}
}

#endif /* atEditorPyoDoc_hpp */
