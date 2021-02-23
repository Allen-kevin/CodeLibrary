import matplotlib.pyplot as plt
import numpy as np
from matplotlib.ticker import MaxNLocator
from collections import namedtuple
import re


def type_conversion(line):
    tail = []
    substr = line.split()[1:]
    for i in substr:
        if "us" in i:
            tail.append(float(re.sub('us', '', i))/1000)
        elif "ms" in i:
            tail.append(float(re.sub('ms', '', i)))
    print(tail)
    return tail

def get_data(filename, name):
    x = ()
    y = ()
    with open(filename, "r") as f:
        lines = f.readlines()
        x = lines[0].split()[1:]
        for line in lines[1:]:
            if name in line:
                y = map(float, line.split()[1:])
    return x, y 

def rps_plot(x, y1, y2, y3, y4, y5, y6):
    FontSize=8
    fig, ax = plt.subplots()
    # plot
    total_width = 0.9
    n = 6
    bar_width = total_width/n
    
    opacity = 0.4
    error_config = {'ecolor':'0.3'}
    
    http = np.arange(len(x))
    https = http + bar_width
    plt.bar(http, map(float, y1), width=bar_width, alpha=opacity, 
            color='blue', hatch='..', label='http')
    plt.bar(http+bar_width, map(float, y2), width=bar_width, alpha=opacity, 
            color='navy',  hatch='oo', label='https')
    plt.bar(http+2*bar_width, map(float, y3), width=bar_width, alpha=opacity, 
            color='salmon', hatch='///', label='qingyun http')
    plt.bar(http+3*bar_width, map(float, y4), width=bar_width, alpha=opacity, 
            color='r',  hatch='////', label='qingyun https')
    plt.bar(http+4*bar_width, map(float, y5), width=bar_width, alpha=opacity, 
            color='darkgrey', label='fstack http')
    plt.bar(http+5*bar_width, map(float, y6), width=bar_width, alpha=opacity, 
            color='dimgray', label='fstack https')

    plt.ylim((0, 160000))
    plt.xlabel("connections")
    plt.ylabel("RPS")
    plt.title('RPS under different stacks')
    ax.set_xticks(http+bar_width*3)
    ax.set_xticklabels(x)
    
#    for a,b in zip(http, y1):
#        plt.text(a, b+0.5, '%.0f' % b, ha='center', va='bottom', fontsize=7)
#    for a,b in zip(https, y2):
#        plt.text(a, b+0.5, '%.0f' % b, ha='center', va='bottom', fontsize=7)
    
    plt.legend(loc='upper left', fontsize=FontSize, bbox_to_anchor=(0.02, 0.99), ncol=3, fancybox=True, shadow=True)
    plt.savefig("wrk-rps.png")
#    fig.tight_layout()
    plt.show()


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
    plt.title('99% tail latency of qingyun')
    ax.set_xticks(http+bar_width/2)
    ax.set_xticklabels(x)
    
    for a,b in zip(http, y1):
        plt.text(a, b+0.5, '%.0f' % b, ha='center', va='bottom', fontsize=7)
    for a,b in zip(https, y2):
        plt.text(a, b+0.5, '%.0f' % b, ha='center', va='bottom', fontsize=7)
    
    plt.legend(loc='upper right', fontsize=FontSize)
    plt.savefig("wrk-tail-latency.png")
#    fig.tight_layout()
    plt.show()


def data_build_plot():

    x, http = get_data("rps.txt", "k_http")
    x , https = get_data("rps.txt", "k_tls")
    x , qy_http = get_data("rps.txt", "qyun_http")
    x , qy_https = get_data("rps.txt", "qingyun_tls")
    x , f_http = get_data("rps.txt", "f_http")
    x , f_https = get_data("rps.txt", "f_tls")

    print(x)
    print(http)
    print(https)
    print(qy_http)
    print(qy_https)
    print(f_http)
    print(f_https)
    rps_plot(x, http, https, qy_http, qy_https, f_http, f_https)
#    tail_latency_plot(x, http_latency, https_latency)

if __name__ == "__main__":
    data_build_plot()
