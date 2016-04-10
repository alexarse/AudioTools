#include "atCommon.h"
#include "atEditorStatusBar.h"
#include "atOpenDialog.hpp"
#include "atSaveDialog.hpp"
#include "PyoAudio.h"
#include "atSkin.hpp"

#include <OpenAX/Core.h>
#include <OpenAX/Toggle.h>
#include <OpenAX/WindowManager.h>

namespace at {
namespace editor {
	StatusBar::StatusBar(const ax::Rect& rect)
		: _font(0)
	{
		// Create window.
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &StatusBar::OnPaint);
		win->event.OnResize = ax::WBind<ax::Size>(this, &StatusBar::OnResize);

		// Transparent toggle with image.
		ax::Toggle::Info tog_info;
		tog_info.normal = ax::Color(0.0, 0.0);
		tog_info.hover = ax::Color(0.0, 0.0);
		tog_info.clicking = ax::Color(0.0, 0.0);
		tog_info.selected = ax::Color(0.0, 0.0);
		tog_info.font_color = ax::Color(0.0, 0.0);

		tog_info.selected = ax::Color(0.0, 0.0);
		tog_info.selected_hover = ax::Color(0.0, 0.0);
		tog_info.selected_clicking = ax::Color(0.0, 0.0);
		tog_info.selected = ax::Color(0.0, 0.0);
		tog_info.selected_font_color = ax::Color(0.0, 0.0);

		tog_info.contour = ax::Color(0.0, 0.0);
		tog_info.font_size = 12;
		tog_info.img = "resources/top_menu_toggle_left.png";
		tog_info.single_img = false;

		ax::Point pos(rect.size.x - 95, 2);

		// Volume meter left.
		ax::Rect volume_rect(rect.size.x - 165, 8, 50, 7);
		auto v_meter_l = ax::shared<at::VolumeMeter>(volume_rect);
		win->node.Add(v_meter_l);
		_volumeMeterLeft = v_meter_l.get();

		// Volume meter right.
		volume_rect.position = volume_rect.GetNextPosDown(0);
		auto v_meter_r = ax::shared<at::VolumeMeter>(volume_rect);
		win->node.Add(v_meter_r);
		_volumeMeterRight = v_meter_r.get();
		
		// Connect volume meter event.
		win->AddConnection(PyoAudio::Events::RMS_VALUE_CHANGE, GetOnAudioRmsValue());
		PyoAudio::GetInstance()->SetConnectedObject(win);
		
		const ax::Size tog_size(25, 25);

		// Left panel toggle.
		auto tog_left = ax::shared<ax::Toggle>(ax::Rect(pos, tog_size), GetOnToggleLeftPanel(), tog_info);

		// Bottom panel toggle.
		pos = tog_left->GetWindow()->dimension.GetRect().GetNextPosRight(5);
		tog_info.img = "resources/top_menu_toggle_bottom.png";
		auto tog_middle = ax::shared<ax::Toggle>(ax::Rect(pos, tog_size), GetOnToggleBottomPanel(), tog_info);

		// Right panel toggle.
		pos = tog_middle->GetWindow()->dimension.GetRect().GetNextPosRight(5);
		tog_info.img = "resources/top_menu_toggle_right.png";
		auto tog_right = ax::shared<ax::Toggle>(ax::Rect(pos, tog_size), GetOnToggleRightPanel(), tog_info);

		tog_left->SetSelected(true);
		tog_middle->SetSelected(true);
		tog_right->SetSelected(true);

		win->node.Add(tog_left);
		win->node.Add(tog_middle);
		win->node.Add(tog_right);

		_toggle_left = tog_left.get();
		_toggle_bottom = tog_middle.get();
		_toggle_right = tog_right.get();

		ax::Button::Info btn_info;
		btn_info.normal = ax::Color(0.30);
		btn_info.hover = ax::Color(0.34);
		btn_info.clicking = ax::Color(0.32);
		btn_info.selected = ax::Color(0.30);
		btn_info.contour = ax::Color(0.30);
		btn_info.font_color = ax::Color(1.0);

