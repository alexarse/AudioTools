#include "atWindowEventsComponent.hpp"
#include "atCommon.hpp"
#include "python/PyUtils.hpp"

namespace at {
std::vector<std::pair<std::string, std::string>> WindowEventsComponent::ParseValuesFromWidgetNode(
	ax::Xml::Node& node)
{
	std::vector<std::pair<std::string, std::string>> fcts_value;
	ax::Xml::Node window_events_node = node.GetNode(at::component::WINDOW_EVENTS);

	if (!window_events_node.IsValid()) {
		return fcts_value;
	}

	ax::Xml::Node evt_node = window_events_node.GetFirstNode();

	while (evt_node.IsValid()) {
		fcts_value.emplace_back(std::pair<std::string, std::string>(evt_node.GetName(), evt_node.GetValue()));
		evt_node = evt_node.GetNextSibling();
	}

	return fcts_value;
}

ax::Window::Event::Function<ax::Point> SetupMouseEvent(
	ax::Window* win, std::function<void(ax::Point)> mouse_fct, const std::string& fct_name)
{
	return ax::WFunc<ax::Point>([win, mouse_fct, fct_name](const ax::Point& pos) {
		if (mouse_fct) {
			mouse_fct(pos);
		}

		if (!win->component.Has(at::component::WINDOW_EVENTS)) {
			return;
		}

		auto we = win->component.Get<at::WindowEventsComponent>(at::component::WINDOW_EVENTS);
		const std::pair<std::string, std::string> fct_value = we->GetFunctionValue(fct_name);

		if (fct_value.first.empty() || fct_value.second.empty()) {
			return;
		}

		ax::python::CallFuncPointParam(fct_value.second, pos);
	});
}

void WindowEventsComponent::SetupWindowEventsWidget(ax::Window* win)
{
	auto m_paint = win->event.OnPaint.GetFunction();
	win->event.OnPaint = ax::WFunc<ax::GC>([win, m_paint](ax::GC gc) {
		if (m_paint) {
			m_paint(gc);
		}

		if (win->component.Has(at::component::WINDOW_EVENTS)) {
			auto we = win->component.Get<at::WindowEventsComponent>(at::component::WINDOW_EVENTS);
			const std::string on_paint_fct = we->GetOnPaint();

			if (on_paint_fct.empty()) {
				return;
			}

			ax::python::CallFuncNoParam(on_paint_fct);
		}
	});

	win->event.OnMouseLeftDown
		= SetupMouseEvent(win, win->event.OnMouseLeftDown.GetFunction(), "MouseLeftDown");
	win->event.OnMouseLeftUp = SetupMouseEvent(win, win->event.OnMouseLeftUp.GetFunction(), "MouseLeftUp");
	win->event.OnMouseMotion = SetupMouseEvent(win, win->event.OnMouseMotion.GetFunction(), "MouseMotion");
	win->event.OnMouseEnter = SetupMouseEvent(win, win->event.OnMouseEnter.GetFunction(), "MouseEnter");
	win->event.OnMouseLeave = SetupMouseEvent(win, win->event.OnMouseLeave.GetFunction(), "MouseLeave");
}

//------------------------------------------------------
WindowEventsComponent::WindowEventsComponent(ax::Window* win)
{
	_win = win;

	_fct_map.insert(std::pair<std::string, std::string&>("Paint", _on_paint));
	_fct_map.insert(std::pair<std::string, std::string&>("MouseLeftDown", _on_m_left_down));
	_fct_map.insert(std::pair<std::string, std::string&>("MouseLeftUp", _on_m_left_up));
	_fct_map.insert(std::pair<std::string, std::string&>("MouseMotion", _on_m_motion));
	_fct_map.insert(std::pair<std::string, std::string&>("MouseEnter", _on_m_enter));
	_fct_map.insert(std::pair<std::string, std::string&>("MouseLeave", _on_m_leave));
}

WindowEventsComponent::~WindowEventsComponent()
{
}

ax::Window* WindowEventsComponent::GetWindow()
{
	return _win;
}

void WindowEventsComponent::SetFunctionValue(const std::pair<std::string, std::string>& fct_value)
{
	auto fct = _fct_map.find(fct_value.first);

	if (fct == _fct_map.end()) {
		return;
	}

	fct->second = fct_value.second;
}

void WindowEventsComponent::SetFunctionsValue(
	const std::vector<std::pair<std::string, std::string>>& fcts_value)
{
	for (auto& n : fcts_value) {
		SetFunctionValue(n);
	}
}

std::pair<std::string, std::string> WindowEventsComponent::GetFunctionValue(const std::string& fct_name) const
{
	auto fct = _fct_map.find(fct_name);

	if (fct == _fct_map.end()) {
		return std::pair<std::string, std::string>("", "");
	}

	return std::pair<std::string, std::string>(fct_name, fct->second);
}

std::vector<std::pair<std::string, std::string>> WindowEventsComponent::GetFunctionsValue() const
{
	std::vector<std::pair<std::string, std::string>> fcts;
	fcts.reserve(_fct_map.size());

	for (auto& n : _fct_map) {
		fcts.emplace_back(std::pair<std::string, std::string>(n.first, n.second));
	}

	return fcts;
}

std::vector<std::string> WindowEventsComponent::GetFunctionNameList() const
{
	std::vector<std::string> fcts_name_list;
	fcts_name_list.reserve(_fct_map.size());

	for (auto& n : _fct_map) {
		fcts_name_list.push_back(n.first);
	}

	return fcts_name_list;
}
}
