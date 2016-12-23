//
//  atEditorProjectInfo.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-22.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef atEditorProjectInfo_hpp
#define atEditorProjectInfo_hpp

#include <axlib/axlib.hpp>

namespace at {
namespace editor {
	class ProjectInfo : public ax::Window::Backbone {
	public:
		ProjectInfo(const ax::Rect& rect);

	private:
		ax::Font _font;
		ax::Font _font_bold;
		void OnPaint(ax::GC gc);
	};
}
}

#endif /* atEditorProjectInfo_hpp */
