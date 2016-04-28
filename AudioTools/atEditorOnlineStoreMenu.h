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
	class OnlineStore {
	public:
		OnlineStore(ax::Event::Object* obj);
		
//		void DownloadWidgetData();
		
		private:
		ax::Event::Object* _obj;
	};

	class OnlineStoreMenu : public ax::Window::Backbone {
	public:
		OnlineStoreMenu(const ax::Rect& rect);

	private:
		ax::Font _font;
		ax::Font _font_bold;
		std::shared_ptr<OnlineStore> _store;

		axEVENT_DECLARATION(ax::Event::EmptyMsg, OnDoneDownloadingWidgetList);

		void OnPaint(ax::GC gc);
	};
}
}

#endif /* atEditorStore_hpp */
