//
//  Audio.hpp
//  axFrameworks
//
//  Created by Alexandre Arsenault on 2016-03-03.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef Audio_hpp
#define Audio_hpp

#include <OpenAX/OpenAX.h>
#include <OpenAX/axAudio.h>
#include "m_pyo.h"

class PyoAudio : public ax::audio::Core {
public:
	static PyoAudio* GetInstance();
	
	~PyoAudio();
	
	enum Events : ax::Event::Id { RMS_VALUE_CHANGE = 89831 };

	void ProcessString(const std::string& script);
	bool IsServerStarted();
	
	void ProcessMidi(int status, int byte1, int byte2)
	{
		pyo_add_midi_event(_pyo, status, byte1, byte2);
	}
	
	void ReloadScript(const std::string& path);
	
	void SetConnectedObject(ax::Event::Object* obj)
	{
		_connected_obj = obj;
	}
	
protected:
	static PyoAudio* _global_audio;

	PyoAudio();

	virtual int CoreCallbackAudio(
		const float* input, float* output, unsigned long frameCount);
	
	void CreateServer(float sr, int bufsize, int chnls);

private:
	ax::Event::Object* _connected_obj;
	PyThreadState* _pyo;
	int _rms_count;
	std::pair<double, double> _rms_values;
	float* _output;
	int _server_id;
	void (*_callback_fct)(int);
};

#endif /* Audio_hpp */
