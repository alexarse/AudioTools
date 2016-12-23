//
//  atEditorWidgetMenuObj.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-21.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef atEditorWidgetMenuObj_hpp
#define atEditorWidgetMenuObj_hpp

#include <axlib/axlib.hpp>

namespace at {
namespace editor {

	struct WidgetMenuInfo {
		std::string file_path;
		std::string buider_name;
		std::string widget_label;
		std::string widget_desc;
		std::string widget_size;
		std::string widget_img;
	};

	class WidgetMenuObj : public ax::Window::Backbone {
	public:
		WidgetMenuObj(const ax::Rect& rect, const WidgetMenuInfo& info);

		void HideText();

		void ShowText();

		void SetSelectable(bool selectable);

	private:
		ax::Font _font;
		ax::Font _font_normal;
		WidgetMenuInfo _info;
		//		std::string _builder_name, _file_path,_title, _info, _size_str;
		std::shared_ptr<ax::Image> _img;
		bool _show_text = true;
		bool _selectable;

		void OnMouseLeftDown(const ax::Point& pos);
		void OnMouseLeftDragging(const ax::Point& pos);
		void OnMouseLeftUp(const ax::Point& pos);
		void OnPaint(ax::GC gc);
	};
}
}

#endif /* atEditorWidgetMenuObj_hpp */
