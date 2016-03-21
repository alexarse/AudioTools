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
