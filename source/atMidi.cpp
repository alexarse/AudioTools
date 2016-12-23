/*
 * Copyright (c) 2016 AudioTools - All Rights Reserved
 *
 * This Software may not be distributed in parts or its entirety
 * without prior written agreement by AudioTools.
 *
 * Neither the name of the AudioTools nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY AUDIOTOOLS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL AUDIOTOOLS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Written by Alexandre Arsenault <alx.arsenault@gmail.com>
 */

#include "atMidi.h"
#include "PyoAudio.h"

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
	: ax::event::Object(ax::App::GetInstance().GetEventManager())
{
}

void Midi::OnMidiNoteOn(const atk::MidiNote& msg)
{
	PyoAudio::GetInstance()->ProcessMidi(144, msg.GetNote(), msg.GetVelocity());
	ax::console::Print("ON", msg.GetNote(), msg.GetVelocity());
}

void Midi::OnMidiNoteOff(const atk::MidiNote& msg)
{
	PyoAudio::GetInstance()->ProcessMidi(144, msg.GetNote(), msg.GetVelocity());
	ax::console::Print("OFF", msg.GetNote(), msg.GetVelocity());
}
}
