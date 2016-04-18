#include "atMidiFeedback.h"

namespace at {
MidiFeedback::MidiFeedback(const ax::Rect& rect)
{
	// Create window.
	win = ax::Window::Create(rect);
	win->event.OnPaint = ax::WBind<ax::GC>(this, &MidiFeedback::OnPaint);
}

void MidiFeedback::OnPaint(ax::GC gc)
{
	const ax::Rect rect(win->dimension.GetDrawingRect());
	gc.SetColor(ax::Color(1.0f, 0.0f, 0.0f));
	gc.DrawRectangle(rect);
}
}