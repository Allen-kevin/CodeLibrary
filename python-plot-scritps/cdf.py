import numpy as np
import matplotlib.pyplot as plt
import sys

def cdf_plot(http, https, qy_http, qy_high, qy_low, y):

    plt.plot(http, y, color = 'green', label='http')
    plt.plot(https, y, color = 'red', label='https')
    plt.plot(qy_http, y, color = 'blue', label='qingyun http')
    plt.plot(qy_high, y, color = 'skyblue', label='qingyun high')
    plt.plot(qy_low, y, color = 'gray', label='qingyun low')
    plt.legend()
    plt.xlabel('tail latency: ms')
    plt.ylabel('CDF')
    plt.savefig('current-200-tail-latency.png')
    plt.show()

def get_data(f):
    y = []
    x = []
    lines = open(f).readlines()
    for line in lines:
        y.append(float(line.split()[0]))
        x.append(float(line.split()[1]))

    return x, y

def main(argv):
        http, y = get_data(argv[0])
        https, y = get_data(argv[1])
        qy_http, y = get_data(argv[2])
        qy_high, y = get_data(argv[3])
        qy_low, y = get_data(argv[4])
        cdf_plot(http, https, qy_http, qy_high, qy_low, y)

if __name__=='__main__':
    main(sys.argv[1:])
