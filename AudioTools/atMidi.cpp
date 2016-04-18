//
//  atMidi.cpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-18.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#include "PyoAudio.h"
#include "atMidi.h"

namespace at {
Midi* Midi::_global_midi = nullptr;

Midi* Midi::GetInstance()
{
	if (_global_midi == nullptr) {
		_global_midi = new Midi();
	}

	return _global_midi;
}

Midi::Midi()
	: ax::Event::Object(ax::App::GetInstance().GetEventManager())
{
}

void Midi::OnMidiNoteOn(const ax::midi::Note& msg)
{
	PyoAudio::GetInstance()->ProcessMidi(144, msg.GetNote(), msg.GetVelocity());
	ax::Print("ON", msg.GetNote(), msg.GetVelocity());
}

void Midi::OnMidiNoteOff(const ax::midi::Note& msg)
{
	PyoAudio::GetInstance()->ProcessMidi(144, msg.GetNote(), msg.GetVelocity());
	ax::Print("OFF", msg.GetNote(), msg.GetVelocity());
}
}