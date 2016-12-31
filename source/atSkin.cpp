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

#include "atSkin.hpp"

namespace at {

Skin* Skin::_global_skin = nullptr;

void Skin::SetLightSkin()
{
	data.status_bar_bg = ax::Color(0.30);
	data.status_bar_text = ax::Color(1.0);

	data.grid_window_bg = ax::Color(1.0);
	data.grid_window_contour = ax::Color(0.7);
	data.grid_window_lines = ax::Color(0.9);

	// Widget menu.
	data.w_menu_separator_bg = ax::Color(0.94);
	data.w_menu_separator_contour = ax::Color(0.94);
	data.w_menu_separator_text = ax::Color(0.3);

	data.w_menu_top_bar = ax::Color(0.45);
	data.w_menu_obj_bg_0 = ax::Color(1.0);
	data.w_menu_obj_bg_1 = ax::Color(0.98);
	data.w_menu_obj_contour = ax::Color(0.9);
	data.w_menu_title_txt = ax::Color(0.1);
	data.w_menu_txt = ax::Color(0.0);

	data.txt_edit_bar = ax::Color(0.45);
	data.txt_edit_bar_contour = ax::Color(0.45);
	data.txt_edit_bar_label = ax::Color(1.0);

	data.preference_panel_bg = ax::Color(0.94);
}

void Skin::SetDarkSkin()
{
	data.status_bar_bg = ax::Color(0.30);
	data.status_bar_text = ax::Color(1.0);

	data.grid_window_bg = ax::Color(0.4);
	data.grid_window_contour = ax::Color(0.3);
	data.grid_window_lines = ax::Color(0.45);

	// Widget menu.
	data.w_menu_separator_bg = ax::Color(0.24);
	data.w_menu_separator_contour = ax::Color(0.22);
	data.w_menu_separator_text = ax::Color(0.8);

	data.w_menu_top_bar = ax::Color(0.25);
	data.w_menu_obj_bg_0 = ax::Color(0.3);
	data.w_menu_obj_bg_1 = ax::Color(0.27);
	data.w_menu_obj_contour = ax::Color(0.2);
	data.w_menu_title_txt = ax::Color(0.7);
	data.w_menu_txt = ax::Color(0.9);

	data.txt_edit_bar = ax::Color(0.25);
	data.txt_edit_bar_contour = ax::Color(0.25);
	data.txt_edit_bar_label = ax::Color(0.8);

	data.preference_panel_bg = ax::Color(0.94);
}
}
// PyoAudio* PyoAudio::_global_audio = nullptr;
//
// PyoAudio* PyoAudio::GetInstance()
//{
//	if (_global_audio == nullptr) {
//		_global_audio = new PyoAudio();
//	}
//
//	return _global_audio;
//}
