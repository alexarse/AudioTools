//
//  Pyo.hpp
//  axFrameworks
//
//  Created by Alexandre Arsenault on 2016-03-03.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef Pyo_hpp
#define Pyo_hpp

#include "m_pyo.h"
#include <string>

class Pyo {
public:
	Pyo(unsigned int sampling_rate, unsigned int buffer_size, unsigned int nchannels);

	int ProcessStatement(const std::string& script, bool debug = false);

private:
	PyThreadState* _pyo;
	float* _output;
	int _server_id;
	void (*_callback_fct)(int);
};

#endif /* Pyo_hpp */
