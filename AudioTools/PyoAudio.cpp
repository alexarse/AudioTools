#include "PyoAudio.h"
#include <OpenAX/Utils.h>

PyoAudio* PyoAudio::_global_audio = nullptr;

PyoAudio* PyoAudio::GetInstance()
{
	if (_global_audio == nullptr) {
		_global_audio = new PyoAudio();
	}

	return _global_audio;
}

PyoAudio::PyoAudio()
	: _connected_obj(nullptr)
{
	CreateServer(44100, 1024, 2);

	char msg[2048];
	int err = pyo_exec_file(_pyo, "scripts/default.py", msg, 1);

	if (err) {
		ax::Error("Load python script.");
	}
}

void PyoAudio::ReloadScript(const std::string& path)
{
	ax::Print("Debug reload");
	StopAudio();

	pyo_end_interpreter(_pyo);

	char msg[6000];
	CreateServer(44100, 1024, 2);

	pyo_exec_file(_pyo, path.c_str(), msg, 1);

	StartAudio();
	ax::Print("Done reset pyo server.");
}

void PyoAudio::CreateServer(float sr, int bufsize, int chnls)
{
	_pyo = pyo_new_interpreter(44100, 1024, 2);
	_server_id = pyo_get_server_id(_pyo);

	_output = (float*)(void*)pyo_get_output_buffer_address(_pyo);
	_callback_fct = (void (*)(int))(pyo_get_embedded_callback_address(_pyo));
}

void PyoAudio::ProcessString(const std::string& script)
{
	char msg[2048];
	sprintf(msg, script.c_str());
	err = pyo_exec_statement(_pyo, msg, 1);
}

bool PyoAudio::IsServerStarted()
{
	return (bool)pyo_is_server_started(_pyo);
}

int PyoAudio::CoreCallbackAudio(const float* input, float* output, unsigned long frameCount)
{
	_callback_fct(_server_id);

	float* pyo_buffer = _output;

	double rms_left = 0.0;
	double rms_right = 0.0;

	for (int i = 0; i < frameCount; i++) {
		rms_left += pow(*pyo_buffer, 2);
		*output++ = *pyo_buffer++;
		
		rms_right += pow(*pyo_buffer, 2);
		*output++ = *pyo_buffer++;
	}

	rms_left /= double(frameCount);
	rms_right /= double(frameCount);

	rms_left = sqrt(rms_left);
	rms_right = sqrt(rms_right);

//	if (_connected_obj) {
//		_connected_obj->PushEvent(89831, new ax::Event::SimpleMsg<std::pair<double, double>>(
//											 std::pair<double, double>(rms_left, rms_right)));
//	}

	return 0;
}