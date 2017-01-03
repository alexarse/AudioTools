#pragma once

#include <axlib/ScrollBar.hpp>
#include <axlib/axlib.hpp>

namespace at {
namespace editor {
	class ProjectSpaceObj : public ax::Window::Backbone {
	public:
		ProjectSpaceObj(const ax::Rect& rect, const std::string& name, ax::Window* widget, int level);

		void UnSelectAll();
		void SetSelectedWidgets(const std::vector<ax::Window*>& widgets);

	private:
		ax::Font _font;
		ax::Font _font_bold;
		std::string _name;
		ax::Window* _widget;
		int _level;
		std::string _unique_name;
		bool _is_selected = false;
		std::shared_ptr<ax::Image> _icon;
		ax::Color _icon_color;

		void OnMouseLeftDoubleClick(const ax::Point& pos);
		void OnPaint(ax::GC gc);
	};

	class ProjectSpace : public ax::Window::Backbone {
	public:
		ProjectSpace(const ax::Rect& rect);

		void UpdateTree();
		void UnSelectAll();
		void SetSelectedWidgets(const std::vector<ax::Window*>& widgets);

		axEVENT_DECLARATION(ax::event::SimpleMsg<ax::Window*>, OnSelectWidget);
		axEVENT_DECLARATION(ax::event::SimpleMsg<int>, OnUnSelectAllWidget);
		axEVENT_DECLARATION(ax::event::SimpleMsg<std::vector<ax::Window*>>, OnSelectMultipleWidget);
		axEVENT_DECLARATION(ax::event::EmptyMsg, OnWidgetAddedOrRemoved);

	private:
		ax::Font _font;
		ax::Font _font_bold;
		bool _has_objects = false;

		ax::Window* _panel;
		ax::ScrollBar* _scroll_bar;
		bool _has_grid_window_connection = false;

		void OnResize(const ax::Size& size);
		void OnPaint(ax::GC gc);
	};
}
}
