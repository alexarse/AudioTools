//
//  atSaveWork.cpp
//  ProjectExample
//
//  Created by Alexandre Arsenault on 2016-04-21.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#include "dialog/atSaveWorkDialog.hpp"

namespace at {
SaveWorkPanel::Msg::Msg(const std::string& name, const std::string& description, const std::string& author)
	: _name(name)
	, _description(description)
	, _author(author)
{
}

ax::event::Msg* SaveWorkPanel::Msg::GetCopy()
{
	return new SaveWorkPanel::Msg(*this);
}

SaveWorkPanel::SaveWorkPanel(const ax::Rect& rect)
	: _font(0)
{
	win = ax::Window::Create(rect);
	win->event.OnPaint = ax::WBind<ax::GC>(this, &SaveWorkPanel::OnPaint);

	_missing_field[0] = false;
	_missing_field[1] = false;
	_missing_field[2] = false;

	ax::TextBox::Info txtInfo;
	txtInfo.normal = ax::Color(0.97);
	txtInfo.hover = txtInfo.normal;
	txtInfo.selected = txtInfo.normal;
	txtInfo.highlight = ax::Color(0.4f, 0.4f, 0.6f, 0.2f);
	txtInfo.contour = ax::Color(0.88);
	txtInfo.cursor = ax::Color(1.0f, 0.0f, 0.0f);
	txtInfo.selected_shadow = ax::Color(0.8f, 0.8f, 0.8f);
	txtInfo.font_color = ax::Color(0.0);

	ax::TextBox::Events txt_evts;

	const ax::Size txt_box_size(202, 25);
	ax::Point txt_box_pos(90, 31);
	auto name_box(ax::shared<ax::TextBox>(ax::Rect(txt_box_pos, txt_box_size), txt_evts, txtInfo));
	win->node.Add(name_box);

	_txt_boxes[0] = name_box.get();

	txt_box_pos = name_box->GetWindow()->dimension.GetRect().GetNextPosDown(9);

	auto description(ax::shared<ax::TextBox>(ax::Rect(txt_box_pos, txt_box_size), txt_evts, txtInfo));
	win->node.Add(description);

	_txt_boxes[1] = description.get();

	txt_box_pos = description->GetWindow()->dimension.GetRect().GetNextPosDown(9);

	auto author(ax::shared<ax::TextBox>(ax::Rect(txt_box_pos, txt_box_size), txt_evts, txtInfo));
	win->node.Add(author);

	_txt_boxes[2] = author.get();

	ax::Button::Info btn_info;
	btn_info.normal = ax::Color(0.97);
	btn_info.hover = ax::Color(0.99);
	btn_info.clicking = ax::Color(0.96);
	btn_info.selected = btn_info.normal;
	btn_info.contour = ax::Color(0.88);
	btn_info.font_color = ax::Color(0.0);
	btn_info.corner_radius = 0;

	ax::Point btn_pos(10, rect.size.h - 32);
	const ax::Size btn_size(88, 25);
	auto ok_btn = ax::shared<ax::Button>(ax::Rect(btn_pos, btn_size), GetOnSave(), btn_info, "", "Save");
	win->node.Add(ok_btn);

	btn_pos.x = rect.size.w - btn_size.w - 10;
	auto cancel_btn
		= ax::shared<ax::Button>(ax::Rect(btn_pos, btn_size), GetOnCancel(), btn_info, "", "Cancel");
	win->node.Add(cancel_btn);
}

void SaveWorkPanel::OnSave(const ax::Button::Msg& msg)
{
	std::string name = _txt_boxes[0]->GetLabel();
	std::string description = _txt_boxes[1]->GetLabel();
	std::string author = _txt_boxes[2]->GetLabel();

	_missing_field[0] = false;
	_missing_field[1] = false;
	_missing_field[2] = false;

	bool can_save = true;

	if (name.empty()) {
		_missing_field[0] = true;
		can_save = false;
	}

	if (description.empty()) {
		_missing_field[1] = true;
		can_save = false;
	}

	if (author.empty()) {
		_missing_field[2] = true;
		can_save = false;
	}

	if (!can_save) {
		win->Update();
		return;
	}

	ax::console::Print(name, description, author);
	win->PushEvent(SAVE, new Msg(name, description, author));
}

void SaveWorkPanel::OnCancel(const ax::Button::Msg& msg)
{
	win->PushEvent(CANCEL, new ax::event::EmptyMsg());
}

void SaveWorkPanel::OnPaint(ax::GC gc)
{
	const ax::Rect rect(win->dimension.GetDrawingRect());
	gc.SetColor(ax::Color(0.95));
	gc.DrawRectangle(rect);

	// Label.
	gc.SetColor(ax::Color(0.80));
	ax::Rect label_rect(0, 0, rect.size.w, 23);
	gc.DrawRectangle(label_rect);

	gc.SetColor(ax::Color(0.0));
	gc.DrawStringAlignedCenter(_font, "Save panel layout", label_rect);

	if (_missing_field[0]) {
		gc.SetColor(ax::Color(255, 0, 0));
	}
	else {
		gc.SetColor(ax::Color(0.0));
	}

	const ax::Point in_dev_pos(label_rect.position.x + 8, label_rect.position.y + 35);
	gc.DrawString(_font, "Name            : ", in_dev_pos);

	if (_missing_field[1]) {
		gc.SetColor(ax::Color(255, 0, 0));
	}
	else {
		gc.SetColor(ax::Color(0.0));
	}

	const ax::Point out_dev_pos(in_dev_pos + ax::Point(0, 34));
	gc.DrawString(_font, "Description   : ", out_dev_pos);

	if (_missing_field[2]) {
		gc.SetColor(ax::Color(255, 0, 0));
	}
	else {
		gc.SetColor(ax::Color(0.0));
	}

	const ax::Point author_pos(out_dev_pos + ax::Point(0, 34));
	gc.DrawString(_font, "Author            : ", author_pos);

	gc.SetColor(ax::Color(0.80));
	gc.DrawRectangleContour(rect);
}

SaveWorkDialog::SaveWorkDialog(const ax::Rect& rect)
{
	// Create window.
	win = ax::Window::Create(rect);
	win->event.OnPaint = ax::WBind<ax::GC>(this, &SaveWorkDialog::OnPaint);
	win->event.OnGlobalClick
		= ax::WBind<ax::Window::Event::GlobalClick>(this, &SaveWorkDialog::OnGlobalClick);
	win->event.OnMouseLeftDown = ax::WBind<ax::Point>(this, &SaveWorkDialog::OnMouseLeftDown);

	//	ax::App::GetInstance().GetPopupManager()->AddGlobalClickListener(win);
	win->event.GrabGlobalMouse();

	ax::Size pref_size(300, 170);
	ax::Point pos((rect.size.w - pref_size.w) / 2, (rect.size.h - pref_size.h) / 2);

	auto save_work = ax::shared<SaveWorkPanel>(ax::Rect(pos, pref_size));
	win->node.Add(save_work);
	_save_work_panel = save_work.get();

	_save_work_panel->GetWindow()->AddConnection(SaveWorkPanel::SAVE, GetOnAcceptSavePanelToWorkpace());
	_save_work_panel->GetWindow()->AddConnection(SaveWorkPanel::CANCEL, GetOnCancelSavePanelToWorkpace());
}

void SaveWorkDialog::OnGlobalClick(const ax::Window::Event::GlobalClick& gclick)
{
	if (_save_work_panel != nullptr) {
		if (!ax::App::GetInstance().GetPopupManager()->IsMouseStillInChildWindow(
				_save_work_panel->GetWindow())) {

			DeleteDialog();
		}
	}
}

void SaveWorkDialog::DeleteDialog()
{
	ax::App::GetInstance().GetWindowManager()->SetPastWindow(nullptr);
	ax::App::GetInstance().GetWindowManager()->UnGrabKey();
	ax::App::GetInstance().GetWindowManager()->UnGrabMouse();

	win->event.UnGrabKey();
	win->event.UnGrabMouse();
	win->backbone = nullptr;

	ax::App::GetInstance().GetPopupManager()->Clear();
	//	ax::App::GetInstance().GetPopupManager()->RemoveGlobalClickListener(win);
	//	ax::App::GetInstance().GetPopupManager()->GetWindowTree()->GetNodeVector().clear();
	//	ax::App::GetInstance().GetPopupManager()->UnGrabKey();
	//	ax::App::GetInstance().GetPopupManager()->UnGrabMouse();
	//	ax::App::GetInstance().GetPopupManager()->SetPastWindow(nullptr);
	ax::App::GetInstance().UpdateAll();
}

void SaveWorkDialog::OnAcceptSavePanelToWorkpace(const at::SaveWorkPanel::Msg& msg)
{
	win->PushEvent(at::SaveWorkPanel::SAVE, new at::SaveWorkPanel::Msg(msg));
	DeleteDialog();
}

void SaveWorkDialog::OnCancelSavePanelToWorkpace(const ax::event::EmptyMsg& msg)
{
	win->PushEvent(at::SaveWorkPanel::CANCEL, new ax::event::EmptyMsg());
	DeleteDialog();
}

void SaveWorkDialog::OnMouseLeftDown(const ax::Point& pos)
{
	//	win->PushEvent(CANCEL, new ax::event::StringMsg(""));
	DeleteDialog();
}

void SaveWorkDialog::OnPaint(ax::GC gc)
{
	const ax::Rect rect(win->dimension.GetDrawingRect());

	gc.SetColor(ax::Color(0.0, 0.6));
	gc.DrawRectangle(rect);
	gc.DrawRectangleContour(rect);
}
}