		// Open button.
		pos = ax::Point(5, 2);
		auto open_menu = ax::shared<ax::Button>(ax::Rect(pos, ax::Size(25, 25)), GetOnOpenLayout(), btn_info,
			"resources/folder.png", "", ax::Button::Flags::SINGLE_IMG);
		win->node.Add(open_menu);

		// Save button.
		pos = open_menu->GetWindow()->dimension.GetRect().GetNextPosRight(5);
		auto save_btn = ax::shared<ax::Button>(ax::Rect(pos, ax::Size(25, 25)), GetOnSaveLayout(), btn_info,
			"resources/save.png", "", ax::Button::Flags::SINGLE_IMG);
		win->node.Add(save_btn);

		// Save as button.
		pos = save_btn->GetWindow()->dimension.GetRect().GetNextPosRight(5);
		auto save_as_btn = ax::shared<ax::Button>(ax::Rect(pos, ax::Size(25, 25)), GetOnSaveAsLayout(),
			btn_info, "resources/save_as.png", "", ax::Button::Flags::SINGLE_IMG);
		win->node.Add(save_as_btn);

		// View button.
		pos = save_as_btn->GetWindow()->dimension.GetRect().GetNextPosRight(5);
		auto view_btn = ax::shared<ax::Button>(ax::Rect(pos, ax::Size(25, 25)), GetOnViewLayout(), btn_info,
			"resources/view.png", "", ax::Button::Flags::SINGLE_IMG);
		win->node.Add(view_btn);

		// Refresh button.
		pos = view_btn->GetWindow()->dimension.GetRect().GetNextPosRight(5);
		auto refresh_btn = ax::shared<ax::Button>(ax::Rect(pos, ax::Size(25, 25)), GetOnReload(), btn_info,
			"resources/refresh.png", "", ax::Button::Flags::SINGLE_IMG);
		win->node.Add(refresh_btn);
		
		// Settings button.
		pos = refresh_btn->GetWindow()->dimension.GetRect().GetNextPosRight(5);
		auto settings_btn = ax::shared<ax::Button>(ax::Rect(pos, ax::Size(25, 25)), GetOnSettings(), btn_info,
												  "resources/settings.png", "", ax::Button::Flags::SINGLE_IMG);
		win->node.Add(settings_btn);
	}

	void StatusBar::OnSaveLayout(const ax::Button::Msg& msg)
	{
		win->PushEvent(SAVE_LAYOUT, new ax::Event::StringMsg(_layout_file_path));
	}

	void StatusBar::OnSaveAsLayout(const ax::Button::Msg& msg)
	{
		ax::Size fsize = ax::App::GetInstance().GetFrameSize();
		ax::Size size(300, 200);
		ax::Point pos((fsize.x - size.x) / 2, (fsize.y - size.y) / 2);

		auto save_dialog = ax::shared<SaveDialog>(ax::Rect(pos, size), _layout_file_path);

		ax::App::GetInstance().GetPopupManager()->GetWindowTree()->AddTopLevel(
			ax::Window::Ptr(save_dialog->GetWindow()));

		save_dialog->GetWindow()->backbone = save_dialog;

		save_dialog->GetWindow()->AddConnection(SaveDialog::SAVE, GetOnSaveDialog());
	}

	void StatusBar::OnOpenLayout(const ax::Button::Msg& msg)
	{
		const ax::Rect rect = msg.GetSender()->GetWindow()->dimension.GetAbsoluteRect();
		ax::Point pos = rect.position;
		pos.y += rect.size.y;

		ax::Size size = ax::App::GetInstance().GetFrameSize();

		auto open_dialog = ax::shared<OpenDialog>(ax::Rect(pos, size));
		
		ax::App::GetInstance().GetPopupManager()->GetWindowTree()->AddTopLevel(
			ax::Window::Ptr(open_dialog->GetWindow()));
		
		open_dialog->GetWindow()->backbone = open_dialog;
		open_dialog->GetWindow()->AddConnection(OpenDialog::OPEN, GetOnOpenDialog());
	}

