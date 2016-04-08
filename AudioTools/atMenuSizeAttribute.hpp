#ifndef atMenuSizeAttribute_hpp
#define atMenuSizeAttribute_hpp

#include <OpenAX/OpenAX.h>
#include <OpenAX/NumberScroll.h>

namespace at {
namespace inspector {
	class SizeAttribute : public ax::Window::Backbone {
	public:
		enum Events : ax::Event::Id { ASSIGN_VALUE };

		SizeAttribute(
			const ax::Rect& rect, const std::string& name, const std::string& value, ax::Event::Function fct);

	private:
		std::string _name;
		ax::Font _font;
		ax::NumberScroll* _width_scroll;
		ax::NumberScroll* _height_scroll;

		axEVENT_DECLARATION(ax::NumberScroll::Msg, OnWidthChange);
		axEVENT_DECLARATION(ax::NumberScroll::Msg, OnHeightChange);

		void OnPaint(ax::GC gc);
	};
}
}

#endif /* atMenuSizeAttribute_hpp */
