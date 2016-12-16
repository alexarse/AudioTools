#ifndef atEditorProjectSpace_hpp
#define atEditorProjectSpace_hpp

#include <OpenAX/OpenAX.h>

namespace at {
namespace editor {
	class ProjectSpace : public ax::Window::Backbone {
	public:
		ProjectSpace(const ax::Rect& rect);

	private:
		ax::Font _font;
		ax::Font _font_bold;
		void OnPaint(ax::GC gc);
	};
}
}

#endif /* atEditorProjectSpace_hpp */
