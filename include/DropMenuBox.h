/*
 * Copyright (c) 2016 Alexandre Arsenault.
 *
 * This file is part of OpenAX.
 *
 * OpenAX is free or commercial software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 or any later version of the
 * license or use a commercial OpenAX License.
 *
 * OpenAX is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenAX. If not, see <http://www.gnu.org/licenses/>.
 *
 * To release a closed-source product which uses OpenAX, commercial
 * licenses are available, email ax.frameworks@gmail.com for more information.
 */

#ifndef DropMenuBox_hpp
#define DropMenuBox_hpp

#include <axlib/axlib.hpp>

#include <axlib/Button.hpp>
#include <axlib/DropMenu.hpp>

namespace ax {
class DropMenuBox : public ax::Window::Backbone {
public:
	DropMenuBox(
		const ax::Rect& rect, const std::string& current_value, const std::vector<std::string>& menu_options);

	enum : ax::event::Id { VALUE_CHANGE };

	typedef ax::event::SimpleMsg<std::string> Msg;

	bool IsDropped() const
	{
		return _is_droped;
	}

	bool IsMouseInDropMenu();

private:
	bool _is_droped;
	ax::Button* _drop_btn;
	ax::DropMenu* _drop_menu;
	std::vector<std::string> _menu_options;

	int FindSelectedIndex();

	axEVENT_ACCESSOR(ax::Button::Msg, OnButtonClick);
	void OnButtonClick(const ax::Button::Msg& msg);

	void CreateMenu(const ax::Rect& rect, const std::vector<std::string>& drop_options);

	void RemoveMenu();

	axEVENT_ACCESSOR(ax::DropMenu::Msg, OnMenuChoice);
	void OnMenuChoice(const ax::DropMenu::Msg& msg);

	void OnPaint(ax::GC gc);
};
}

#endif /* DropMenuBox_hpp */
