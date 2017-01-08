//
//  atEditorWorkspaceObj.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-21.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef atEditorWorkspaceObj_hpp
#define atEditorWorkspaceObj_hpp

#include <axlib/axlib.hpp>

namespace at {
namespace editor {

	class WorkspaceObj : public ax::Window::Backbone {
	public:
		WorkspaceObj(const ax::Rect& rect, const std::string& builder_name, const std::string& file_path,
			const std::string& title, const std::string& info, const std::string& size,
			const std::string& img_path);

		void HideText();

		void ShowText();

		void SetSelectable(bool selectable);

	private:
		ax::Font _font;
		ax::Font _font_normal;
		std::string _builder_name, _file_path, _title, _info, _size_str;
		std::shared_ptr<ax::Image> _img;
		ax::Size _img_size;
		bool _show_text = true;
		bool _selectable;

		void OnMouseLeftDown(const ax::Point& pos);
		void OnMouseLeftDragging(const ax::Point& pos);
		void OnMouseLeftUp(const ax::Point& pos);
		void OnPaint(ax::GC gc);
	};
}
}

#endif /* atEditorWorkspaceObj_hpp */
