#!/usr/bin/python
# coding: utf-8
from matplotlib import pyplot
import string
import sys
import numpy as np


pyplot.figure(figsize = (9,6))
pyplot.title('2D-ising model for Ferromagnetism Metropolice Method, z VS m')
pyplot.xlabel('z')
pyplot.ylabel('m')
raw_data = np.loadtxt('data_L_z_m.txt',comments='a')

data = []
arr = [[0 for i in range(3)] for j in range(5)]
z_data = [[0 for i in range(32)] for j in range(6)]
m_data = [[0 for i in range(32)] for j in range(6)]
L = [12,16,24,32,48,64]


for i in range(6):
    data = raw_data[32*i:32*(i+1)]
    for j in range(32):
        z_data[i][j] = data[j][0]
        m_data[i][j] = data[j][1]

    pyplot.plot(z_data[i],m_data[i],label = "L = {}".format(L[i]))


pyplot.legend()
pyplot.show()
