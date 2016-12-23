//
//  atEditorPyDocElement.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-19.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef atEditorPyDocElement_hpp
#define atEditorPyDocElement_hpp

#include <axlib/axlib.hpp>

namespace at {
namespace editor {

	class PyDocElement : public ax::Window::Backbone {
	public:
		PyDocElement(const ax::Rect& rect, const std::string& name, const std::string& description);

	private:
		ax::Font _font;
		ax::Font _font_normal;
		std::string _name;
		std::string _description;
		std::vector<std::string> _desc_content;

		void OnPaint(ax::GC gc);
	};
}
}

#endif /* atEditorPyDocElement_hpp */
