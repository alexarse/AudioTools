//#include "axEditorAttributes.h"
//#include "axScrollBar.h"
//
//ax::Editor::Attributes::Attributes(ax::Window* parent, const ax::Rect& rect) :
//axPanel(parent, rect),
//_font(0),
//_widget(nullptr)
//{
//    _font.SetFontSize(14);
//    //    SetBlockDrawing(true);
//    property.AddProperty("BlockDrawing");
//}
//
//void ax::Editor::Attributes::SetEditingWidget(axWidget* widget)
//{
//    _widget = widget;
//    
//    ax::TextBox::Events txtEvents;
//    txtEvents.enter_click = GetOnAttributeEdit();
//    
//    ax::TextBox::Info txtInfo;
//    txtInfo.normal = ax::Color(1.0, 1.0, 1.0, 1.0);
//    txtInfo.hover = ax::Color(1.0, 1.0, 1.0, 1.0);
//    txtInfo.selected = ax::Color(1.0, 1.0, 1.0, 1.0);
//    txtInfo.highlight = ax::Color(0.4, 0.4, 0.6, 1.0);
//    txtInfo.contour = ax::Color(0.0, 0.0, 0.0, 1.0);
//    txtInfo.cursor = ax::Color(1.0, 0.0, 0.0, 1.0);
//    txtInfo.selected_shadow = ax::Color(0.8, 0.8, 0.8, 0.2);
//    
//    ax::Label::Info labelInfo;
//    labelInfo.normal = ax::Color(0.8, 0.8, 0.8, 1.0);
//    labelInfo.contour = ax::Color(0.0, 0.0, 0.0, 1.0);
//    labelInfo.font_color = ax::Color(0.0, 0.0, 0.0, 1.0);
//    labelInfo.font_size = 12;
//    labelInfo._alignement = ax::Utils::Alignement::axALIGN_CENTER;
//    
//    
//    ax::Label::Info labelInfoWindow;
//    labelInfoWindow.normal = ax::Color(0.4, 0.4, 0.4, 1.0);
//    labelInfoWindow.contour = ax::Color(0.0, 0.0, 0.0, 1.0);
//    labelInfoWindow.font_color = ax::Color(0.8, 0.8, 0.8, 1.0);
//    labelInfoWindow.font_size = 12;
//    labelInfoWindow._alignement = ax::Utils::Alignement::axALIGN_CENTER;
//    
//    ax::StringVector attributes = widget->GetInfo()->GetParamNameList();
//    axInfo* info = widget->GetInfo();
//    
//    
//    for(int i = 0; i < _infoEditor.size(); i++)
//    {
//        //        GetApp()->GetCore()->GetPopupManager()->GetWindowTree()->
//        //        DeleteWindow( std::get<1>(_infoEditor[i]) );
//        std::get<1>(_infoEditor[i])->DeleteWindow();
//        
//        if(std::get<2>(_infoEditor[i]) != nullptr)
//        {
//            //            GetApp()->GetCore()->GetPopupManager()->GetWindowTree()->
//            //            DeleteWindow( std::get<2>(_infoEditor[i]) );
//            std::get<2>(_infoEditor[i])->DeleteWindow();
//        }
//    }
//    _infoEditor.clear();
//    
//    AddEditorLabel("Runtime Info", labelInfoWindow);
//    AddEditorAttribute(ax::StringPair("object id",
//                                      std::to_string(_widget->GetId())),
//                       labelInfo, txtInfo, ax::TextBox::Events());
//    
//    ax::Rect rel_rect(_widget->dimension.GetRect());
//    std::string relative_rect = std::to_string(rel_rect.position.x) + ", " +
//    std::to_string(rel_rect.position.y) + ", " +
//    std::to_string(rel_rect.size.x) + ", " +
//    std::to_string(rel_rect.size.y);
//    
//    AddEditorAttribute(ax::StringPair("relative rectangle", relative_rect),
//                       labelInfo, txtInfo, ax::TextBox::Events());
//    
//    ax::Rect abs_rect(_widget->dimension.GetAbsoluteRect());
//    std::string absolute_rect = std::to_string(abs_rect.position.x) + ", " +
//    std::to_string(abs_rect.position.y) + ", " +
//    std::to_string(abs_rect.size.x) + ", " +
//    std::to_string(abs_rect.size.y);
//    
//    AddEditorAttribute(ax::StringPair("absolute rectangle", absolute_rect),
//                       labelInfo, txtInfo, ax::TextBox::Events());
//    
//    AddEditorLabel("Builder Info", labelInfoWindow);
//    //    axStringVector builder_list = widget->GetParamNameList();
//    ax::StringPairVector builder_attributes = widget->GetBuilderAttributes();
//    
//    for(auto& n : builder_attributes)
//    {
//        if(n.first == "rect")
//        {
//            n.first = "start rectangle";
//        }
//        AddEditorAttribute(n, labelInfo, txtInfo, ax::TextBox::Events());
//    }
//    
//    AddEditorLabel("Widget Info", labelInfoWindow);
//    
//    AddEditorAttribute(ax::StringPair("info path", info->GetPath()),
//                       labelInfo, txtInfo, ax::TextBox::Events());
//    
//    for(auto& n : attributes)
//    {
//        AddEditorAttribute(ax::StringPair(n, info->GetAttributeValue(n)),
//                           labelInfo, txtInfo, txtEvents);
//    }
//    
//    ax::Rect lastRect = std::get<1>(_infoEditor[_infoEditor.size()-1])->dimension.GetRect();
//    ax::Size newSize(dimension.GetRect().size.x, lastRect.position.y + lastRect.size.y);
//    dimension.SetSizeNoShowRect(newSize);
//    
//    axScrollBar* scrollBar = GetScrollBar();
//    if(scrollBar != nullptr)
//    {
//        scrollBar->SetPanelSize(newSize);
//    }
//    
//    Update();
//}
//
//void ax::Editor::Attributes::AddEditorAttribute(const ax::StringPair& att,
//                                                const ax::Label::Info& labelInfo,
//                                                const ax::TextBox::Info& txtInfo,
//                                                const ax::TextBox::Events& evt)
//{
//    ax::Point pos(0, 0);
//    
//    if(_infoEditor.size())
//    {
//        pos = std::get<1>(_infoEditor[_infoEditor.size() - 1])->dimension.GetRect().GetNextPosDown(0);
//    }
//    
//    ax::Label* label = new ax::Label(this, ax::Rect(pos, ax::Size(120, 25)),
//                                     labelInfo, att.first);
//    
//    ax::TextBox* txtBox = new ax::TextBox(this, ax::Rect(label->dimension.GetRect().GetNextPosRight(0),
//                                                         ax::Size(180, 25)),
//                                          evt, txtInfo, "",
//                                          att.second);
//    
//    _infoEditor.push_back(axEditorTuple(att.first, label, txtBox));
//    
//    
//    label->property.AddProperty("EditingWidget");
//    txtBox->property.AddProperty("EditingWidget");
//    //    label->SetEditingWidget(true);
//    //    txtBox->SetEditingWidget(true);
//    
//    
//    //    label->SetEditable(false);
//    //    txtBox->SetEditable(false);
//}
//
//void ax::Editor::Attributes::AddEditorLabel(const std::string& name,
//                                            const ax::Label::Info& labelInfo)
//{
//    ax::Point pos(0, 00);
//    
//    if(_infoEditor.size())
//    {
//        pos = std::get<1>(_infoEditor[_infoEditor.size() - 1])->dimension.GetRect().GetNextPosDown(0);
//    }
//    
//    ax::Label* label = new ax::Label(this, ax::Rect(pos, ax::Size(300, 25)), labelInfo, name);
//    
//    _infoEditor.push_back(axEditorTuple(name, label, nullptr));
//    
//    label->property.AddProperty("EditingWidget");
//    //    label->SetEditingWidget(true);
//    //    label->SetEditable(false);
//}
//
//void ax::Editor::Attributes::OnAttributeEdit(const ax::TextBox::Msg& msg)
//{
//    if(_widget != nullptr)
//    {
//        ax::StringPairVector attributes;
//        
//        for(auto& n : _infoEditor)
//        {
//            if(std::get<2>(n) != nullptr)
//            {
//                std::string name = std::get<0>(n);
//                std::string value = std::get<2>(n)->GetLabel();
//                attributes.push_back(ax::StringPair(name, value));
//            }
//        }
//        
//        _widget->SetInfo(attributes);
//    }
//}
//
//void ax::Editor::Attributes::UpdateAttribute(const ax::StringPair& att)
//{
//    for(auto& n : _infoEditor)
//    {
//        if(std::get<0>(n) == att.first &&
//           std::get<2>(n)->GetLabel() != att.second)
//        {
//            std::get<2>(n)->SetLabel(att.second);
//            break;
//        }
//    }
//}
//
//void ax::Editor::Attributes::UpdateInfo()
//{
//    ax::StringVector attributes = _widget->GetInfo()->GetParamNameList();
//    axInfo* info = _widget->GetInfo();
//    
//    UpdateAttribute(ax::StringPair("object id", std::to_string(_widget->GetId())));
//    
//    ax::Rect rel_rect(_widget->dimension.GetRect());
//    std::string relative_rect = std::to_string(rel_rect.position.x) + ", " +
//    std::to_string(rel_rect.position.y) + ", " +
//    std::to_string(rel_rect.size.x) + ", " +
//    std::to_string(rel_rect.size.y);
//    
//    UpdateAttribute(ax::StringPair("relative rectangle", relative_rect));
//    
//    
//    ax::Rect abs_rect(_widget->dimension.GetAbsoluteRect());
//    std::string absolute_rect = std::to_string(abs_rect.position.x) + ", " +
//    std::to_string(abs_rect.position.y) + ", " +
//    std::to_string(abs_rect.size.x) + ", " +
//    std::to_string(abs_rect.size.y);
//    
//    UpdateAttribute(ax::StringPair("absolute rectangle", absolute_rect));
//    
//    
//    for(auto& n : attributes)
//    {
//        UpdateAttribute(ax::StringPair(n, info->GetAttributeValue(n)));
//    }
//}
//
//void ax::Editor::Attributes::OnPaint()
//{
//    ax::GC gc;
//    ax::Rect rect(dimension.GetDrawingRect());
//    
//    gc.DrawRectangleColorFade(rect, ax::Color(0.5), ax::Color(0.6));
//}