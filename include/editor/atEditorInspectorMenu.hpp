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

#ifndef mdiInspectorMenu_hpp
#define mdiInspectorMenu_hpp

#include <axlib/axlib.hpp>

namespace at {
namespace editor {

	class MenuSeparator : public ax::Window::Backbone {
	public:
		MenuSeparator(const ax::Rect& rect, const std::string& name);

	private:
		std::string _name;
		ax::Font _font;

		void OnPaint(ax::GC gc);
	};

	class InspectorMenu : public ax::Window::Backbone {
	public:
		InspectorMenu(const ax::Rect& rect);

		void SetWidgetHandle(ax::Window* handle);
		void RemoveHandle();

		void SetMultipleWidgetSelected(bool on);

	private:
		ax::Window* _selected_handle;
		ax::Font _font;
		ax::Font _font_bold;
		bool _has_multiple_widget_selected;

		using StrPairMsg = ax::event::SimpleMsg<std::pair<std::string, std::string>>;
		axEVENT_DECLARATION(StrPairMsg, OnPyoCallback);
		axEVENT_DECLARATION(StrPairMsg, OnWidgetUpdate);
		axEVENT_DECLARATION(StrPairMsg, OnInfoUpdate);
		axEVENT_DECLARATION(StrPairMsg, OnUniqueName);

		void OnPaint(ax::GC gc);
	};
}
}

#endif /* mdiInspectorMenu_hpp */
