//
//  atEditorAccount.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-19.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef atEditorAccount_hpp
#define atEditorAccount_hpp

#include <axlib/axlib.hpp>

namespace at {
namespace editor {
	class Account : public ax::Window::Backbone {
	public:
		Account(const ax::Rect& rect);

	private:
		ax::Font _font;
		ax::Font _font_bold;
		void OnPaint(ax::GC gc);
	};
}
}

#endif /* atEditorAccount_hpp */
