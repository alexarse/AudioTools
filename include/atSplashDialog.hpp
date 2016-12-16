/*
 * Copyright (c) 2016 AudioTools - All Rights Reserved
 *
 * This Software may not be distributed in parts or its entirety
 * without prior written agreement by AutioTools.
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

#ifndef __AT_SPLASH_DIALOG_H__
#define __AT_SPLASH_DIALOG_H__

#include <OpenAX/Button.h>
#include <OpenAX/OpenAX.h>

namespace at {
class SplashDialog : public ax::Window::Backbone {
public:
	SplashDialog(const ax::Rect& rect);

	typedef std::pair<double, std::string> LoadInfoMsg;

	axEVENT_DECLARATION(ax::Event::SimpleMsg<LoadInfoMsg>, OnLoadingPercent);

private:
	ax::Font _font;
	ax::Font _font_app_name;
	ax::Image _logo_img;
	double _load_percent;
	std::string _load_info;

	void OnPaint(ax::GC gc);
};
}

#endif // __AT_SPLASH_DIALOG_H__
