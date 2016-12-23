//
//  atHelpBar.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-18.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef atHelpBar_hpp
#define atHelpBar_hpp

#include <axlib/Button.hpp>
#include <axlib/Toggle.hpp>

namespace at {
namespace editor {
	void AttachHelpInfo(ax::Window* window, const std::string& help_msg);
}
}

#endif /* atHelpBar_hpp */
