#include "PyoAudio.h"
#include "axUtils.h"

PyoAudio* PyoAudio::_global_audio = nullptr;

PyoAudio* PyoAudio::GetInstance()
{
	if (_global_audio == nullptr) {
		_global_audio = new PyoAudio();
	}

	return _global_audio;
}

PyoAudio::PyoAudio()
{
	CreateServer(44100, 1024, 2);

	char msg[2048];
	pyo_exec_file(_pyo, "scripts/simple_saw.py", msg, 1);
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

int PyoAudio::CoreCallbackAudio(
	const float* input, float* output, unsigned long frameCount)
{
	_callback_fct(_server_id);
//
	float* pyo_buffer = _output;

	for (int i = 0; i < frameCount; i++) {
//		*output++ = 0.0f;
//		*output++ = 0.0f;
		*output++ = *pyo_buffer++;
		*output++ = *pyo_buffer++;
	}
	return 0;
}