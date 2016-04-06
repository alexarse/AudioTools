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
	, _rms_values(0.0, 0.0)
	, _rms_count(0)
{
	CreateServer(44100, 1024, 2);

	char msg[2048];
	int err = pyo_exec_file(_pyo, "scripts/default.py", msg, 1);

	if (err) {
		ax::Error("Load python script.");
	}
}

PyoAudio::~PyoAudio()
{
	StopAudio();
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
	_rms_values.first = rms_left;
	_rms_values.second = rms_right;

	_rms_count += frameCount;

	if (_rms_count >= 5000 && _connected_obj) {
		_connected_obj->PushEvent(
			Events::RMS_VALUE_CHANGE, new ax::Event::SimpleMsg<std::pair<double, double>>(_rms_values));

		_rms_count = 0;
	}

	return 0;
}