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

#pragma once

#include <axlib/axlib.hpp>
#include <map>

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
		bool _has_grid_window_connection = false;

		ax::Window::Backbone* _widget_build_pos_att = nullptr;
		ax::Window::Backbone* _widget_build_size_att = nullptr;

		using BuilderFct = std::function<std::shared_ptr<ax::Window::Backbone>(const ax::Rect& rect,
			const std::string& name, const std::string& value, ax::event::Function fct)>;
		std::map<ax::widget::ParamType, BuilderFct> _att_builder_map;

		using StrPairMsg = ax::event::SimpleMsg<std::pair<std::string, std::string>>;
		axEVENT_DECLARATION(StrPairMsg, OnPyoCallback);
		axEVENT_DECLARATION(StrPairMsg, OnWidgetUpdate);
		axEVENT_DECLARATION(StrPairMsg, OnInfoUpdate);
		axEVENT_DECLARATION(StrPairMsg, OnUniqueName);
		axEVENT_DECLARATION(StrPairMsg, OnClassName);
		axEVENT_DECLARATION(StrPairMsg, OnWindowEvents);

		// From grid window.
		axEVENT_DECLARATION(ax::event::EmptyMsg, OnDraggingWidget);
		axEVENT_DECLARATION(ax::event::EmptyMsg, OnWidgetResize);
		axEVENT_DECLARATION(ax::event::SimpleMsg<ax::util::Direction>, OnArrowMoveSelectedWidget);

		void OnPaint(ax::GC gc);
	};
}
}
