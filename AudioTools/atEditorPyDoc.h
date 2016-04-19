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

namespace at {
namespace editor {

	class PyDoc : public ax::Window::Backbone {
	public:
		PyDoc(const ax::Rect& rect);

	private:
		void OnPaint(ax::GC gc);
	};
}
}

#endif /* atEditorPyoDoc_hpp */
