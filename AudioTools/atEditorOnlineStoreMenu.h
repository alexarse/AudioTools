//
//  atEditorStore.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-21.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef atEditorStore_hpp
#define atEditorStore_hpp

#include <OpenAX/OpenAX.h>

namespace at {
namespace editor {
	class OnlineStoreMenu : public ax::Window::Backbone {
	public:
		OnlineStoreMenu(const ax::Rect& rect);

	private:
		ax::Font _font;
		ax::Font _font_bold;
		void OnPaint(ax::GC gc);
	};
}
}

#endif /* atEditorStore_hpp */
