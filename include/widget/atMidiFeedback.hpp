#ifndef __AT_MIDI_FEEDBACK_H__
#define __AT_MIDI_FEEDBACK_H__

#include <axlib/axlib.hpp>

namespace at {
/*
 * MidiFeedback.
 */
class MidiFeedback : public ax::Window::Backbone {
public:
	MidiFeedback(const ax::Rect& rect);

private:
	void OnPaint(ax::GC gc);
};
}

#endif // __AT_MIDI_FEEDBACK_H__
