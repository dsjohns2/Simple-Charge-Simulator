import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt("particle.dat")
t = data[:, 0]
x = data[:, 1]
y = data[:, 2]

fig = plt.figure();
plt.title("XY Plot")
plt.plot([0], [0], marker='x', markersize=3, color="red")
plt.plot(x, y)
plt.xlim((-6,6))
plt.ylim((-6,6))
plt.ylabel('y')
plt.xlabel('x')
plt.savefig("xyplot.png")

fig = plt.figure();
plt.title("TX Plot")
plt.plot(t, x)
plt.xlim((0,None))
plt.ylabel('x')
plt.xlabel('t')
plt.savefig("txplot.png")
