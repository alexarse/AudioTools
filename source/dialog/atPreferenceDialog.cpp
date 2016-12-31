/*
 * Copyright (c) 2016 AudioTools - All Rights Reserved
 *
 * This Software may not be distributed in parts or its entirety
 * without prior written agreement by AudioTools.
 *
 * Neither the name of the AudioTools nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY AUDIOTOOLS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL AUDIOTOOLS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Written by Alexandre Arsenault <alx.arsenault@gmail.com>
 */

#include "dialog/atPreferenceDialog.hpp"

#include <axlib/Core.hpp>
#include <axlib/FileSystem.hpp>
#include <axlib/Toggle.hpp>

#include "PyoAudio.h"
#include "atMidi.h"
#include "atSkin.hpp"

namespace at {
namespace editor {
	PreferencePanel::PreferencePanel(const ax::Rect& rect)
		: _font(0)
		, _audio_rect(10, 10, rect.size.w - 20, 100)
	{
		_audio_label_rect = ax::Rect(_audio_rect.position, ax::Size(_audio_rect.size.w, 23));

		_midi_rect = ax::Rect(_audio_rect.GetNextPosDown(10), ax::Size(_audio_rect.size.w, 65));
		_midi_label_rect = ax::Rect(_midi_rect.position, ax::Size(_midi_rect.size.w, 23));

		// Create window.
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &PreferencePanel::OnPaint);

		PyoAudio* audio = PyoAudio::GetInstance();

		// Audio input device.
		auto btn_in = ax::shared<ax::DropMenuBox>(
			ax::Rect(ax::Point(_audio_label_rect.position.x + 95, _audio_label_rect.position.y + 30),
				ax::Size(175, 25)),
			audio->GetCurrentInputDevice(), audio->GetInputDevices());

		win->node.Add(btn_in);
		btn_in->GetWindow()->AddConnection(ax::DropMenuBox::VALUE_CHANGE, GetOnAudioInputDevice());
		_menu_boxes[AUDIO_IN] = btn_in.get();

		// Audio output device.
		ax::Point o_pos(btn_in->GetWindow()->dimension.GetRect().GetNextPosDown(10));
		auto btn_out = ax::shared<ax::DropMenuBox>(
			ax::Rect(o_pos, ax::Size(175, 25)), audio->GetCurrentOutputDevice(), audio->GetOutputDevices());

		btn_out->GetWindow()->AddConnection(ax::DropMenuBox::VALUE_CHANGE, GetOnAudioOutputDevice());
		win->node.Add(btn_out);

		_menu_boxes[AUDIO_OUT] = btn_out.get();

		at::Midi* midi = at::Midi::GetInstance();
		std::vector<std::string> midi_in_opts = midi->GetMidiInputList();

		// Midi input device.
		auto btn_midi_in = ax::shared<ax::DropMenuBox>(
			ax::Rect(ax::Point(_midi_label_rect.position.x + 95, _midi_label_rect.position.y + 30),
				ax::Size(175, 25)),
			midi->GetCurrentPortName(), midi_in_opts);

		btn_midi_in->GetWindow()->AddConnection(ax::DropMenuBox::VALUE_CHANGE, GetOnMidiInputDevice());
		win->node.Add(btn_midi_in);
		_menu_boxes[MIDI_IN] = btn_midi_in.get();
	}

	PreferencePanel::~PreferencePanel()
	{
	}

	void PreferencePanel::OnAudioInputDevice(const ax::DropMenuBox::Msg& msg)
	{
		PyoAudio* audio = PyoAudio::GetInstance();
		audio->SetCurrentInputDevice(msg.GetMsg());
	}

	void PreferencePanel::OnAudioOutputDevice(const ax::DropMenuBox::Msg& msg)
	{
		PyoAudio* audio = PyoAudio::GetInstance();
		audio->SetCurrentOutputDevice(msg.GetMsg());
	}

	void PreferencePanel::OnMidiInputDevice(const ax::DropMenuBox::Msg& msg)
	{
		at::Midi* midi = at::Midi::GetInstance();
		midi->SetInputPort(msg.GetMsg());
	}

