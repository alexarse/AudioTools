#include "PyoComponent.h"
#include "axEditorInspectorMenu.h"
#include "axLabel.h"
#include "axTextBox.h"
#include "atCommon.h"

namespace ax {
namespace editor {
	MenuSeparator::MenuSeparator(const ax::Rect& rect, const std::string& name)
		: _name(name)
		, _font("fonts/FreeSansBold.ttf")
	{
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &MenuSeparator::OnPaint);
	}

	void MenuSeparator::OnPaint(ax::GC gc)
	{
		ax::Rect rect(win->dimension.GetDrawingRect());

		gc.SetColor(ax::Color(0.94));
		gc.DrawRectangle(rect);

		gc.SetColor(ax::Color(0.3));
		gc.DrawString(_font, _name, ax::Point(10, 2));

		gc.SetColor(ax::Color(0.94));
		gc.DrawRectangleContour(rect);
	}

	MenuAttribute::MenuAttribute(const ax::Rect& rect, const std::string& name,
		const std::string& value, ax::Event::Function fct)
		: _name(name)
		, _value(value)
		, _font("fonts/Lato.ttf")
	{
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &MenuAttribute::OnPaint);

		if (fct) {
			win->AddConnection(99, fct);
		}

		ax::TextBox::Info txtInfo;
		txtInfo.normal = ax::Color(1.0);
		txtInfo.hover = ax::Color(1.0);
		txtInfo.selected = ax::Color(1.0);
		txtInfo.highlight = ax::Color(0.4f, 0.4f, 0.6f);
		txtInfo.contour = ax::Color(0.88);
		txtInfo.cursor = ax::Color(1.0f, 0.0f, 0.0f);
		txtInfo.selected_shadow = ax::Color(0.8f, 0.8f, 0.8f);
		txtInfo.font_color = ax::Color(0.0);

		ax::Label::Info labelInfo;
		labelInfo.normal = ax::Color(0.98);
		labelInfo.contour = ax::Color(0.88);
		labelInfo.font_color = ax::Color(0.0);
		labelInfo.font_size = 12;
		labelInfo.font_name = "fonts/Lato.ttf";
		labelInfo.alignement = ax::Utils::Alignement::axALIGN_LEFT;

		ax::Point pos(0, 0);
		win->node.Add(ax::shared<ax::Label>(
			ax::Rect(pos, ax::Size(90, 25)), labelInfo, _name));

		ax::TextBox::Events txt_evts;
		txt_evts.enter_click = ax::Event::Function([&](ax::Event::Msg* msg) {
			ax::TextBox::Msg* tmsg = static_cast<ax::TextBox::Msg*>(msg);
			std::string msg_str = tmsg->GetMsg();
			win->PushEvent(99, new ax::Event::SimpleMsg<ax::StringPair>(
								   ax::StringPair(_name, msg_str)));
		});

		win->node.Add(ax::shared<ax::TextBox>(
			ax::Rect(ax::Point(90, 0), ax::Size(rect.size.x - 90, 25)),
			txt_evts, txtInfo, "", _value));
	}

	void MenuAttribute::OnPaint(ax::GC gc)
	{
		ax::Rect rect(win->dimension.GetDrawingRect());

		gc.SetColor(ax::Color(0.94));
		gc.DrawRectangle(rect);

		gc.SetColor(ax::Color(0.94));
		gc.DrawRectangleContour(rect);
	}

	InspectorMenu::InspectorMenu(const ax::Rect& rect)
		: _selected_handle(nullptr)
		, _font("fonts/Lato.ttf")
	{
		// Create window.
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &InspectorMenu::OnPaint);

