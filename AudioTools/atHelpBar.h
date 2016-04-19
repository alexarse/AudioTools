//
//  atHelpBar.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-18.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef atHelpBar_hpp
#define atHelpBar_hpp

#include <OpenAX/Button.h>
#include <OpenAX/Toggle.h>

namespace at {
namespace editor {
	void AttachHelpInfo(ax::Button* btn, const std::string& help_msg);
	void AttachHelpInfo(ax::Toggle* tog, const std::string& help_msg);
}
}

#endif /* atHelpBar_hpp */
