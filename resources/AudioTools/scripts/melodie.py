t = CosTable([(0,0), (50,1), (250,.3), (8191,0)])
met = Metro(time=.125, poly=2).play()
amp = TrigEnv(met, table=t, dur=.25, mul=.8)
freq = TrigRand(met, min=400, max=1000)
a = Sine(freq=freq, mul=amp).out()

def OnSetMetro(value):
    met.setTime(0.05 + value * 0.5);

def OnSetDur(value):
    amp.setDur(0.05 + value);










