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

#ifndef atPreferenceDialog_hpp
#define atPreferenceDialog_hpp

#include <OpenAX/Button.h>
#include <OpenAX/DropMenu.h>
#include <OpenAX/OpenAX.h>



namespace at {
	namespace editor {
		class PreferencePanel : public ax::Window::Backbone {
		public:
			PreferencePanel(const ax::Rect& rect);
			
			virtual ~PreferencePanel()
			{
			}
			
			//			enum : ax::Event::Id { OPEN, CANCEL };
			
		private:
			//			std::shared_ptr<ax::DropMenu> _menu;
			
			//			axEVENT_DECLARATION(ax::Button::Msg, OnOpen);
			//			axEVENT_DECLARATION(ax::Button::Msg, OnCancel);
			//			axEVENT_DECLARATION(ax::DropMenu::Msg, OnMenuSelection);
			
//			void DeleteDialog();
//			
//			void OnGlobalClick(const ax::Window::Event::GlobalClick& gclick);
//			void OnMouseLeftDown(const ax::Point& pos);
			void OnPaint(ax::GC gc);
		};
	
		class PreferenceDialog : public ax::Window::Backbone {
		public:
			PreferenceDialog(const ax::Rect& rect);
			
			virtual ~PreferenceDialog()
			{
			}
			
//			enum : ax::Event::Id { OPEN, CANCEL };
			
		private:
//			std::shared_ptr<ax::DropMenu> _menu;
			
//			axEVENT_DECLARATION(ax::Button::Msg, OnOpen);
//			axEVENT_DECLARATION(ax::Button::Msg, OnCancel);
//			axEVENT_DECLARATION(ax::DropMenu::Msg, OnMenuSelection);
			
			void DeleteDialog();
			
			void OnGlobalClick(const ax::Window::Event::GlobalClick& gclick);
			void OnMouseLeftDown(const ax::Point& pos);
			void OnPaint(ax::GC gc);
		};
	}
}

#endif /* atPreferenceDialog_hpp */
