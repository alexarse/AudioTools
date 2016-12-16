#include "python/Pyo.hpp"

Pyo::Pyo(unsigned int sampling_rate, unsigned int buffer_size, unsigned int nchannels)
{
	_pyo = pyo_new_interpreter(sampling_rate, buffer_size, nchannels);
	_server_id = pyo_get_server_id(_pyo);

	_output = (float*)(void*)pyo_get_output_buffer_address(_pyo);
	_callback_fct = (void (*)(int))(pyo_get_embedded_callback_address(_pyo));
}

int Pyo::ProcessStatement(const std::string& script, bool debug)
{
	return 0;
}