	void StatusBar::OnViewLayout(const ax::Button::Msg& msg)
	{
		ax::Print("On view layout.");
		win->PushEvent(VIEW_LAYOUT, new ax::Event::SimpleMsg<int>(0));
	}

	void StatusBar::OnReload(const ax::Button::Msg& msg)
	{
		ax::Print("On reload script.");
		win->PushEvent(RELOAD_SCRIPT, new ax::Event::SimpleMsg<int>(0));
	}
	
	void StatusBar::OnSettings(const ax::Button::Msg& msg)
	{
		ax::Print("Setting");
	}

	void StatusBar::OnToggleLeftPanel(const ax::Toggle::Msg& msg)
	{
		win->PushEvent(TOGGLE_LEFT_PANEL, new ax::Toggle::Msg(msg));
	}

	void StatusBar::OnToggleBottomPanel(const ax::Toggle::Msg& msg)
	{
		win->PushEvent(TOGGLE_BOTTOM_PANEL, new ax::Toggle::Msg(msg));
	}

	void StatusBar::OnToggleRightPanel(const ax::Toggle::Msg& msg)
	{
		win->PushEvent(TOGGLE_RIGHT_PANEL, new ax::Toggle::Msg(msg));
	}

	void StatusBar::OnSaveDialog(const ax::Event::StringMsg& msg)
	{
		win->PushEvent(SAVE_LAYOUT, new ax::Event::StringMsg(msg));
	}

	void StatusBar::OnOpenDialog(const ax::Event::StringMsg& msg)
	{
		win->PushEvent(OPEN_LAYOUT, new ax::Event::StringMsg(msg));
	}

	void StatusBar::OnCancelDialog(const ax::Event::StringMsg& msg)
	{
	}
	
	void StatusBar::OnAudioRmsValue(const ax::Event::SimpleMsg<StereoRmsValue>& msg)
	{
		_volumeMeterLeft->SetValue(msg.GetMsg().first);
		_volumeMeterRight->SetValue(msg.GetMsg().second);
	}

	void StatusBar::OnResize(const ax::Size& size)
	{
		// Repos left volume meter.
		_volumeMeterLeft->GetWindow()->dimension.SetPosition(ax::Point(size.x - 165, 8));

		// Repos right volume meter.
		_volumeMeterRight->GetWindow()->dimension.SetPosition(
			_volumeMeterLeft->GetWindow()->dimension.GetRect().GetNextPosDown(0));

		// Left toggle.
		ax::Point pos(size.x - 95, 2);
		_toggle_left->GetWindow()->dimension.SetPosition(pos);
		pos = _toggle_left->GetWindow()->dimension.GetRect().GetNextPosRight(5);

		// Middle toggle.
		_toggle_bottom->GetWindow()->dimension.SetPosition(pos);
		pos = _toggle_bottom->GetWindow()->dimension.GetRect().GetNextPosRight(5);

		// Right toggle.
		_toggle_right->GetWindow()->dimension.SetPosition(pos);
	}

	void StatusBar::OnPaint(ax::GC gc)
	{
		ax::Rect rect(win->dimension.GetDrawingRect());

//		gc.SetColor(ax::Color(0.30));
		gc.SetColor(at::Skin::GetInstance()->data.status_bar_bg);
		gc.DrawRectangle(rect);

		if (!_layout_file_path.empty()) {
//			gc.SetColor(ax::Color(1.0));
			gc.SetColor(at::Skin::GetInstance()->data.status_bar_text);
			gc.DrawStringAlignedCenter(_font, _layout_file_path, rect);
		}

//		gc.SetColor(ax::Color(0.30));
		gc.SetColor(at::Skin::GetInstance()->data.status_bar_bg);
		gc.DrawRectangleContour(rect);
	}
}
}