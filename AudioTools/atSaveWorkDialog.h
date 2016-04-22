//
//  atSaveWork.hpp
//  ProjectExample
//
//  Created by Alexandre Arsenault on 2016-04-21.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef atSaveWork_hpp
#define atSaveWork_hpp

#include <OpenAX/OpenAX.h>

#include <OpenAX/Button.h>
#include <OpenAX/Slider.h>
#include <OpenAX/TextBox.h>

namespace at {
class SaveWorkPanel : public ax::Window::Backbone {
public:
	enum Events : ax::Event::Id { SAVE, CANCEL };

	class Msg : public ax::Event::Msg {
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

		ax::Event::Msg* GetCopy();

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

	void OnGlobalClick(const ax::Window::Event::GlobalClick& gclick);
	void OnMouseLeftDown(const ax::Point& pos);
	void OnPaint(ax::GC gc);
};
}

#endif /* atSaveWork_hpp */
