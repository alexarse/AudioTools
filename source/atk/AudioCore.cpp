#include "atk/AudioCore.hpp"
#include <axlib/Util.hpp>
#include <iostream>

namespace atk {
AudioCore::AudioCore()
{
}

AudioCore::~AudioCore()
{
	StopAudio();
	Pa_Terminate();

	delete[] _output_buffer[0];
	delete[] _output_buffer[1];
	delete[] _output_buffer;
}

int AudioCore::InitAudio()
{
	err = Pa_Initialize();

	if (err != paNoError) {
		std::cerr << "Error." << std::endl;
		exit(1);
	}

	if (Pa_GetDeviceCount() <= 0) {
		ax::console::Error("No available audio device.");
		return -1;
	}

	//----------------------------------------------------------------------
	//----------------------------------------------------------------------
	//		outputParameters.device = 1; // Pa_GetDefaultOutputDevice();
	//----------------------------------------------------------------------
	//----------------------------------------------------------------------

	//		if (outputParameters.device == paNoDevice) {
	//			std::cerr << "Error." << std::endl;
	//			exit(1);
	//		}

	// Stereo output.
	//		outputParameters.channelCount = 2;

	// 32 bit floating point output.
	//		outputParameters.sampleFormat = paFloat32;

	//	outputParameters.suggestedLatency = Pa_GetDeviceInfo(
	// outputParameters.device )->defaultLowOutputLatency;
	//		outputParameters.hostApiSpecificStreamInfo = NULL;

	// Ouput parameters.
	_outputParameters.channelCount = 2;
	_outputParameters.device = 1;
	_outputParameters.hostApiSpecificStreamInfo = NULL;
	_outputParameters.sampleFormat = paFloat32;
	_outputParameters.suggestedLatency = Pa_GetDeviceInfo(_outputParameters.device)->defaultLowInputLatency;

	// Input parameters.
	_inputParameters.channelCount = 2;
	_inputParameters.device = Pa_GetDefaultInputDevice();
	_inputParameters.hostApiSpecificStreamInfo = NULL;
	_inputParameters.sampleFormat = paFloat32;
	_inputParameters.suggestedLatency = Pa_GetDeviceInfo(Pa_GetDefaultInputDevice())->defaultLowInputLatency;

	err = Pa_OpenStream(&stream, &_inputParameters, &_outputParameters,
		44100, // Sample rate.
		1024, // Buffer size.
		paClipOff, // No cliping.
		myPaCallback, this);

	if (err != paNoError) {
		std::cerr << "Error." << std::endl;
		exit(1);
	}

	_output_buffer = new double*[2];
	_output_buffer[0] = new double[1024];
	_output_buffer[1] = new double[1024];

	return 0;
}

void AudioCore::SetCurrentOutputDevice(const std::string& name)
{
	int numDevices = Pa_GetDeviceCount();

	if (numDevices <= 0) {
		ax::console::Error("No available audio device.");
		return;
	}

	int index = -1;
	for (int i = 0; i < numDevices; i++) {
		const PaDeviceInfo* device_info = Pa_GetDeviceInfo(i);
		if (device_info->maxOutputChannels != 0) {
			if (std::string(device_info->name) == name) {
				index = i;
				break;
			}
		}
	}

	if (index != -1) {
		// Strop stream.
		Pa_StopStream(stream);

		// Set new output device.
		_outputParameters.device = index;

		PaError err = Pa_OpenStream(&stream,
			&_inputParameters, // No input.
			&_outputParameters,
			44100, // Sample rate.
			1024, // Buffer size.
			paClipOff, // No cliping.
			myPaCallback, this);

		if (err != paNoError) {
			ax::console::Error("Poraudio error opening stream.");
			return;
		}

		err = Pa_StartStream(stream);

		if (err != paNoError) {
			ax::console::Error("Poraudio error starting stream.");
		}
	}
}

void AudioCore::SetCurrentInputDevice(const std::string& name)
{
	int numDevices = Pa_GetDeviceCount();

	if (numDevices <= 0) {
		ax::console::Error("No available audio device.");
		return;
	}

	int index = -1;
	for (int i = 0; i < numDevices; i++) {
		const PaDeviceInfo* device_info = Pa_GetDeviceInfo(i);
		if (device_info->maxInputChannels != 0) {
			if (std::string(device_info->name) == name) {
				index = i;
				break;
			}
		}
	}

	if (index != -1) {
		// Strop stream.
		Pa_StopStream(stream);

		// Set new output device.
		_inputParameters.device = index;

		PaError err = Pa_OpenStream(&stream,
			&_inputParameters, // No input.
			&_outputParameters,
			44100, // Sample rate.
			1024, // Buffer size.
			paClipOff, // No cliping.
			myPaCallback, this);

		if (err != paNoError) {
			ax::console::Error("Poraudio error opening stream.");
			return;
		}

		err = Pa_StartStream(stream);

		if (err != paNoError) {
			ax::console::Error("Poraudio error starting stream.");
		}
	}
}

std::vector<std::string> AudioCore::GetInputDevices()
{
	int numDevices = Pa_GetDeviceCount();

	if (numDevices <= 0) {
		ax::console::Error("No available audio device.");
		return std::vector<std::string>();
	}

	std::vector<std::string> devices;

	for (int i = 0; i < numDevices; i++) {
		const PaDeviceInfo* device_info = Pa_GetDeviceInfo(i);
		if (device_info->maxInputChannels != 0) {
			devices.push_back(device_info->name);
		}
	}

	return devices;
}

std::vector<std::string> AudioCore::GetOutputDevices()
{
	int numDevices = Pa_GetDeviceCount();

	if (numDevices <= 0) {
		ax::console::Error("No available audio device.");
		return std::vector<std::string>();
	}

	std::vector<std::string> devices;

	for (int i = 0; i < numDevices; i++) {
		const PaDeviceInfo* device_info = Pa_GetDeviceInfo(i);
		if (device_info->maxOutputChannels != 0) {
			devices.push_back(device_info->name);
		}
	}

	return devices;
}

std::string AudioCore::GetCurrentInputDevice()
{
	int numDevices = Pa_GetDeviceCount();

	if (numDevices <= 0) {
		ax::console::Error("No available audio device.");
		return "None";
	}

	for (int i = 0; i < numDevices; i++) {
		const PaDeviceInfo* device_info = Pa_GetDeviceInfo(i);
		if (i == _inputParameters.device) {
			return std::string(device_info->name);
		}
	}

	return "None";
}

std::string AudioCore::GetCurrentOutputDevice()
{
	int numDevices = Pa_GetDeviceCount();

	if (numDevices <= 0) {
		ax::console::Error("No available audio device.");
		return "None";
	}

	for (int i = 0; i < numDevices; i++) {
		const PaDeviceInfo* device_info = Pa_GetDeviceInfo(i);
		if (i == _outputParameters.device) {
			return std::string(device_info->name);
		}
	}

	return "None";
}

void AudioCore::StartAudio()
{
	err = Pa_StartStream(stream);

	if (err != paNoError) {
		std::cerr << "Error openning audio." << std::endl;
		exit(1);
	}
}

void AudioCore::StopAudio()
{
	err = Pa_StopStream(stream);

	if (err == paNoError) {
		return;
	}

	if (err == paStreamIsStopped) {
		std::cerr << "Audio is already stoped." << std::endl;
		return;
	}

	std::cerr << "Error stoping audio." << std::endl;
}
} // atk.
