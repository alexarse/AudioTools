#ifndef mdiWidgetMenu_hpp
#define mdiWidgetMenu_hpp

#include "axLib.h"
#include "axScrollBar.h"

namespace ax {
	namespace editor {
	class WidgetMenuSeparator : public ax::Window::Backbone {
	public:
		WidgetMenuSeparator(const ax::Rect& rect, const std::string& name);
		
	private:
		ax::Font _font;
		std::string _name;
		
		void OnMouseLeftDown(const ax::Point& pos);
		void OnMouseLeftDragging(const ax::Point& pos);
		void OnMouseLeftUp(const ax::Point& pos);
		void OnPaint(ax::GC gc);
	};
	
	class WidgetMenuObj : public ax::Window::Backbone {
	public:
		WidgetMenuObj(const ax::Rect& rect, const std::string& builder_name, const std::string& file_path,
					  const std::string& title, const std::string& info,
					  const std::string& size, const std::string& img_path);
		
	private:
		ax::Font _font;
		ax::Font _font_normal;
		std::string _builder_name, _file_path, _title, _info, _size_str;
		std::shared_ptr<ax::Image> _img;
		
		void OnMouseLeftDown(const ax::Point& pos);
		void OnMouseLeftDragging(const ax::Point& pos);
		void OnMouseLeftUp(const ax::Point& pos);
		void OnPaint(ax::GC gc);
	};
	
	class WidgetMenu : public ax::Window::Backbone {
	public:
		WidgetMenu(const ax::Rect& rect);
	private:
		std::shared_ptr<WidgetMenuObj> _objs; // Fuckup for now.
		ax::Window* _panel;
		ax::ScrollBar::Ptr _scrollBar;
		
		void OnResize(const ax::Size& size);
		void OnPaint(ax::GC gc);
	};
}
}

#endif /* mdiWidgetMenu_hpp */
