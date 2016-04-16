//
//  atConsoleStream.cpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-15.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#include "atConsoleStream.h"

namespace at {

std::unique_ptr<ConsoleStream> ConsoleStream::_instance = nullptr;

ConsoleStream::ConsoleStream()
	: ax::Event::Object(ax::App::GetInstance().GetEventManager())
{
}

void ConsoleStream::Write()
{
	_stream << std::endl;
	PushEvent(WRITE_NEW_LINE, new ax::Event::SimpleMsg<int>(0));
}

void ConsoleStream::Error()
{
	_stream << std::endl;
	PushEvent(WRITE_ERROR, new ax::Event::SimpleMsg<int>(0));
}
}