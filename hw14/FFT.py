import matplotlib.pyplot as plt
import numpy as np
import csv

def myfft(data, time):
    y = data
    n = len(y)
    Fs = n / time[-1]  # Sample rate
    k = np.arange(n)
    T = n / Fs
    frq = k / T  # Two sides frequency range
    frq = frq[range(int(n/2))]  # One side frequency range
    Y = np.fft.fft(y) / n  # FFT computing and normalization
    Y = Y[range(int(n/2))]
    print(Fs)
    return abs(Y), frq

# Read data from the CSV file
t = []
data1 = []

with open('C:/Users/zjxu/me433/hw14/sigD.csv') as f:
    reader = csv.reader(f)
    for row in reader:
        t.append(float(row[0]))  # Leftmost column
        data1.append(float(row[1]))  # Second column

# Compute FFT
Y, frequency = myfft(data1, t)

# Plotting
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8))

ax1.plot(t, data1, 'black')
ax1.set_xlabel('Time')
ax1.set_ylabel('Amplitude')
ax1.set_title('Time Domain Signal')

ax2.loglog(frequency, Y, 'b')  # Plotting the FFT
ax2.set_xlabel('Frequency (Hz)')
ax2.set_ylabel('|Y(frequency)|')
ax2.set_title('Frequency Domain Signal')

# Adjust layout to prevent overlap
plt.tight_layout()

# Save the plot to a file
save_path = 'C:/Users/zjxu/me433/hw14/figure/FFT_sigD.png'
plt.savefig(save_path)

# Show the plot
plt.show()