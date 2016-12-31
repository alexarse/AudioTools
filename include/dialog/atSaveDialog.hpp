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

#ifndef __MDI_SAVE_DIALOG_H__
#define __MDI_SAVE_DIALOG_H__

#include <axlib/Button.hpp>
#include <axlib/TextBox.hpp>
#include <axlib/axlib.hpp>

namespace at {
namespace editor {
	/*
	 * mdi::StatusBar.
	 */
	class SaveDialog : public ax::Window::Backbone {
	public:
		SaveDialog(const ax::Rect& rect, const std::string& default_name = "");

		virtual ~SaveDialog()
		{
		}

		enum : ax::event::Id { SAVE, CANCEL };

	private:
		std::shared_ptr<ax::TextBox> _txtBox;

		axEVENT_DECLARATION(ax::Button::Msg, OnSave);
		axEVENT_DECLARATION(ax::Button::Msg, OnCancel);

		void DeleteDialog();

		void OnGlobalClick(const ax::Window::Event::GlobalClick& gclick);

		void OnPaint(ax::GC gc);
	};
}
}

#endif // __MDI_SAVE_DIALOG_H__
