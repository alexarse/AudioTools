//
//  PyoComponent.hpp
//  axFrameworks
//
//  Created by Alexandre Arsenault on 2016-03-03.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef PyoComponent_hpp
#define PyoComponent_hpp

#include "axUtils.h"
#include "axWindow.h"

namespace pyo {
class Component : public ax::Component {
public:
	/// Shared pointer.
	typedef std::shared_ptr<Component> Ptr;

	Component(ax::Window* win)
	{
		_win = win;
	}

	virtual ~Component()
	{
		
	}

	ax::Window* GetWindow()
	{
		return _win;
	}
	
	void SetFunctionName(const std::string& name)
	{
		_fct_name = name;
	}
	
	std::string GetFunctionName() const
	{
		return _fct_name;
	}

protected:
	ax::Window* _win;
	std::string _fct_name;
};
}

#endif /* PyoComponent_hpp */
