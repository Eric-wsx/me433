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

    return abs(Y), frq

def mymaf_IIR(data, A, B):
    mafdata = [0]

    for i in range(len(data)):
        average = mafdata[-1] * A + data[i] * B
        mafdata.append(average)

    return mafdata[1:]

# Read data from the CSV file
t = []
data1 = []

with open('C:/Users/zjxu/me433/hw14/sigD.csv') as f:
    reader = csv.reader(f)
    for row in reader:
        t.append(float(row[0]))  # Leftmost column
        data1.append(float(row[1]))  # Second column

# Apply the IIR filter
A = 0.95
B = 1 - A
data2 = mymaf_IIR(data1, A, B)

# Plot the original and filtered data
fig, ax1 = plt.subplots(1, 1)
ax1.plot(t, data1, 'black', label='Original Data')
ax1.plot(t, data2, 'red', label='Filtered Data')
ax1.set_xlabel('Time')
ax1.set_ylabel('Amplitude')
ax1.set_title(f'sigD, A={A}, B={B}')
ax1.legend()

# Save the plot to a file
save_path = 'C:/Users/zjxu/me433/hw14/figure/q2_sigD.png'
plt.savefig(save_path)

# Show the plot
plt.show()