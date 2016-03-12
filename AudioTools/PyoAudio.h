//
//  Audio.hpp
//  axFrameworks
//
//  Created by Alexandre Arsenault on 2016-03-03.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef Audio_hpp
#define Audio_hpp

#include "axAudio.h"
//#include "axPython.h"
#include "m_pyo.h"

class PyoAudio : public ax::audio::Core {
public:
	static PyoAudio* GetInstance();

	void ProcessString(const std::string& script);
	bool IsServerStarted();
	
	void ProcessMidi(int status, int byte1, int byte2)
	{
		pyo_add_midi_event(_pyo, status, byte1, byte2);
	}
	
protected:
	static PyoAudio* _global_audio;

	PyoAudio();

	virtual int CoreCallbackAudio(
		const float* input, float* output, unsigned long frameCount);

private:
	PyThreadState* _pyo;
	float* _output;
	int _server_id;
	void (*_callback_fct)(int);
};

#endif /* Audio_hpp */
