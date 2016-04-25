//
//  atEditorPyDocElement.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-19.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef atEditorPyDocSeparator_hpp
#define atEditorPyDocSeparator_hpp

#include <OpenAX/OpenAX.h>
#include <OpenAX/Toggle.h>

#include "atEditorPyDocElement.h"

namespace at {
namespace editor {
	class PyDocSeparator : public ax::Window::Backbone {
	public:
		PyDocSeparator(const ax::Rect& rect, const std::string& name, const ax::StringPairVector& elements);
		
		enum Events : ax::Event::Id { NEED_RESIZE };

	private:
		ax::Font _font;
		std::string _name;
		std::string _description;
		std::vector<std::string> _desc_content;
		
		std::vector<PyDocElement*> _elements;
		
		axEVENT_DECLARATION(ax::Toggle::Msg, OnResizeSeparator);
		
		void OnPaint(ax::GC gc);
	};
}
}

#endif /* atEditorPyDocElement_hpp */
