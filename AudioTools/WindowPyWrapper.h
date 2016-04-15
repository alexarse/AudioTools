//
//  WindowPyWrapper.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-15.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef WindowPyWrapper_hpp
#define WindowPyWrapper_hpp

#include <OpenAX/OpenAX.h>

namespace ax {
namespace python {

	class Window {
	public:
		Window(ax::Window* win);
		
		void SetPosition(const ax::Point& position);

	private:
		ax::Window* _win;
	};

	void export_python_wrapper_window();
}
}

#endif /* WindowPyWrapper_hpp */
