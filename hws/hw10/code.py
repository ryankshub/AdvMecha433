from ulab import numpy as np # to get access to ulab numpy functions
import time
# Declare an array with some made up data like

secs = np.linspace(0, 3, 1024)
oneHz_sin_wave = np.array([np.sin(2*np.pi*t) for t in secs])
twoHz_sin_wave = np.array([np.sin(4*np.pi*t) for t in secs])
fourHz_sin_wave = np.array([np.sin(8*np.pi*t) for t in secs])
mega_sin_wave = oneHz_sin_wave + twoHz_sin_wave + fourHz_sin_wave


fft_mag = np.fft.fft(mega_sin_wave)[0]
for freq in fft_mag[0:100]: #Only do the first 100 to capture the signals
    time.sleep(.01)
    print((freq, ))