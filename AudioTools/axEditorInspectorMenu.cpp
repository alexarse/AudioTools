#include "PyoComponent.h"
#include "atCommon.h"
#include "atEditorInspectorMenu.h"
#include "atMenuAttribute.h"
#include "atMenuBoolAttribute.hpp"
#include "atMenuColorAttribute.hpp"
#include "atMenuIntegerAttribute.hpp"
#include "atMenuSizeAttribute.hpp"
#include <OpenAX/WindowManager.h>

namespace at {
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

	/*
	 * InspectorMenu.
	 */
	InspectorMenu::InspectorMenu(const ax::Rect& rect)
		: _selected_handle(nullptr)
		, _font("fonts/Lato.ttf")
	{
		// Create window.
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &InspectorMenu::OnPaint);
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
			win->node.Add(ax::shared<MenuSeparator>(ax::Rect(ax::Point(0, 0), separator_size), "Widget"));

			ax::widget::Component::Ptr widget
				= _selected_handle->component.Get<ax::widget::Component>("Widget");

			// Builder attributes.
			ax::StringPairVector atts_pair = widget->GetBuilderAttributes();
			std::map<std::string, std::string> atts_map;

			for (auto& n : atts_pair) {
				atts_map.insert(n);
			}

			std::vector<ax::widget::ParamInfo> builder_atts_info = widget->GetBuilderAttributesInfo();

			ax::Point att_pos(0, 20);
			ax::Size att_size(rect.size.x, 20);

			for (auto& n : builder_atts_info) {
//				std::string value = info->GetAttributeValue(n.second);
				std::string value = atts_map[n.second];

				if (n.first == ax::widget::ParamType::COLOR) {
					win->node.Add(ax::shared<at::inspector::ColorAttribute>(
						ax::Rect(att_pos, att_size), n.second, value, GetOnWidgetUpdate()));
				}
				else if (n.first == ax::widget::ParamType::BOOLEAN) {
					win->node.Add(ax::shared<at::inspector::BoolAttribute>(
						ax::Rect(att_pos, att_size), n.second, value, GetOnWidgetUpdate()));
				}
				else if (n.first == ax::widget::ParamType::SIZE) {
					win->node.Add(ax::shared<at::inspector::SizeAttribute>(
						ax::Rect(att_pos, att_size), n.second, value, GetOnWidgetUpdate()));
				}
				else if (n.first == ax::widget::ParamType::INTEGER) {
					win->node.Add(ax::shared<at::inspector::IntegerAttribute>(
						ax::Rect(att_pos, att_size), n.second, value, GetOnWidgetUpdate()));
				}
				else {
					//					ax::Print("Attribute :", n.second, value);
					win->node.Add(ax::shared<at::inspector::MenuAttribute>(
						ax::Rect(att_pos, att_size), n.second, value, GetOnWidgetUpdate()));
				}

				att_pos.y += att_size.y;
			}

			//			for (auto& n : atts_pair) {
			//				win->node.Add(ax::shared<at::inspector::MenuAttribute>(
			//					ax::Rect(att_pos, att_size), n.first, n.second, GetOnWidgetUpdate()));
			//				att_pos.y += att_size.y;
			//			}

			win->node.Add(ax::shared<MenuSeparator>(ax::Rect(att_pos, separator_size), "Info"));

			att_pos.y += separator_size.y;

			// Widget info attributes.
			ax::widget::Info* info = widget->GetInfo();
			std::vector<ax::widget::ParamInfo> info_atts = info->GetParametersInfo();

			for (auto& n : info_atts) {
				std::string value = info->GetAttributeValue(n.second);

				if (n.first == ax::widget::ParamType::COLOR) {
					win->node.Add(ax::shared<at::inspector::ColorAttribute>(
						ax::Rect(att_pos, att_size), n.second, value, GetOnInfoUpdate()));
				}
				else if (n.first == ax::widget::ParamType::BOOLEAN) {
					win->node.Add(ax::shared<at::inspector::BoolAttribute>(
						ax::Rect(att_pos, att_size), n.second, value, GetOnInfoUpdate()));
				}
				else if (n.first == ax::widget::ParamType::SIZE) {
					win->node.Add(ax::shared<at::inspector::SizeAttribute>(
						ax::Rect(att_pos, att_size), n.second, value, GetOnInfoUpdate()));
				}
				else if (n.first == ax::widget::ParamType::INTEGER) {
					win->node.Add(ax::shared<at::inspector::IntegerAttribute>(
						ax::Rect(att_pos, att_size), n.second, value, GetOnInfoUpdate()));
				}
				else {
					//					ax::Print("Attribute :", n.second, value);
					win->node.Add(ax::shared<at::inspector::MenuAttribute>(
						ax::Rect(att_pos, att_size), n.second, value, GetOnInfoUpdate()));
				}

				att_pos.y += att_size.y;
			}

			// Python attributes.
			if (_selected_handle->component.Has("pyo")) {
				pyo::Component::Ptr pyo_comp = _selected_handle->component.Get<pyo::Component>("pyo");

				win->node.Add(ax::shared<MenuSeparator>(ax::Rect(att_pos, separator_size), "Pyo"));

				att_pos.y += separator_size.y;

				std::string fct_name = pyo_comp->GetFunctionName();

				auto menu = ax::shared<at::inspector::MenuAttribute>(
					ax::Rect(att_pos, att_size), "callback", fct_name, GetOnPyoCallback());
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
		ax::App::GetInstance().GetWindowManager()->SetPastKeyWindow(nullptr);
		win->Update();
	}

	void InspectorMenu::OnPyoCallback(const ax::Event::SimpleMsg<ax::StringPair>& msg)
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
			pyo::Component::Ptr pyo_comp = _selected_handle->component.Get<pyo::Component>("pyo");
			pyo_comp->SetFunctionName(msg.GetMsg().second);
			return;
		}
	}

	void InspectorMenu::OnWidgetUpdate(const ax::Event::SimpleMsg<ax::StringPair>& msg)
	{
		ax::Print("Pyocallback");
		if (_selected_handle == nullptr) {
			return;
		}

		if (_selected_handle->component.Has("Widget")) {
			ax::widget::Component::Ptr widget
				= _selected_handle->component.Get<ax::widget::Component>("Widget");

			widget->SetBuilderAttributes(ax::StringPairVector{ msg.GetMsg() });
		}
	}

	void InspectorMenu::OnInfoUpdate(const ax::Event::SimpleMsg<ax::StringPair>& msg)
	{
		ax::Print("Pyocallback");
		if (_selected_handle == nullptr) {
			return;
		}
		ax::widget::Component::Ptr widget = _selected_handle->component.Get<ax::widget::Component>("Widget");

		widget->SetInfo(ax::StringPairVector{ msg.GetMsg() });
		widget->ReloadInfo();
	}

	void InspectorMenu::OnPaint(ax::GC gc)
	{
		ax::Rect rect(win->dimension.GetDrawingRect());

		gc.SetColor(ax::Color(255, 255, 255));
		gc.DrawRectangle(rect);

		gc.SetColor(ax::Color(0.7));
		gc.DrawRectangleContour(rect);
	}
}
}