	bool PreferencePanel::IsMouseInDropMenu()
	{
		for (int i = 0; i < NUMBER_OF_PREF_BOX; i++) {
			if (_menu_boxes[i]->IsMouseInDropMenu()) {
				return true;
			}
		}
		return false;
	}

	void PreferencePanel::OnPaint(ax::GC gc)
	{
		const ax::Rect rect(win->dimension.GetDrawingRect());
		gc.SetColor(ax::Color(0.95));
		gc.DrawRectangle(rect);

		// Audio rectangle.
		gc.SetColor(ax::Color(0.80));
		gc.DrawRectangleContour(_audio_rect);

		// Audio label.
		gc.SetColor(ax::Color(0.80));
		gc.DrawRectangle(_audio_label_rect);

		gc.SetColor(ax::Color(0.0));
		gc.DrawStringAlignedCenter(_font, "Audio", _audio_label_rect);

		// Audio input device.
		gc.SetColor(ax::Color(0.0));
		const ax::Point in_dev_pos(_audio_label_rect.position.x + 8, _audio_label_rect.position.y + 35);
		gc.DrawString(_font, "Input device    : ", in_dev_pos);

		// Audio output device.
		const ax::Point out_dev_pos(in_dev_pos + ax::Point(0, 34));
		gc.DrawString(_font, "Output device : ", out_dev_pos);

		// Midi rectangle.
		gc.SetColor(ax::Color(0.80));
		gc.DrawRectangleContour(_midi_rect);

		// Midi label.
		gc.SetColor(ax::Color(0.80));
		gc.DrawRectangle(_midi_label_rect);

		gc.SetColor(ax::Color(0.0));
		gc.DrawStringAlignedCenter(_font, "Midi", _midi_label_rect);

		// Audio input device.
		gc.SetColor(ax::Color(0.0));
		const ax::Point midi_in_dev_pos(_midi_label_rect.position.x + 8, _midi_label_rect.position.y + 35);
		gc.DrawString(_font, "Input device    : ", midi_in_dev_pos);

		// Preference contour.
		gc.SetColor(ax::Color(0.80));
		gc.DrawRectangleContour(rect);
	}

	PreferenceDialog::PreferenceDialog(const ax::Rect& rect)
	{
		// Create window.
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &PreferenceDialog::OnPaint);

		win->event.OnGlobalClick
			= ax::WBind<ax::Window::Event::GlobalClick>(this, &PreferenceDialog::OnGlobalClick);

		win->event.OnMouseLeftDown = ax::WBind<ax::Point>(this, &PreferenceDialog::OnMouseLeftDown);
		win->event.OnAssignToWindowManager = ax::WBind<int>(this, &PreferenceDialog::OnAssignToWindowManager);

		ax::Size pref_size(300, 194);
		ax::Point pos((rect.size.w - pref_size.w) / 2, (rect.size.h - pref_size.h) / 2);

		auto pref_panel = ax::shared<PreferencePanel>(ax::Rect(pos, pref_size));
		win->node.Add(pref_panel);
		_pref_panel = pref_panel.get();
	}

	void PreferenceDialog::OnAssignToWindowManager(const int& v)
	{
		win->event.GrabGlobalMouse();
	}

	void PreferenceDialog::OnGlobalClick(const ax::Window::Event::GlobalClick& gclick)
	{
		ax::console::Print("Global click");

		if (_pref_panel != nullptr) {
			if (!ax::App::GetInstance().GetPopupManager()->IsMouseStillInChildWindow(
					_pref_panel->GetWindow())) {
				if (!_pref_panel->IsMouseInDropMenu() && gclick.type != gclick.LEFT_CLICK_UP) {
					DeleteDialog();
				}
			}
		}
	}

	void PreferenceDialog::DeleteDialog()
	{
		ax::App::GetInstance().GetPopupManager()->Clear();
	}

	void PreferenceDialog::OnMouseLeftDown(const ax::Point& pos)
	{
		DeleteDialog();
	}

	void PreferenceDialog::OnPaint(ax::GC gc)
	{
		const ax::Rect rect(win->dimension.GetDrawingRect());

		gc.SetColor(ax::Color(0.0, 0.6));
		gc.DrawRectangle(rect);
		gc.DrawRectangleContour(rect);
	}
}
}
