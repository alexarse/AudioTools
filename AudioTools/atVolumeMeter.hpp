#ifndef atVolumeMeter_hpp
#define atVolumeMeter_hpp

#include <OpenAX/OpenAX.h>

namespace at {
/*
 * VolumeMeter.
 */
class VolumeMeter : public ax::Window::Backbone {
public:
	VolumeMeter(const ax::Rect& rect);

	void SetValue(double value)
	{
		_value = ax::Utils::Clamp<double>(value, 0.0, 1.0);
	}

private:
	double _value;

	void OnPaint(ax::GC gc);
};
}

#endif /* atVolumeMeter_hpp */
