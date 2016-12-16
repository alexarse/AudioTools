#include "atk/MidiCore.hpp"

namespace atk {
MidiCore::MidiCore()
{
	_midiInHandle = new RtMidiIn();
	_input_port = 0;
	// Check available ports.
	unsigned int nPorts = _midiInHandle->getPortCount();

	if (nPorts == 0) {
		std::cout << "No ports available!\n";

		/// @todo Trhow exception.
	}
	for (int i = 0; i < nPorts; i++) {
		//                std::cout << _midiInHandle->RtMidi::getPortName()
		//                << std::endl;
		std::cout << _midiInHandle->getPortName() << std::endl;
	}

	// for(int i = 0; i < nPorts; i++)
	// {
	//     _midiInHandle->openPort(i);
	// }
	_midiInHandle->openPort(_input_port);
	//            _midiInHandle->openPort( 0 );

	// Set our callback function.  This should be done immediately after
	// opening the port to avoid having incoming messages written to the
	// queue.
	_midiInHandle->setCallback(&MidiCallBack, (void*)this);

	// Don't ignore sysex, timing, or active sensing messages.
	_midiInHandle->ignoreTypes(false, false, false);

	// cleanup:
	//    delete midiin;
}

std::vector<std::string> MidiCore::GetMidiInputList()
{
	std::vector<std::string> list;

	unsigned int nPorts = _midiInHandle->getPortCount();

	for (int i = 0; i < nPorts; i++) {
		list.push_back(_midiInHandle->getPortName());
	}

	return list;
}

std::string MidiCore::GetCurrentPortName() const
{
	return _midiInHandle->getPortName(_input_port);
}

void MidiCore::SetInputPort(const std::string& port_name)
{
	unsigned int nPorts = _midiInHandle->getPortCount();

	for (int i = 0; i < nPorts; i++) {
		if (port_name == _midiInHandle->getPortName()) {
			if (_midiInHandle->isPortOpen()) {
				_midiInHandle->closePort();
			}

			_midiInHandle->openPort(i);
			_input_port = i;
			break;
		}
	}
}

MidiCore::~MidiCore()
{
}

void MidiCore::OnMidiNoteOn(const MidiNote& msg)
{
}

void MidiCore::OnMidiNoteOff(const MidiNote& msg)
{
}

void MidiCore::MidiCallBack(double deltatime, std::vector<unsigned char>* message, void* userData)
{
	unsigned int nBytes = (unsigned int)message->size();

	if (nBytes >= 3) {
		MidiNote midiMsg(message->at(1), message->at(2));
		MidiCore* midi = static_cast<MidiCore*>(userData);

		if (message->at(0) == 144) {
			midi->OnMidiNoteOn(midiMsg);
			//            std::cout << "Note on : " << (int)message->at(1)
			//            << " velocity : " << (int)message->at(2) <<
			//            std::endl;
		}
		else if (message->at(0)) {
			midi->OnMidiNoteOff(midiMsg);
			//            std::cout << "Note off : " << (int)message->at(1)
			//            << " velocity : " << (int)message->at(2) <<
			//            std::endl;
		}
	}
}
} // atk.
