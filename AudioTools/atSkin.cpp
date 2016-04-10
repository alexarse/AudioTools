//
//  atSkin.cpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-08.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#include "atSkin.hpp"

namespace at {

Skin* Skin::_global_skin = nullptr;

}
//PyoAudio* PyoAudio::_global_audio = nullptr;
//
//PyoAudio* PyoAudio::GetInstance()
//{
//	if (_global_audio == nullptr) {
//		_global_audio = new PyoAudio();
//	}
//	
//	return _global_audio;
//}