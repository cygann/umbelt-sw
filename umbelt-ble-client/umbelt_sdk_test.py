from umbelt import Umbelt


umb = Umbelt(name="Umlaut")

if umb.connect():
    umb.vibrate_motor(motor_idx=1, duration=200, intensity=1.0)

