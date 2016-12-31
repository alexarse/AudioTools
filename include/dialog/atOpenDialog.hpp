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

#ifndef __MDI_OPEN_DIALOG_H__
#define __MDI_OPEN_DIALOG_H__

#include <axlib/Button.hpp>
#include <axlib/DropMenu.hpp>
#include <axlib/axlib.hpp>

namespace at {
namespace editor {
	/*
	 * mdi::StatusBar.
	 */
	class OpenDialog : public ax::Window::Backbone {
	public:
		OpenDialog(const ax::Rect& rect);

		virtual ~OpenDialog()
		{
		}

		enum : ax::event::Id { OPEN, CANCEL };

	private:
		std::shared_ptr<ax::DropMenu> _menu;

		axEVENT_DECLARATION(ax::Button::Msg, OnOpen);
		axEVENT_DECLARATION(ax::Button::Msg, OnCancel);
		axEVENT_DECLARATION(ax::DropMenu::Msg, OnMenuSelection);

		void DeleteDialog();

		void OnGlobalClick(const ax::Window::Event::GlobalClick& gclick);
		void OnAssignToWindowManager(const int& v);
		void OnMouseLeftDown(const ax::Point& pos);
		void OnPaint(ax::GC gc);
	};
}
}

#endif // __MDI_OPEN_DIALOG_H__
