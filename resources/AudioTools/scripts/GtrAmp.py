snd_path = "sounds/g_tdd_120_01.wav";
sf = SfPlayer(snd_path, speed=1.0, loop=True, mul=.3);
disto = Disto(sf, drive=0.5, slope=.8, mul=.15).out();
# disto.ctrl(title="Frequency modulation controls")

# boost from -20 to 20.0
low_eq = EQ(disto, freq=167, q=1.76, boost=-3.0, type=0, mul=1, add=0).out();
# low_eq.ctrl(title="EQ modulation controls")
# s.gui(locals())

def OnSetDrive(value):
    disto.setDrive(value);

 # SfPlayer(path, speed=1, loop=False, offset=0, interp=2, mul=1, add=0)
# EQ(input, freq=1000, q=1, boost=-3.0, type=0, mul=1, add=0)

