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

#include "widget/DropMenuBox.hpp"

#include <axlib/WindowManager.hpp>

namespace ax {
DropMenuBox::DropMenuBox(
	const ax::Rect& rect, const std::string& current_value, const std::vector<std::string>& menu_options)
	: _is_droped(false)
	, _drop_menu(nullptr)
	, _menu_options(menu_options)
{
	// Create window.
	win = ax::Window::Create(rect);
	win->event.OnPaint = ax::WBind<ax::GC>(this, &DropMenuBox::OnPaint);

	ax::Button::Info btn_info;
	btn_info.normal = ax::Color(0.95);
	btn_info.hover = ax::Color(0.97);
	btn_info.clicking = ax::Color(0.93);
	btn_info.selected = btn_info.normal;
	btn_info.contour = ax::Color(0.80);
	btn_info.font_color = ax::Color(0.0);

	auto btn = ax::shared<ax::Button>(
		ax::Rect(0, 0, rect.size.w, rect.size.h), GetOnButtonClick(), btn_info, "", current_value);
	_drop_btn = btn.get();
	win->node.Add(btn);
}

int DropMenuBox::FindSelectedIndex()
{
	const std::vector<std::string>& items = _drop_menu->GetItems();

	for (int i = 0; i < items.size(); i++) {
		if (items[i] == _drop_btn->GetLabel()) {
			return i;
			break;
		}
	}

	return -1;
}

void DropMenuBox::OnButtonClick(const ax::Button::Msg& msg)
{
	if (_is_droped) {
		RemoveMenu();
		_is_droped = false;
	}
	else {
		ax::Button* sender = msg.GetSender();
		const ax::Point pos(sender->GetWindow()->dimension.GetAbsoluteRect().GetNextPosDown(0));

		CreateMenu(ax::Rect(pos, ax::Size(175, 300)), _menu_options);
		_is_droped = true;
	}
}

void DropMenuBox::CreateMenu(const ax::Rect& rect, const std::vector<std::string>& drop_options)
{
	ax::DropMenu::Info menu_info;
	menu_info.normal = ax::Color(240, 240, 240);
	menu_info.hover = ax::Color(246, 246, 246);
	menu_info.font_color = ax::Color(0.0);
	menu_info.selected = ax::Color(41, 222, 255);
	menu_info.selected_hover = ax::Color(41, 226, 255);
	menu_info.selected_font_color = ax::Color(0.0);
	menu_info.contour = ax::Color(0.86);
	menu_info.separation = ax::Color(0.86);
	menu_info.up_down_arrow = ax::Color(0.35);
	menu_info.right_arrow = ax::Color(0.70);
	menu_info.item_height = 25;

	auto menu = ax::shared<ax::DropMenu>(rect, GetOnMenuChoice(), menu_info, drop_options);

	_drop_menu = menu.get();

	// Find selected index.
	int selected_index = FindSelectedIndex();

	if (selected_index != -1) {
		_drop_menu->SetSelectedItem(selected_index);
	}

	ax::App::GetInstance().AddPopupTopLevel(menu);

	//	auto pop_man = ax::App::GetInstance().GetPopupManager();
	//	auto pop_win_tree = pop_man->GetWindowTree();
	//	std::vector<std::shared_ptr<ax::Window>>& pop_top_node_vector = pop_win_tree->GetNodeVector();

	//	if (pop_top_node_vector.size() == 0) {
	//		// Add to top level.
	//		menu->GetWindow()->backbone = menu;
	//		pop_top_node_vector.push_back(std::shared_ptr<ax::Window>(menu->GetWindow()));
	//	}
	//	else {
	//		// Add beside top level.
	//		menu->GetWindow()->backbone = menu;
	//		pop_top_node_vector.push_back(std::shared_ptr<ax::Window>(menu->GetWindow()));
	//	}
}

bool DropMenuBox::IsMouseInDropMenu()
{
	if (!_is_droped) {
		return false;
	}

	if (_drop_menu == nullptr) {
		return false;
	}

	auto pop_man = ax::App::GetInstance().GetPopupManager();
	return pop_man->IsMouseStillInChildWindow(_drop_menu->GetWindow());
}

void DropMenuBox::RemoveMenu()
{
	if (_drop_menu != nullptr) {
		auto pop_man = ax::App::GetInstance().GetPopupManager();
		auto pop_win_tree = pop_man->GetWindowTree();
		std::vector<std::shared_ptr<ax::Window>>& pop_top_node_vector = pop_win_tree->GetNodeVector();

		int index = -1;

		for (int i = 0; i < pop_top_node_vector.size(); i++) {
			if (pop_top_node_vector[i]->GetId() == _drop_menu->GetWindow()->GetId()) {
				index = i;
				break;
			}
		}

		if (index != -1) {
			std::shared_ptr<ax::Window> tmp_menu = pop_top_node_vector[index];
			pop_man->SetPastWindow(nullptr);
			pop_man->UnGrabKey();
			pop_man->UnGrabMouse();
			pop_top_node_vector.erase(pop_top_node_vector.begin() + index);
			tmp_menu->backbone.reset();
			_drop_menu = nullptr;
		}
	}
}

void DropMenuBox::OnMenuChoice(const ax::DropMenu::Msg& msg)
{
	ax::console::Print(msg.GetItem());
	_drop_btn->SetLabel(msg.GetItem());
	RemoveMenu();
	_is_droped = false;

	win->PushEvent(VALUE_CHANGE, new DropMenuBox::Msg(msg.GetItem()));
}

void DropMenuBox::OnPaint(ax::GC gc)
{
	const ax::Rect rect(win->dimension.GetDrawingRect());
	gc.SetColor(ax::Color(0.95));
	gc.DrawRectangle(rect);
}
}
