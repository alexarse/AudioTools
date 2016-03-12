///*
// * Copyright (c) 2013 Alexandre Arsenault.
// *
// * This file is part of axFrameworks.
// *
// * axFrameworks is free or commercial software: you can redistribute it and/or
// * modify it under the terms of the GNU General Public License as published by
// * the Free Software Foundation, either version 3 or any later version of the
// * License or use a commercial axFrameworks License.
// *
// * axFrameworks is distributed in the hope that it will be useful,
// * but WITHOUT ANY WARRANTY; without even the implied warranty of
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// * GNU General Public License for more details.
// *
// * You should have received a copy of the GNU General Public License
// * along with axFrameworks. If not, see <http://www.gnu.org/licenses/>.
// *
// * To release a closed-source product which uses axFrameworks, commercial
// * licenses are available, email alx.arsenault@gmail.com for more information.
// */
//#include "axEditorSelector.h"
//
//#include "axLabel.h"
//#include "axTextBox.h"
//#include "axEditorMenu.h"
//#include "axResourceStorage.h"
//
//#include "axEditor.h"
//
//ax::Editor::Selector::Selector(ax::Window* parent) :
//// Heritage.
//axPanel(parent, ax::Rect(ax::Point(parent->GetSize().x - 8, parent->GetSize().y - 8), ax::Size(8, 8))),
//// Members.
////_events(events),
//_info(ax::Color(1.0, 0.0, 0.0, 1.0),
//      ax::Color(1.0, 0.0, 0.0, 1.0),
//      ax::Color(0.85, 0.0, 0.0, 1.0),
//      ax::Color(0.9, 0.0, 0.0, 1.0),
//      ax::Color(0.0, 0.0, 0.0, 1.0),
//      ax::Color(0.0, 0.0, 0.0, 1.0)),
//_label(""),
//_flags(ax::Button::Flags::SINGLE_IMG),
//_selected(false),
//_msg(""),
////_isEditing(false),
//_font(nullptr)
//{
//    _currentColor = &_info.normal;
//    _font = std::unique_ptr<ax::Font>(new ax::Font(0));
//    
//    AddProperty("EditingWidget");
//    
//}
//
//void ax::Editor::Selector::SetMsg(const std::string& msg)
//{
//    _msg = msg;
//}
//
//void ax::Editor::Selector::SetSelected(const bool& selected)
//{
//    if (_selected != selected)
//    {
//        _selected = selected;
//        
//        if (_selected)
//        {
//            if (_currentColor == &_info.normal)
//            {
//                _currentColor = &_info.selected;
//                Update();
//            }
//        }
//        else
//        {
//            if (_currentColor == &_info.selected)
//            {
//                _currentColor = &_info.normal;
//                Update();
//            }
//        }
//    }
//}
//
//void ax::Editor::Selector::SetLabel(const std::string& label)
//{
//    _label = label;
//    Update();
//}
//
//void ax::Editor::Selector::OnMouseLeftDown(const ax::Point& pos)
//{
//    _currentColor = &_info.clicking;
//    
//    axWidget* widget = static_cast<axWidget*>(GetParent());
//    
//    // Prevent from click on the hidden edit button of editing widgets.
//    if(!widget->HasProperty("EditingWidget") && widget->HasProperty("Editable"))
//    {
//        ax::Editor::Interface* editor = GetApp()->GetEditorInterface();
//        
//        if(editor)
//        {
//            editor->SetEditingWidget(widget);
//        }
//
//        GrabMouse();
//        
//        PushEvent(ax::Button::Events::BUTTON_CLICK, new ax::Button::Msg(nullptr, _msg));
//        
//        Update();
//    }
//}
//
//void ax::Editor::Selector::OnMouseLeftDragging(const ax::Point &pos)
//{
//    ax::Window* panel = GetParent()->GetParent();
//    ax::Rect panelRect = panel->GetAbsoluteRect();
//    
//    ax::Point position(pos);
//    position = position - panelRect.position;
//    position.x = position.x - GetParent()->GetSize().x + 5;
//    position.y = position.y - GetParent()->GetSize().y + 5;
//
//
//    position.x = ax::Utils::Clamp<int>(position.x, 0,
//                              panelRect.size.x - GetParent()->GetSize().x);
//    
//    position.y = ax::Utils::Clamp<int>(position.y, 0,
//                              panelRect.size.y - GetParent()->GetSize().y);
//    
//    
//    GetParent()->SetPosition(position);
//    
//    ax::Editor::Interface* editor = GetApp()->GetEditorInterface();
//    
//    if(editor)
//    {
//        editor->UpdateWidgetInfo();
//    }
//}
//
//void ax::Editor::Selector::OnMouseLeftUp(const ax::Point& pos)
//{
//    if (IsGrabbed())
//    {
//        UnGrabMouse();
//        
//        if (IsMouseHoverWindow())
//        {
//            _currentColor = &_info.hover;
//            _selected = true;
//        }
//        else
//        {
//            if (_selected)
//            {
//                _currentColor = &_info.selected;
//            }
//            else
//            {
//                _currentColor = &_info.normal;
//            }
//        }
//        
//        Update();
//    }
//}
//
//void ax::Editor::Selector::OnMouseRightDown(const ax::Point& pos)
//{
//
//}
//
//void ax::Editor::Selector::OnAttributeEdit(const axTextBox::Msg& msg)
//{
//    axWidget* widget = static_cast<axWidget*>(GetParent());
//    
//    ax::StringPairVector attributes;
//    
//    for(auto& n : _infoEditor)
//    {
//        std::string name = std::get<0>(n);
//        std::string value = std::get<2>(n)->GetLabel();
//        attributes.push_back(ax::StringPair(name, value));
//    }
//    
//    widget->SetInfo(attributes);
//}
//
//void ax::Editor::Selector::OnKeyDown(const char& key)
//{
//    ax::Print(key);
//}
//
//void ax::Editor::Selector::OnMouseEnter()
//{
//    if (!IsGrabbed())
//    {
//        GrabKey();
//        _currentColor = &_info.hover;
//        Update();
//    }
//}
//
//void ax::Editor::Selector::OnMouseLeave()
//{
//    if (!IsGrabbed())
//    {
//        UnGrabKey();
//        if (_selected)
//        {
//            _currentColor = &_info.selected;
//        }
//        else
//        {
//            _currentColor = &_info.normal;
//        }
//    }
//    
//    Update();
//}
//
//void ax::Editor::Selector::OnPaint()
//{
//    ax::GC gc;
//    ax::Rect rect(GetDrawingRect());
//    
//    gc.SetColor(*_currentColor);
//    gc.DrawRectangle(rect);
//    
//    gc.SetColor(_info.contour);
//    gc.DrawRectangleContour(rect);
//}
//
//
//
//>>>>>>> 89b7fc250b4dfc9c4cb88a1790a8a9c118d418c0
