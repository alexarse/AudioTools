//
//  atEditorPyDocElement.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-19.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef atEditorPyDocSeparator_hpp
#define atEditorPyDocSeparator_hpp

#include <axlib/Toggle.hpp>
#include <axlib/axlib.hpp>

#include "editor/atEditorPyDocElement.hpp"

namespace at {
namespace editor {
	class PyDocSeparator : public ax::Window::Backbone {
	public:
		PyDocSeparator(const ax::Rect& rect, const std::string& name,
			const std::vector<std::pair<std::string, std::string>>& elements);

		enum Events : ax::event::Id { NEED_RESIZE };

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
