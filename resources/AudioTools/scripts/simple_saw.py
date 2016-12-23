# Simple saw synth.
midi = Notein(scale=1);
midi_env = MidiAdsr(midi['velocity'], attack=0.05, decay=0.1, sustain=0.4, release=0.1);

saw_wave = SuperSaw(freq=midi['pitch'], detune=0.5, bal=0.7, mul=midi_env, add=0);
lfo = Sine(freq=8, mul=1.0, add=1.0);
filter = Biquadx(saw_wave, freq=200 + lfo * 500, q=0.707, type=0).out();

# lfo = Sine(freq=8, mul=2.0, add=1.0);
# lp_filter = Biquad(saw_wave, freq=200, q=1, type=0, mul=1, add=0).mix(1).out();



def OnSetLPLfoFreq(value):
    lfo.setFreq(2.0 + value * 10);

# def OnSetFilterFreq(value):
    # filter.setFreq(100 + value * 5000);

def OnSetFilterQ(value):
    filter.setQ(value * 5.0);

def OnSetDetune(value):
    saw_wave.setDetune(value);

def OnSetEnvMul(value):
    print value;
    

def OnSetAttack(value):
    midi_env.setAttack(value);

def OnSetDecay(value):
    midi_env.setDecay(value);

def OnSetSustain(value):
    midi_env.setSustain(value);

def OnSetRelease(value):
    midi_env.setRelease(value);








