//
//  atMainWindowWidgetHandler.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-22.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef atMainWindowWidgetHandler_hpp
#define atMainWindowWidgetHandler_hpp

#include <axlib/axlib.hpp>

namespace at {
namespace editor {

	class MainWindow;

	class MainWindowWidgetHandler {
	public:
		MainWindowWidgetHandler(MainWindow* main_window);

		void DeleteCurrentWidgets();

		typedef std::pair<std::pair<std::string, std::string>, ax::Point> ObjMsg;

		axEVENT_DECLARATION(ax::event::SimpleMsg<ObjMsg>, OnCreateDraggingWidget);
		axEVENT_DECLARATION(ax::event::SimpleMsg<ObjMsg>, OnCreateCustomDraggingWidget);
		axEVENT_DECLARATION(ax::event::SimpleMsg<ax::Point>, OnDraggingWidget);
		axEVENT_DECLARATION(ax::event::SimpleMsg<ax::Point>, OnReleaseObjWidget);
		axEVENT_DECLARATION(ax::event::SimpleMsg<ax::Window*>, OnSelectWidget);
		axEVENT_DECLARATION(ax::event::SimpleMsg<int>, OnUnSelectAllWidget);
		axEVENT_DECLARATION(ax::event::EmptyMsg, OnDeleteSelectedWidget);
		axEVENT_DECLARATION(ax::event::EmptyMsg, OnDuplicateSelectedWidget);
		axEVENT_DECLARATION(ax::event::SimpleMsg<ax::util::Direction>, OnArrowMoveSelectedWidget);
		axEVENT_DECLARATION(ax::event::SimpleMsg<std::vector<ax::Window*>>, OnSelectMultipleWidget);

	private:
		MainWindow* _main_window;

		bool _has_tmp_widget;
		std::string _tmp_widget_builder_name;
	};
}
}

#endif /* atMainWindowWidgetHandler_hpp */
