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

#ifndef atMenuSizeAttribute_hpp
#define atMenuSizeAttribute_hpp

#include <axlib/NumberScroll.hpp>
#include <axlib/axlib.hpp>

namespace at {
namespace inspector {
	class SizeAttribute : public ax::Window::Backbone {
	public:
		enum Events : ax::event::Id { ASSIGN_VALUE };

		SizeAttribute(
			const ax::Rect& rect, const std::string& name, const std::string& value, ax::event::Function fct);

		void SetValue(const std::string& value);

	private:
		std::string _name;
		ax::Font _font;
		ax::NumberScroll* _width_scroll;
		ax::NumberScroll* _height_scroll;

		axEVENT_DECLARATION(ax::NumberScroll::Msg, OnWidthChange);
		axEVENT_DECLARATION(ax::NumberScroll::Msg, OnHeightChange);

		void OnPaint(ax::GC gc);
	};
}
}

#endif /* atMenuSizeAttribute_hpp */
