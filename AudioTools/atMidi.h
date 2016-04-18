//
//  atMidi.hpp
//  AudioTools
//
//  Created by Alexandre Arsenault on 2016-04-18.
//  Copyright © 2016 Alexandre Arsenault. All rights reserved.
//

#ifndef atMidi_hpp
#define atMidi_hpp

#include <OpenAX/OpenAX.h>
#include <OpenAX/axMidiCore.h>

namespace at {
	class Midi : public ax::midi::Core, public ax::Event::Object {
	public:
		static Midi* GetInstance();
		
		virtual void OnMidiNoteOn(const ax::midi::Note& msg);
		
		virtual void OnMidiNoteOff(const ax::midi::Note& msg);
		
	private:
		Midi();
		static Midi* _global_midi;

	};
}

#endif /* atMidi_hpp */
