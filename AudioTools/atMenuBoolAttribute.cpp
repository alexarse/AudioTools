#include "atMenuBoolAttribute.hpp"
#include <OpenAX/Button.h>
#include <OpenAX/ColorPicker.h>
#include <OpenAX/Label.h>
#include <OpenAX/TextBox.h>
#include <OpenAX/Toggle.h>
#include <OpenAX/WindowManager.h>
#include <OpenAX/Xml.h>

namespace at {
namespace inspector {
	BoolAttribute::BoolAttribute(
		const ax::Rect& rect, const std::string& name, const std::string& value, ax::Event::Function fct)
		: _name(name)
		, _font("fonts/Lato.ttf")
	{
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &BoolAttribute::OnPaint);

		if (fct) {
			win->AddConnection(Events::ASSIGN_VALUE, fct);
		}

		ax::Label::Info labelInfo;
		labelInfo.normal = ax::Color(0.98);
		labelInfo.contour = ax::Color(0.88);
		labelInfo.font_color = ax::Color(0.0);
		labelInfo.font_size = 12;
		labelInfo.font_name = "fonts/Lato.ttf";
		labelInfo.alignement = ax::Utils::Alignement::axALIGN_LEFT;

		ax::Point pos(0, 0);
		win->node.Add(ax::shared<ax::Label>(ax::Rect(pos, ax::Size(90, 25)), labelInfo, _name));

		ax::Toggle::Info tog_info;
		tog_info.normal = ax::Color(0.0, 0.0);
		tog_info.hover = ax::Color(0.0, 0.0);
		tog_info.clicking = ax::Color(0.0, 0.0);
		tog_info.font_color = ax::Color(0.0, 0.0);
		tog_info.selected = ax::Color(0.0, 0.0);
		tog_info.selected_hover = ax::Color(0.0, 0.0);
		tog_info.selected_clicking = ax::Color(0.0, 0.0);
		tog_info.selected_font_color = ax::Color(0.0, 0.0);

		tog_info.contour = ax::Color(0.0, 0.0);

		tog_info.font_size = 12;

		tog_info.img = "resources/bool_toggle.png";
		tog_info.single_img = false;

		win->node.Add(ax::shared<ax::Toggle>(ax::Rect(95, 4, 13, 13), ax::Toggle::Events(), tog_info));
		//		ax::TextBox::Info txtInfo;
		//		txtInfo.normal = ax::Color(1.0);
		//		txtInfo.hover = ax::Color(1.0);
		//		txtInfo.selected = ax::Color(1.0);
		//		txtInfo.highlight = ax::Color(0.4f, 0.4f, 0.6f, 0.2f);
		//		txtInfo.contour = ax::Color(0.88);
		//		txtInfo.cursor = ax::Color(1.0f, 0.0f, 0.0f);
		//		txtInfo.selected_shadow = ax::Color(0.8f, 0.8f, 0.8f);
		//		txtInfo.font_color = ax::Color(0.0);
		//
		//		ax::TextBox::Events txt_evts;
		//
		//		txt_evts.enter_click = ax::Event::Function([&](ax::Event::Msg* msg) {
		//			ax::TextBox::Msg* tmsg = static_cast<ax::TextBox::Msg*>(msg);
		//			std::string msg_str = tmsg->GetMsg();
		//
		//			win->PushEvent(Events::ASSIGN_VALUE,
		//				new ax::Event::SimpleMsg<ax::StringPair>(ax::StringPair(_name, msg_str)));
		//		});
		//
		//		win->node.Add(ax::shared<ax::TextBox>(
		//			ax::Rect(ax::Point(90, 0), ax::Size(rect.size.x - 90, 25)), txt_evts, txtInfo, "",
		//_value));

		//		win->node.Add(ax::shared<ax::Button>(
		//			ax::Rect(ax::Point(90, 0), ax::Size(rect.size.x - 90, 25)), ax::Button::Events(),
		// ax::Button::Info(), "", ""));
	}

	//		void ColorAttribute::OnColorSelect(const ax::ColorPicker::Msg& msg)
	//		{
	//			win->PushEvent(Events::ASSIGN_VALUE,
	//						   new ax::Event::SimpleMsg<ax::StringPair>(ax::StringPair(_name,
	//msg.GetMsg().ToString())));
	//
	//			ax::App& app(ax::App::GetInstance());
	//			auto w = app.GetPopupManager()->GetWindowTree()->GetTopLevel();
	//			//		app.GetWindowManager()->RemoveGlobalClickListener(win);
	//			app.GetPopupManager()->SetPastWindow(nullptr);
	//			app.GetPopupManager()->UnGrabKey();
	//			app.GetPopupManager()->UnGrabMouse();
	//
	//			w->event.UnGrabKey();
	//			w->event.UnGrabMouse();
	//
	//			w->backbone = nullptr;
	//
	//			app.GetPopupManager()->GetWindowTree()->GetNodeVector().clear();
	//			app.GetPopupManager()->SetPastWindow(nullptr);
	//			app.UpdateAll();
	//		}

	//		void ColorAttribute::OnColorCancel(const ax::ColorPicker::Msg& msg)
	//		{
	//		}
	//
	//		void ColorAttribute::OnMouseLeftDown(const ax::Point& pos)
	//		{
	//			auto c_picker = ax::shared<ax::ColorPicker>(
	//														ax::Rect(win->dimension.GetAbsoluteRect().position.x - 120, 30, 205, 272),
	//_color);
	//
	//			ax::App::GetInstance().GetPopupManager()->GetWindowTree()->AddTopLevel(
	//																				   ax::Window::Ptr(c_picker->GetWindow()));
	//
	//			c_picker->GetWindow()->backbone = c_picker;
	//			c_picker->GetWindow()->AddConnection(ax::ColorPicker::Events::SELECT, GetOnColorSelect());
	//		}

	void BoolAttribute::OnPaint(ax::GC gc)
	{
		const ax::Rect rect(win->dimension.GetDrawingRect());

		gc.SetColor(ax::Color(0.94));
		gc.DrawRectangle(rect);

		gc.SetColor(ax::Color(1.0));
		gc.DrawRectangle(rect.GetInteriorRect(ax::Point(1, 1)));

		gc.SetColor(ax::Color(0.94));
		gc.DrawRectangleContour(rect);
	}
}
}