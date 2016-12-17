//
//  atSaveWork.hpp
//  ProjectExample
//
//  Created by Alexandre Arsenault on 2016-04-21.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef atSaveWork_hpp
#define atSaveWork_hpp

#include <axlib/axlib.hpp>

#include <axlib/Button.hpp>
#include <axlib/Slider.hpp>
#include <axlib/TextBox.hpp>

namespace at {
class SaveWorkPanel : public ax::Window::Backbone {
public:
	enum Events : ax::event::Id { SAVE, CANCEL };

	class Msg : public ax::event::Msg {
	public:
		Msg(const std::string& name, const std::string& description, const std::string& author);

		inline std::string GetName() const
		{
			return _name;
		}

		inline std::string GetDescription() const
		{
			return _description;
		}

		inline std::string GetAuthor() const
		{
			return _author;
		}

		ax::event::Msg* GetCopy();

	private:
		std::string _name, _description, _author;
	};

	SaveWorkPanel(const ax::Rect& rect);

private:
	ax::Font _font;
	ax::TextBox* _txt_boxes[3];

	bool _missing_field[3];

	axEVENT_DECLARATION(ax::Button::Msg, OnSave);
	axEVENT_DECLARATION(ax::Button::Msg, OnCancel);

	void OnPaint(ax::GC gc);
};

class SaveWorkDialog : public ax::Window::Backbone {
public:
	SaveWorkDialog(const ax::Rect& rect);

	virtual ~SaveWorkDialog()
	{
	}

private:
	SaveWorkPanel* _save_work_panel;

	void DeleteDialog();

	axEVENT_DECLARATION(at::SaveWorkPanel::Msg, OnAcceptSavePanelToWorkpace);
	axEVENT_DECLARATION(ax::event::EmptyMsg, OnCancelSavePanelToWorkpace);

	void OnGlobalClick(const ax::Window::Event::GlobalClick& gclick);
	void OnMouseLeftDown(const ax::Point& pos);
	void OnPaint(ax::GC gc);
};
}

#endif /* atSaveWork_hpp */
