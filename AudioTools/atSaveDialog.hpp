#ifndef __MDI_SAVE_DIALOG_H__
#define __MDI_SAVE_DIALOG_H__

#include <OpenAX/Button.h>
#include <OpenAX/OpenAX.h>
#include <OpenAX/TextBox.h>

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

		enum : ax::Event::Id { SAVE, CANCEL };

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
