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

#ifndef atColorButton_hpp
#define atColorButton_hpp

#include <OpenAX/Button.h>

namespace at {
class ColorButton : public ax::Button {
public:
	ColorButton(const ax::Rect& rect, const ax::Button::Events& events, const ax::Button::Info& info,
		std::string img_path = "", std::string label = "", ax::Flag flags = 0, std::string msg = "");

	inline void SetSelected(bool sel)
	{
		if (_selected != sel) {
			_selected = sel;
			win->Update();
		}
	}

	inline void SetColor(const ax::Color& color)
	{
		_color = color;
		win->Update();
	}

private:
	bool _selected = false;
	ax::Color _color = ax::Color(250, 172, 0);

	void OnCustomPaint(ax::GC gc);
};
}

#endif /* atColorButton_hpp */
