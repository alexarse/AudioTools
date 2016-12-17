//
//  atEditorStore.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-21.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef atEditorStore_hpp
#define atEditorStore_hpp

#include <axlib/axlib.hpp>

namespace at {
namespace editor {
	class OnlineStore {
	public:
		OnlineStore(ax::event::Object* obj);

		//		void DownloadWidgetData();

	private:
		ax::event::Object* _obj;
	};

	class OnlineStoreMenu : public ax::Window::Backbone {
	public:
		OnlineStoreMenu(const ax::Rect& rect);

	private:
		ax::Font _font;
		ax::Font _font_bold;
		std::shared_ptr<OnlineStore> _store;

		axEVENT_DECLARATION(ax::event::EmptyMsg, OnDoneDownloadingWidgetList);

		void OnPaint(ax::GC gc);
	};
}
}

#endif /* atEditorStore_hpp */
