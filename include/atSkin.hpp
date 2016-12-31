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

#ifndef atSkin_hpp
#define atSkin_hpp

#include <axlib/axlib.hpp>

namespace at {

struct SkinData {

	ax::Color common_at_yellow;
	// Status bar.
	ax::Color status_bar_bg;
	ax::Color status_bar_text;

	// Grid window.
	ax::Color grid_window_bg;
	ax::Color grid_window_contour;
	ax::Color grid_window_lines;

	// Widget menu.
	ax::Color w_menu_separator_bg;
	ax::Color w_menu_separator_contour;
	ax::Color w_menu_separator_text;

	ax::Color w_menu_top_bar;
	ax::Color w_menu_obj_bg_0;
	ax::Color w_menu_obj_bg_1;
	ax::Color w_menu_obj_contour;
	ax::Color w_menu_title_txt;
	ax::Color w_menu_txt;

	// Text editor.
	ax::Color txt_edit_bar;
	ax::Color txt_edit_bar_contour;
	ax::Color txt_edit_bar_label;

	// Preference panel.
	ax::Color preference_panel_bg;
};

class Skin {
public:
	inline static Skin* GetInstance()
	{
		if (_global_skin == nullptr) {
			_global_skin = new Skin();
		}

		return _global_skin;
	}

	void SetLightSkin();

	void SetDarkSkin();

	SkinData data;

private:
	static Skin* _global_skin;

	Skin()
	{
		data.common_at_yellow = ax::Color(250, 172, 0);
	}
};
}

#endif /* atSkin_hpp */
