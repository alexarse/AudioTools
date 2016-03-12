#ifndef mdiInspectorMenu_hpp
#define mdiInspectorMenu_hpp

#include "axLib.h"

namespace ax {
namespace editor {

	class MenuSeparator : public ax::Window::Backbone {
	public:
		MenuSeparator(const ax::Rect& rect, const std::string& name);
		
	private:
		std::string _name;
		ax::Font _font;
		
		void OnPaint(ax::GC gc);
	};
	
	class MenuAttribute : public ax::Window::Backbone {
	public:
		MenuAttribute(const ax::Rect& rect, const std::string& name, const std::string& value, ax::Event::Function fct);
		
	private:
		std::string _name;
		std::string _value;
		ax::Font _font;
		
		void OnPaint(ax::GC gc);
	};


	class InspectorMenu : public ax::Window::Backbone {
	public:
		InspectorMenu(const ax::Rect& rect);

		void SetWidgetHandle(ax::Window* handle);
		void RemoveHandle();

	private:
		ax::Window* _selected_handle;
		ax::Font _font;
		ax::StringVector _info_txt;
		ax::StringPairVector _info_pair_txt;
	
		axEVENT_DECLARATION(ax::Event::SimpleMsg<ax::StringPair>, OnPyoCallback);
		axEVENT_DECLARATION(ax::Event::SimpleMsg<ax::StringPair>, OnWidgetUpdate);
		axEVENT_DECLARATION(ax::Event::SimpleMsg<ax::StringPair>, OnInfoUpdate);
	
		void OnPaint(ax::GC gc);
	};
}
}

#endif /* mdiInspectorMenu_hpp */
