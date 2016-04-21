//
//  atEditorWorkspace.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-21.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef atEditorWorkspace_hpp
#define atEditorWorkspace_hpp

#include <OpenAX/OpenAX.h>

namespace at {
namespace editor {
	class Workspace : public ax::Window::Backbone {
	public:
		Workspace(const ax::Rect& rect);

	private:
		ax::Font _font;
		ax::Font _font_bold;
		void OnPaint(ax::GC gc);
	};
}
}

#endif /* atEditorWorkspace_hpp */
