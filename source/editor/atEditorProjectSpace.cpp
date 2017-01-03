#include "editor/atEditorProjectSpace.hpp"
#include "atUniqueNameComponent.h"
#include "editor/atEditor.hpp"
#include "editor/atEditorGridWindow.hpp"
#include "editor/atEditorMainWindow.hpp"

namespace at {
namespace editor {
	std::string GetWidgetName(ax::Window* w)
	{
		if (w == nullptr) {
			return "";
		}

		if (!w->component.Has("Widget")) {
			return "";
		}

		ax::widget::Component::Ptr w_comp = w->component.Get<ax::widget::Component>("Widget");
		return w_comp->GetBuilderName();
	}

	ProjectSpaceObj::ProjectSpaceObj(
		const ax::Rect& rect, const std::string& name, ax::Window* widget, int level)
		: _font(0)
		, _font_bold("fonts/FreeSansBold.ttf")
		, _name(name)
		, _widget(widget)
		, _level(level)
	{
		// Create window.
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &ProjectSpaceObj::OnPaint);
		win->event.OnMouseLeftDoubleClick
			= ax::WBind<ax::Point>(this, &ProjectSpaceObj::OnMouseLeftDoubleClick);

		if (widget == nullptr) {
			return;
		}

		// Icon.
		if (_name == "Panel") {
			_icon_color = ax::Color(0.2);
			_icon = std::make_shared<ax::Image>("resources/tree_icon_panel.png");
		}
		else if (_name == "Button") {
			_icon_color = ax::Color(200, 0, 0);
			_icon = std::make_shared<ax::Image>("resources/tree_icon.png");
		}
		else if (_name == "Knob") {
			_icon_color = ax::Color(200, 200, 0);
			_icon = std::make_shared<ax::Image>("resources/tree_icon.png");
		}
		else if (_name == "Toggle") {
			_icon_color = ax::Color(200, 0, 200);
			_icon = std::make_shared<ax::Image>("resources/tree_icon.png");
		}
		else if (_name == "Sprite") {
			_icon_color = ax::Color(0, 200, 200);
			_icon = std::make_shared<ax::Image>("resources/tree_icon.png");
		}
		else {
			_icon_color = ax::Color(0.7);
			_icon = std::make_shared<ax::Image>("resources/tree_icon.png");
		}

		if (widget->component.Has("unique_name")) {
			at::UniqueNameComponent::Ptr uname
				= widget->component.Get<at::UniqueNameComponent>("unique_name");
			_unique_name = uname->GetName();
		}

		// GetSelectedWindows

		std::vector<std::shared_ptr<ax::Window>>& children = _widget->node.GetChildren();

		ax::Point pos(0, rect.size.h);

		for (auto w : children) {
			const std::string w_name = GetWidgetName(w.get());
			if (w_name.empty()) {
				continue;
			}

			const ax::Size size(win->dimension.GetSize());
			auto obj1 = std::make_shared<ProjectSpaceObj>(
				ax::Rect(0, pos.y, size.w, 25), w_name, w.get(), _level + 1);
			win->node.Add(obj1);
			pos = obj1->GetWindow()->dimension.GetRect().GetNextPosDown(-1);
		}

		win->dimension.SetSize(ax::Size(rect.size.w, pos.y));
	}

	void ProjectSpaceObj::UnSelectAll()
	{
		if (_is_selected) {
			_is_selected = false;
			win->Update();
		}

		for (auto n : win->node.GetChildren()) {
			n->GetBackbone<ProjectSpaceObj>()->UnSelectAll();
		}
	}

	void ProjectSpaceObj::SetSelectedWidgets(const std::vector<ax::Window*>& widgets)
	{
		if (_widget == nullptr) {
			return;
		}

		for (ax::Window* w : widgets) {
			if (_widget->GetId() == w->GetId()) {
				_is_selected = true;
				win->Update();
				break;
			}
		}

		for (auto n : win->node.GetChildren()) {
			n->GetBackbone<ProjectSpaceObj>()->SetSelectedWidgets(widgets);
		}
	}

