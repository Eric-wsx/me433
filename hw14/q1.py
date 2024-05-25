import matplotlib.pyplot as plt
import numpy as np
import csv

# Function to compute the FFT of the data
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

    return abs(Y), frq

# Function to compute the moving average filter
def mymaf(data, window_size):
    return np.convolve(data, np.ones(window_size)/window_size, mode='same')

# Read data from the CSV file
t = []
data1 = []

with open('C:/Users/zjxu/me433/hw14/sigD.csv') as f:
    reader = csv.reader(f)
    for row in reader:
        t.append(float(row[0]))  # Leftmost column
        data1.append(float(row[1]))  # Second column

# Apply the moving average filter
window_size = 30
data2 = mymaf(data1, window_size)

# Plot the original and filtered data
fig, ax1 = plt.subplots(1, 1)
ax1.plot(t, data1, 'black', label='Original Data')
ax1.plot(t, data2, 'red', label='Filtered Data')
ax1.set_xlabel('Time')
ax1.set_ylabel('Amplitude')
ax1.set_title(f'sigD, window_size={window_size}')
ax1.legend()

# Save the plot to a file
save_path = 'C:/Users/zjxu/me433/hw14/figure/q1_sigD.png'
plt.savefig(save_path)

# Show the plot
plt.show()