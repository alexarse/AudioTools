#ifndef mdiStatusBar_hpp
#define mdiStatusBar_hpp

#include <OpenAX/OpenAX.h>

#include <OpenAX/Button.h>
#include <OpenAX/Toggle.h>

#include "atVolumeMeter.hpp"

namespace at {
namespace editor {
	/*
	 * ax::editor::StatusBar.
	 */
	class StatusBar : public ax::Window::Backbone {
	public:
		StatusBar(const ax::Rect& rect);

		enum : ax::Event::Id {
			SAVE_LAYOUT,
			SAVE_AS_LAYOUT,
			OPEN_LAYOUT,
			VIEW_LAYOUT,
			RELOAD_SCRIPT,
			TOGGLE_LEFT_PANEL,
			TOGGLE_BOTTOM_PANEL,
			TOGGLE_RIGHT_PANEL
		};
		
		void SetLayoutFilePath(const std::string& path)
		{
			_layout_file_path = path;
			win->Update();
		}

	private:
		ax::Font _font;
		ax::Toggle* _toggle_left;
		ax::Toggle* _toggle_bottom;
		ax::Toggle* _toggle_right;
		at::VolumeMeter* _volumeMeterLeft;
		at::VolumeMeter* _volumeMeterRight;
		std::string _layout_file_path;
		
		axEVENT_DECLARATION(ax::Button::Msg, OnSaveLayout);
		axEVENT_DECLARATION(ax::Button::Msg, OnSaveAsLayout);
		axEVENT_DECLARATION(ax::Button::Msg, OnOpenLayout);
		axEVENT_DECLARATION(ax::Button::Msg, OnViewLayout);
		axEVENT_DECLARATION(ax::Button::Msg, OnReload);
		axEVENT_DECLARATION(ax::Button::Msg, OnSettings);

		axEVENT_DECLARATION(ax::Toggle::Msg, OnToggleLeftPanel);
		axEVENT_DECLARATION(ax::Toggle::Msg, OnToggleBottomPanel);
		axEVENT_DECLARATION(ax::Toggle::Msg, OnToggleRightPanel);

		axEVENT_DECLARATION(ax::Event::StringMsg, OnSaveDialog);
		axEVENT_DECLARATION(ax::Event::StringMsg, OnCancelDialog);
		
		axEVENT_DECLARATION(ax::Event::StringMsg, OnOpenDialog);
		
		typedef std::pair<double, double> StereoRmsValue;
		
		axEVENT_DECLARATION(ax::Event::SimpleMsg<StereoRmsValue>, OnAudioRmsValue);

		void OnResize(const ax::Size& size);
		void OnPaint(ax::GC gc);
	};
}
}

#endif /* mdiStatusBar_hpp */
