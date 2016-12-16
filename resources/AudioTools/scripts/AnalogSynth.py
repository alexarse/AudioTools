#-------------------------------------------------------------------------
# Analog synth.
#-------------------------------------------------------------------------
# Author     : Alexandre Arsenault
# Email      : alx.arsenault@gmail.com
# Copyright  : AudioTools 2016
# License    : GPL
# Version    : 1.0
#-------------------------------------------------------------------------

# Midi.
midi = Notein(scale=1);
midi_env = MidiAdsr(midi['velocity'], attack=0.05, decay=0.1, sustain=0.4, release=0.1);
pitch = midi['pitch'];

# Osc 1.
saw_wave = SuperSaw(freq=pitch, mul=midi_env);
saw_pan = Pan(saw_wave, outs=2);
output = Mix(saw_pan, voices=2);

# Osc 2.
square_table = SquareTable();
osc = Osc(square_table, freq=pitch, mul=midi_env);
square_pan = Pan(osc, outs=2);
output2 = Mix(square_pan, voices=2);

# Filter LFO
lfo = LFO(freq=2.0, sharp=0.5, type=0, mul=1, add=0);

# Filter.
b_filter = Biquad(output + output2, freq=200.0 + lfo * 300.0, q=0.7, type=0).out();

def SetLfoFreq(value):
    lfo.setFreq(2.0 + value * 12);

def SetLfoAmount(value):
    lfo.setMul(value);

def SetFilterFreq(value):
    b_filter.setFreq(100.0 + value * 5000.0 + lfo * 300.0);

def SetFilterRes(value):
    b_filter.setQ(0.1 + value * 8.0);

def SetOsc1Volume(value):
    output.setMul(value);

def OnOsc2Volume(value):
    output2.setMul(value);

def OnSetDetuneOsc1(value):
    saw_wave.setDetune(value);

def SetBalance(value):
    saw_wave.setBal(value);

def OnOsc2Feedback(value):
    sine_wave.setFeedback(value);

def OnSetAttack(value):
    midi_env.setAttack(value);
    print value;

def OnSetDecay(value):
    midi_env.setDecay(value);

def OnSetSustain(value):
    midi_env.setSustain(value);

def OnSetRelease(value):
    midi_env.setRelease(value);








