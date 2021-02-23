import matplotlib.pyplot as plt
import numpy as np
from matplotlib.ticker import MaxNLocator
from collections import namedtuple
import re

def get_data(filename, name):
    latency = ()
    with open(filename, "r") as f:
        lines = f.readlines()
        for line in lines:
            if "name" in line:
                latency = map(float, line.split()[1:])
    return latency

def tail_latency_plot(x, y1, y2):
    FontSize=10
    fig, ax = plt.subplots()
    # plot
    bar_width = 0.2
    opacity = 0.4
    error_config = {'ecolor':'0.3'}
    
    http = np.arange(len(x))
    https = http + bar_width
    plt.bar(http, map(float, y1), width=bar_width, alpha=opacity, 
            color='b', hatch='///', label='http')
    plt.bar(https, map(float, y2), width=bar_width, alpha=opacity, 
            color='r',  hatch='////', label='https')
#    plt.ylim((0, 150000))
    plt.xlabel("connection")
    plt.ylabel("Tail Latency: ms")
    plt.title('tail latency of qingyun')
    ax.set_xticks(http+bar_width/2)
    ax.set_xticklabels(x)
    
    for a,b in zip(http, y1):
        plt.text(a, b+0.5, '%.0f' % b, ha='center', va='bottom', fontsize=7)
    for a,b in zip(https, y2):
        plt.text(a, b+0.5, '%.0f' % b, ha='center', va='bottom', fontsize=7)
    
    plt.legend(loc='upper right', fontsize=FontSize)
    plt.savefig("monitor-tail-latency.png")
#    fig.tight_layout()
    plt.show()


def data_build_plot():
    x = ['99%', '99.9%']
    http_latency = get_data("latency.txt", "99%latency")
    https_latency = get_data("latency.txt", "99.9%latency")
    tail_latency_plot(x, http_latency, https_latency)

if __name__ == "__main__":
    data_build_plot()