//		_info_txt.push_back("Bang");
	}

	void InspectorMenu::SetWidgetHandle(ax::Window* handle)
	{
		// Clear old content.
		RemoveHandle();
		_selected_handle = handle;

		if (_selected_handle) {
			ax::Rect rect(win->dimension.GetRect());

			ax::Size separator_size(rect.size.x, 20);

			// Add widget separator.
			win->node.Add(ax::shared<MenuSeparator>(
				ax::Rect(ax::Point(0, 0), separator_size), "Widget"));

			ax::widget::Component::Ptr widget
				= _selected_handle->component.Get<ax::widget::Component>(
					"Widget");
			ax::widget::Info* info = widget->GetInfo();

			ax::StringVector info_atts = info->GetParamNameList();
			ax::StringPairVector atts_pair = widget->GetBuilderAttributes();

			ax::Point att_pos(0, 20);
			ax::Size att_size(rect.size.x, 20);

			_info_txt.clear();

			for (auto& n : atts_pair) {
				win->node.Add(
					ax::shared<MenuAttribute>(ax::Rect(att_pos, att_size),
						n.first, n.second, GetOnWidgetUpdate()));
				att_pos.y += att_size.y;
			}

			win->node.Add(ax::shared<MenuSeparator>(
				ax::Rect(att_pos, separator_size), "Info"));

			att_pos.y += separator_size.y;

			for (auto& n : info_atts) {
				std::string value = info->GetAttributeValue(n);
				win->node.Add(ax::shared<MenuAttribute>(
					ax::Rect(att_pos, att_size), n, value, GetOnInfoUpdate()));
				att_pos.y += att_size.y;
			}

			// pyo function.
			if (_selected_handle->component.Has("pyo")) {
				pyo::Component::Ptr pyo_comp
					= _selected_handle->component.Get<pyo::Component>("pyo");

				win->node.Add(ax::shared<MenuSeparator>(
					ax::Rect(att_pos, separator_size), "Pyo"));

				att_pos.y += separator_size.y;

				std::string fct_name = pyo_comp->GetFunctionName();
				auto menu
					= ax::shared<MenuAttribute>(ax::Rect(att_pos, att_size),
						"callback", fct_name, GetOnPyoCallback());
				win->node.Add(menu);

				att_pos.y += att_size.y;
			}
		}
		win->Update();
	}

	void InspectorMenu::RemoveHandle()
	{
		if (_selected_handle != nullptr) {
			win->node.GetChildren().clear();
		}
		_selected_handle = nullptr;
	}

	void InspectorMenu::OnPyoCallback(
		const ax::Event::SimpleMsg<ax::StringPair>& msg)
	{
		ax::Print("Pyocallback");
		if (_selected_handle == nullptr) {
			return;
		}

		if (msg.GetMsg().first == "callback") {
			if (!_selected_handle->component.Has("pyo")) {
				return;
			}

			/// @todo Check string before.
			pyo::Component::Ptr pyo_comp
				= _selected_handle->component.Get<pyo::Component>("pyo");
			pyo_comp->SetFunctionName(msg.GetMsg().second);
			return;
		}
	}

	void InspectorMenu::OnWidgetUpdate(
		const ax::Event::SimpleMsg<ax::StringPair>& msg)
	{
		ax::Print("Pyocallback");
		if (_selected_handle == nullptr) {
			return;
		}

		if (_selected_handle->component.Has("Widget")) {
			ax::widget::Component::Ptr widget
				= _selected_handle->component.Get<ax::widget::Component>(
					"Widget");

			widget->SetBuilderAttributes(ax::StringPairVector{ msg.GetMsg() });
		}
	}

	void InspectorMenu::OnInfoUpdate(
		const ax::Event::SimpleMsg<ax::StringPair>& msg)
	{
		ax::Print("Pyocallback");
		if (_selected_handle == nullptr) {
			return;
		}
	}

	void InspectorMenu::OnPaint(ax::GC gc)
	{
		ax::Rect rect(win->dimension.GetDrawingRect());

		gc.SetColor(ax::Color(1.0));
		gc.DrawRectangle(rect);

		ax::Point pos(5, 25);
		gc.SetColor(ax::Color(0.0));
		for (auto& n : _info_pair_txt) {
			gc.DrawString(_font, n.first + " -> " + n.second, pos);
			pos.y += 20;
		}

		gc.SetColor(ax::Color(0.7));
		gc.DrawRectangleContour(rect);
	}
}
}