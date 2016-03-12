////
////  axEditorWidgets.cpp
////  axLib
////
////  Created by Alexandre Arsenault on 2015-04-01.
////  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
////
//
//#include "axEditorWidgetPanel.h"
//#include "axEditorWidget.h"
//
//ax::Editor::WidgetPanel::WidgetPanel(ax::Window* parent, const ax::Rect& rect):
//axPanel(parent, rect)
//{
////    SetBlockDrawing(true);
//    
//    ax::Size btnSize(rect.size.x, 40);
//    ax::Editor::Widget* btn1 = new ax::Editor::Widget(this,
//                                                      ax::Rect(ax::Point(0, 0), btnSize),
//                                                      "Button");
//    
//    ax::Editor::Widget* btn2 = new ax::Editor::Widget(this,
//                                                      ax::Rect(btn1->dimension.GetRect().GetNextPosDown(0), btnSize),
//                                                      "Toggle");
//    
//    ax::Editor::Widget* btn3 = new ax::Editor::Widget(this,
//                                                      ax::Rect(btn2->dimension.GetRect().GetNextPosDown(0), btnSize),
//                                                      "Number");
//    
//    ax::Editor::Widget* btn4 = new ax::Editor::Widget(this,
//                                                      ax::Rect(btn3->dimension.GetRect().GetNextPosDown(0), btnSize),
//                                                      "Knob");
//    
//    ax::Editor::Widget* btn5 = new ax::Editor::Widget(this,
//                                                      ax::Rect(btn4->dimension.GetRect().GetNextPosDown(0), btnSize),
//                                                      "Slider");
//
////    btn1->SetEditingWidget(true);
//    btn1->property.AddProperty("EditingWidget");
////    btn1->SetEditable(false);
//    
////    btn2->SetEditingWidget(true);
//    btn2->property.AddProperty("EditingWidget");
////    btn2->SetEditable(false);
//    
////    btn3->SetEditingWidget(true);
//    btn3->property.AddProperty("EditingWidget");
////    btn3->SetEditable(false);
//    
////    btn4->SetEditingWidget(true);
//    btn4->property.AddProperty("EditingWidget");
////    btn4->SetEditable(false);
//    
////    btn5->SetEditingWidget(true);
//    btn5->property.AddProperty("EditingWidget");
////    btn5->SetEditable(false);
//}
//
//void ax::Editor::WidgetPanel::OnPaint()
//{
////    axGC gc(this);
//    ax::GC gc;
//    ax::Rect rect(dimension.GetDrawingRect());
//    
////    gc.DrawRectangleColorFade(rect, ax::Color(0.8), ax::Color(0.7));
////    
////    gc.SetColor(ax::Color(0.0));
//    gc.DrawRectangleContour(rect);
//    //    gc.DrawRectangleColorFade(ax::Rect(0, 0, rect.size.x, 40),
//    //                              axColor(0.6), axColor(0.9));
//}