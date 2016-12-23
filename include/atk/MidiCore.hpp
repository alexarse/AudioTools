#pragma once

#include <axlib/Util.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

//#define __MACOSX_CORE__
#include "atk/RtMidi.hpp"

namespace atk {
class MidiNote {
public:
	MidiNote(const int& note, const int& velocity)
		: _note(note)
		, _velocity(velocity)
	{
	}

	int GetNote() const
	{
		return _note;
	}

	int GetVelocity() const
	{
		return _velocity;
	}

private:
	int _note, _velocity;
};

class MidiCore {
public:
	MidiCore();
	~MidiCore();

	std::vector<std::string> GetMidiInputList();

	std::string GetCurrentPortName() const;

	void SetInputPort(const std::string& port_name);

protected:
	virtual void OnMidiNoteOn(const MidiNote& msg);
	virtual void OnMidiNoteOff(const MidiNote& msg);

private:
	RtMidiIn* _midiInHandle;
	int _input_port;

	static void MidiCallBack(double deltatime, std::vector<unsigned char>* message, void* userData);
};
} // atk.