	void ProjectSpaceObj::OnMouseLeftDoubleClick(const ax::Point& pos)
	{
		editor::App* app = editor::App::GetInstance();
		editor::MainWindow* main_win = app->GetMainWindow();
		editor::GridWindow* grid_win = main_win->GetGridWindow();
		grid_win->GetWindow()->PushEvent(
			at::editor::GridWindow::SELECT_WIDGET, new ax::event::SimpleMsg<ax::Window*>(_widget));

		_is_selected = true;
		win->Update();
	}

	void ProjectSpaceObj::OnPaint(ax::GC gc)
	{
		const ax::Rect rect(win->dimension.GetDrawingRect());

		_is_selected ? gc.SetColor(ax::Color(0.95)) : gc.SetColor(ax::Color(1.0));
		gc.DrawRectangle(rect);

		if (_icon->IsImageReady()) {
			gc.DrawImageColor(_icon.get(), ax::Point(10 + _level * 15, 6), _icon_color);
		}

		_is_selected ? gc.SetColor(ax::Color(250, 172, 0)) : gc.SetColor(ax::Color(0.3));
		gc.DrawString(_font_bold, _name, ax::Point(25 + _level * 15, 4));

		if (!_unique_name.empty()) {
			gc.SetColor(ax::Color(0.3));
			gc.DrawStringAlignedRight(
				_font, _unique_name, ax::Rect(rect.position, rect.size - ax::Size(10, 0)));
		}

		gc.SetColor(ax::Color(0.7));
		gc.DrawRectangleContour(rect);
	}

	//
	ProjectSpace::ProjectSpace(const ax::Rect& rect)
		: _font(0)
		, _font_bold("fonts/FreeSansBold.ttf")
	{
		// Create window.
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &ProjectSpace::OnPaint);
		win->event.OnResize = ax::WBind<ax::Size>(this, &ProjectSpace::OnResize);

		// Create scrolling window.
		_panel = ax::Window::Create(ax::Rect(0, 0, rect.size.w, rect.size.h));
		win->node.Add(std::shared_ptr<ax::Window>(_panel));

		ax::Point pos(0, 0);

		ax::ScrollBar::Info sInfo;
		sInfo.normal = ax::Color(0.80, 0.3);
		sInfo.hover = ax::Color(0.85, 0.3);
		sInfo.clicking = ax::Color(0.82, 0.3);
		sInfo.slider_contour = ax::Color(0.6, 0.2);
		sInfo.contour = ax::Color(0.9, 0.2);
		sInfo.bg_top = ax::Color(0.9, 0.2);
		sInfo.bg_bottom = ax::Color(0.92, 0.2);

		const ax::Rect sRect(rect.size.w - 9, 0, 10, rect.size.h);
		auto sb = std::make_shared<ax::ScrollBar>(sRect, ax::ScrollBar::Events(), sInfo);
		_scroll_bar = sb.get();
		win->node.Add(sb);

		_panel->property.AddProperty("BlockDrawing");
		_panel->dimension.SetSizeNoShowRect(ax::Size(rect.size.w, rect.size.h));

