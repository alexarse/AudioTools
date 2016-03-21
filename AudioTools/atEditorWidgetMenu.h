#ifndef mdiWidgetMenu_hpp
#define mdiWidgetMenu_hpp

#include <OpenAX/OpenAX.h>

#include <OpenAX/Button.h>
#include <OpenAX/ScrollBar.h>

namespace at {
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
			const std::string& title, const std::string& info, const std::string& size,
			const std::string& img_path);
		
		void HideText();
		
		void ShowText();
		
	private:
		ax::Font _font;
		ax::Font _font_normal;
		std::string _builder_name, _file_path, _title, _info, _size_str;
		std::shared_ptr<ax::Image> _img;
		bool _show_text = true;

		void OnMouseLeftDown(const ax::Point& pos);
		void OnMouseLeftDragging(const ax::Point& pos);
		void OnMouseLeftUp(const ax::Point& pos);
		void OnPaint(ax::GC gc);
	};

	class WidgetMenu : public ax::Window::Backbone {
	public:
		WidgetMenu(const ax::Rect& rect);
		
		enum : ax::Event::Id { SMALLER_MENU };

	private:
//		std::shared_ptr<WidgetMenuObj> _objs; // Fuckup for now.
		ax::Window* _panel;
		ax::ScrollBar::Ptr _scrollBar;
		std::vector<std::shared_ptr<WidgetMenuObj>> _objs;
		bool _dropped_smaller = false;
	
		axEVENT_DECLARATION(ax::Button::Msg, OnSmallerMenu);
		

//
		void SetSmall();
		void OnMouseEnter(const ax::Point& pos);
		void OnMouseLeave(const ax::Point& pos);
		void OnMouseEnterChild(const ax::Point& pos);
		void OnMouseLeaveChild(const ax::Point& pos);
		void OnScrollWheel(const ax::Point& delta);
		void OnResize(const ax::Size& size);
		void OnPaint(ax::GC gc);
	};
}
}

#endif /* mdiWidgetMenu_hpp */
