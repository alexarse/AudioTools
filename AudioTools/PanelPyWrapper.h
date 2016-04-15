//
//  PanelPyWrapper.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-15.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef PanelPyWrapper_hpp
#define PanelPyWrapper_hpp

#include <OpenAX/OpenAX.h>
#include <OpenAX/Panel.h>

namespace ax {
namespace python {

	class Panel {
	public:
		Panel(ax::Panel* panel);

		void SetBackgroundColor(const ax::Color& color);

		void SetContourColor(const ax::Color& color);

	private:
		ax::Panel* _panel;
	};

	void export_python_wrapper_panel();
}
}

#endif /* PanelPyWrapper_hpp */