		_scroll_bar->SetWindowHandle(_panel);
		_scroll_bar->UpdateWindowSize(_panel->dimension.GetSize());
	}

	void ProjectSpace::UpdateTree()
	{
		if (_has_grid_window_connection == false) {
			_has_grid_window_connection = true;

			ax::Window* gw = editor::App::GetInstance()->GetMainWindow()->GetGridWindow()->GetWindow();
			gw->AddConnection(GridWindow::SELECT_WIDGET, GetOnSelectWidget());
			gw->AddConnection(GridWindow::UNSELECT_ALL, GetOnUnSelectAllWidget());
			gw->AddConnection(GridWindow::SELECT_MULTIPLE_WIDGET, GetOnSelectMultipleWidget());
			gw->AddConnection(GridWindow::DELETE_SELECTED_WIDGET, GetOnWidgetAddedOrRemoved());
			gw->AddConnection(GridWindow::DUPLICATE_SELECTED_WIDGET, GetOnWidgetAddedOrRemoved());
			gw->AddConnection(
				GridWindow::DELETE_SELECTED_WIDGET_FROM_RIGHT_CLICK, GetOnWidgetAddedOrRemoved());
			gw->AddConnection(
				GridWindow::DUPLICATE_SELECTED_WIDGET_FROM_RIGHT_CLICK, GetOnWidgetAddedOrRemoved());
		}

		// Remove all children.
		_panel->node.GetChildren().clear();
		_panel->Update();

		editor::App* app = editor::App::GetInstance();
		editor::MainWindow* main_win = app->GetMainWindow();
		editor::GridWindow* grid_win = main_win->GetGridWindow();
		ax::Window* main_widget = grid_win->GetMainWindow();

		if (main_widget == nullptr) {
			_has_objects = false;
			win->Update();
			return;
		}

		const ax::Size size(win->dimension.GetSize());
		auto obj1 = std::make_shared<ProjectSpaceObj>(
			ax::Rect(0, 0, size.w, 25), GetWidgetName(main_widget), main_widget, 0);
		_panel->node.Add(obj1);

		_panel->dimension.SetSizeNoShowRect(
			ax::Size(size.w, obj1->GetWindow()->dimension.GetRect().GetNextPosDown(0).y)); // pos.y));
		_scroll_bar->UpdateWindowSize(_panel->dimension.GetSize());

		SetSelectedWidgets(editor::App::GetInstance()->GetMainWindow()->GetSelectedWindows());
		_has_objects = true;
		win->Update();
	}

	void ProjectSpace::UnSelectAll()
	{
		for (auto n : _panel->node.GetChildren()) {
			n->GetBackbone<ProjectSpaceObj>()->UnSelectAll();
		}
	}

	void ProjectSpace::SetSelectedWidgets(const std::vector<ax::Window*>& widgets)
	{
		for (auto n : _panel->node.GetChildren()) {
			n->GetBackbone<ProjectSpaceObj>()->SetSelectedWidgets(widgets);
		}
	}

	void ProjectSpace::OnSelectWidget(const ax::event::SimpleMsg<ax::Window*>& msg)
	{
		ax::Window* w = msg.GetMsg();
		std::vector<ax::Window*> ws(1);
		ws[0] = w;

		UnSelectAll();
		SetSelectedWidgets(ws);
	}

	void ProjectSpace::OnUnSelectAllWidget(const ax::event::SimpleMsg<int>& msg)
	{
		UnSelectAll();
	}

	void ProjectSpace::OnSelectMultipleWidget(const ax::event::SimpleMsg<std::vector<ax::Window*>>& msg)
	{
		UnSelectAll();
		SetSelectedWidgets(msg.GetMsg());
	}

	void ProjectSpace::OnWidgetAddedOrRemoved(const ax::event::EmptyMsg& msg)
	{
		if (win->IsShown()) {
			UpdateTree();
		}
	}

	void PrintWidgetName(ax::Window* w)
	{
		if (w == nullptr) {
			return;
		}

		if (!w->component.Has("Widget")) {
			return;
		}

		ax::widget::Component::Ptr w_comp = w->component.Get<ax::widget::Component>("Widget");
		ax::console::Print("Builder name :", w_comp->GetBuilderName());
	}

	void ProjectSpace::OnResize(const ax::Size& size)
	{
		ax::Rect sRect(size.w - 9, 0, 10, size.h);
		_scroll_bar->GetWindow()->dimension.SetRect(sRect);
		_panel->dimension.SetShownRect(ax::Rect(0, 0, size.w, size.h));
		_scroll_bar->UpdateWindowSize(_panel->dimension.GetSize());
	}

	void ProjectSpace::OnPaint(ax::GC gc)
	{
		const ax::Rect rect(win->dimension.GetDrawingRect());

		gc.SetColor(ax::Color(1.0));
		gc.DrawRectangle(rect);

		if (!_has_objects) {
			gc.SetColor(ax::Color(0.3));
			gc.DrawString(_font_bold, "No widget on grid.", ax::Point(15, 20));
		}

		gc.SetColor(ax::Color(0.7));
		gc.DrawRectangleContour(rect);
	}
}
}
