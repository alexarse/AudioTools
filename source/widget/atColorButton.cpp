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

#include "widget/atColorButton.hpp"

namespace at {
ColorButton::ColorButton(const ax::Rect& rect, const ax::Button::Events& events, const ax::Button::Info& info,
	std::string img_path, std::string label, ax::util::Flag flags, std::string msg)
	: ax::Button(rect, events, info, img_path, label, flags, msg)
{

	win->event.OnPaint = ax::WBind<ax::GC>(this, &ColorButton::OnCustomPaint);
}

void ColorButton::OnCustomPaint(ax::GC gc)
{
	//	ax::console::Print("Draw ButtonColor");
	//	ax::Rect rect(win->dimension.GetRect());
	ax::Rect rect0(win->dimension.GetDrawingRect());

	ax::widget::Component::Ptr widget = win->component.Get<ax::widget::Component>("Widget");
	ax::Button::Info* info = static_cast<ax::Button::Info*>(widget->GetInfo());

	gc.SetColor(_currentColor);

	int radius = info->corner_radius;

	if (radius > 1) {
		gc.DrawRoundedRectangle(rect0, radius);
	}
	else {
		gc.DrawRectangle(rect0);
	}

	if (_btnImg->IsImageReady()) {

		if (_selected) {
			//		if (ax::IsFlag(Flags::SINGLE_IMG, _flags)) {
			gc.DrawImageResizeColor(_btnImg.get(), rect0.position, rect0.size - ax::Size(1, 1), _color);
			//		}
		}
		else {
			gc.DrawImageResize(_btnImg.get(), rect0.position, rect0.size - ax::Size(1, 1), 1.0);
		}
	}

	//	if (!_label.empty()) {
	//		gc.SetColor(info->font_color, 1.0);
	//		gc.DrawStringAlignedCenter(_font, _label, rect0);
	//	}

	//	gc.SetColor(info->contour);
	//
	//	if (radius > 1) {
	//		gc.DrawRoundedRectangleContour(rect0, radius);
	//	}
	//	else {
	gc.DrawRectangleContour(rect0);
	//	}
}
}
