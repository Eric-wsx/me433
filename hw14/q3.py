import matplotlib.pyplot as plt
import numpy as np
import csv

h = [
    0.106829760062658569,
    0.110016815310794311,
    0.112327457130323383,
    0.113727633012899371,
    0.114196668966648776,
    0.113727633012899371,
    0.112327457130323383,
    0.110016815310794311,
    0.106829760062658569,
]
def myfft(data,time):
    
    y = data # the data to make the fft from
    n = len(y) # length of the signal
    Fs = n/time[-1] # sample rate
    k = np.arange(n)
    T = n/Fs
    frq = k/T # two sides frequency range
    frq = frq[range(int(n/2))] # one side frequency range
    Y = np.fft.fft(y)/n # fft computing and normalization
    Y = Y[range(int(n/2))]

    return abs(Y),frq

def myFIR(data,coef):
    window_size = len(coef)-1
    FIRdata = np.zeros(window_size)

    for i in range(len(data)):
        window = np.append(FIRdata[-window_size:],data[i])

        average = np.sum(np.multiply(window,coef))
        FIRdata = np.append([FIRdata],[average])
    
    print(FIRdata)
    return FIRdata[window_size:]


t = [] # column 0
data1 = [] # column 1


with open('C:/Users/zjxu/me433/hw14/sigD.csv') as f:
    # open the csv file
    reader = csv.reader(f)
    for row in reader:
            # read the rows 1 one by one
            t.append(float(row[0])) # leftmost column
            data1.append(float(row[1])) # second column

data2 =  myFIR(data1,h)

fig, ax1 = plt.subplots(1, 1)
ax1.plot(t,data1,'black')
ax1.set_xlabel('Time')
ax1.set_ylabel('Amplitude')
ax1.plot(t,data2,'red')
ax1.set_title('sigD,Cutoff = 250Hz,Transition = 450Hz, method = Rectangular')

# Save the plot to a file
save_path = 'C:/Users/zjxu/me433/hw14/figure/q3_sigD.png'
plt.savefig(save_path)


plt.show()