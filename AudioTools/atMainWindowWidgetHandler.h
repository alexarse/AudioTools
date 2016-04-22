//
//  atMainWindowWidgetHandler.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-22.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef atMainWindowWidgetHandler_hpp
#define atMainWindowWidgetHandler_hpp

#include <OpenAX/OpenAX.h>

namespace at {
namespace editor {

	class MainWindow;

	class MainWindowWidgetHandler {
	public:
		MainWindowWidgetHandler(MainWindow* main_window);

		typedef std::pair<ax::StringPair, ax::Point> ObjMsg;

		axEVENT_DECLARATION(ax::Event::SimpleMsg<ObjMsg>, OnCreateDraggingWidget);
		axEVENT_DECLARATION(ax::Event::SimpleMsg<ax::Point>, OnDraggingWidget);
		axEVENT_DECLARATION(ax::Event::SimpleMsg<ax::Point>, OnReleaseObjWidget);
		axEVENT_DECLARATION(ax::Event::SimpleMsg<ax::Window*>, OnSelectWidget);
		axEVENT_DECLARATION(ax::Event::SimpleMsg<int>, OnUnSelectAllWidget);

	private:
		MainWindow* _main_window;
		
		bool _has_tmp_widget;
		std::string _tmp_widget_builder_name;
	};
}
}

#endif /* atMainWindowWidgetHandler_hpp */
