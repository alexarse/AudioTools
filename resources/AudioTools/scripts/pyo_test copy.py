midi = Notein(scale=1);
midi_env = MidiAdsr(midi['velocity'], attack=.05, decay=.1, sustain=.4, release=0.1);
sine_wave = Sine(midi['pitch'], mul=midi_env).out();

# tsnd = SndTable("accord.aif");
# tenv = CosTable([(0,0), (100,1), (1000,.5), (8192,0)])
# met = Metro(.125, poly=2).play()
# amp = TrigEnv(met, table=tenv, dur=.25, mul=.7)
# mid = TrigChoice(met, choice=[43, 45, 60, 63], port=.0025)
# sp = MToT(mid)
# snd = Osc(tsnd, freq=tsnd.getRate()/sp, mul=amp).out()
# adsr = Adsr(attack=.01, decay=.2, sustain=.5, release=.3, dur=2, mul=.5);
# freq_adsr = Adsr(attack=.01, decay=.1, sustain=.2, release=.3, dur=2, mul=.5);

# lfo = Sine([5], mul=0.5, add = 0.5);
# sine = Sine(200 + freq_adsr * 2000, mul=adsr).out();
# sine3 = Sine(800 + freq_adsr * 500, mul=adsr).out();


# mid = Notein(scale=1)
# env = MidiAdsr(mid['velocity'], attack=.005, decay=.1, sustain=.4, release=1)
# a = SineLoop(freq=mid['pitch'], feedback=.8, mul=env).out()
# mid = Notein(scale=1)
# env = MidiAdsr(mid['velocity'], attack=.005, decay=.1, sustain=0, release=0.1)
# sine2 = Sine(mid['pitch'], mul=env).out();


# >>> a = SineLoop(freq=mid['pitch'], feedback=.1, mul=env).out()
# >>> b = SineLoop(freq=mid['pitch']*1.005, feedback=.1, mul=env).out(1)

# mid = Notein(scale=1);
# sp = MToT(mid['pitch']);
# mid_env = MidiAdsr(mid['velocity'], attack=.005, decay=.1, sustain=.4, release=1);
# snd = SndTable("snd_4.aif");
# env = HannTable();

# pos = Phasor(snd.getRate() * .25, 0, snd.getSize());
# dur = Noise(.001, .1);
# g = Granulator(snd, env, mid['pitch'], pos, dur, 24, mul=mid_env).out();

# snd = SndTable("baseballmajeur_m.aif");
# env = HannTable();
# pos = Phasor(snd.getRate()*.25, 0, snd.getSize());
# dur = Noise(.001, .1);
# g = Granulator(snd, env, [1, 1.001], pos, dur, 24, mul=.1).out()

# def OnSetLfoFreq(value):
#     v = 5.0 + 20.0 * value;
#     lfo.setFreq(v);

# def OnSetSineMul(value):
#     sine.setMul(value);

# def OnTrigger():
#     adsr.play();
#     freq_adsr.play();

# def OnTrigger2():
    # adsr2.play();

