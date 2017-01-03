#pragma once

#include <axlib/ScrollBar.hpp>
#include <axlib/axlib.hpp>

namespace at {
namespace editor {
	class ProjectSpaceObj : public ax::Window::Backbone {
	public:
		ProjectSpaceObj(const ax::Rect& rect, const std::string& name, ax::Window* widget, int level);

	private:
		ax::Font _font;
		ax::Font _font_bold;
		std::string _name;
		ax::Window* _widget;
		int _level;
		std::string _unique_name;

		void OnMouseLeftDoubleClick(const ax::Point& pos);
		void OnPaint(ax::GC gc);
	};

	class ProjectSpace : public ax::Window::Backbone {
	public:
		ProjectSpace(const ax::Rect& rect);

		void UpdateTree();

	private:
		ax::Font _font;
		ax::Font _font_bold;

		ax::Window* _panel;
		ax::ScrollBar* _scroll_bar;

		void OnResize(const ax::Size& size);
		void OnMouseLeftDown(const ax::Point& pos);
		void OnPaint(ax::GC gc);
	};
}
}
