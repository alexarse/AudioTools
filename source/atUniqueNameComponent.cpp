//
//  atUniqueNameComponent.cpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-15.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#include "atUniqueNameComponent.h"

namespace at {
UniqueNameComponent::UniqueNameComponent(ax::Window* win)
{
	_win = win;
}

UniqueNameComponent::UniqueNameComponent(ax::Window* win, const std::string& name)
	: _win(win)
	, _name(name)
{
}

UniqueNameComponent::~UniqueNameComponent()
{
}

ax::Window* UniqueNameComponent::GetWindow()
{
	return _win;
}

void UniqueNameComponent::SetName(const std::string& name)
{
	_name = name;
}

std::string UniqueNameComponent::GetName() const
{
	return _name;
}
}
