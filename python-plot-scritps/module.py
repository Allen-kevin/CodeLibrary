import getopt
import os
import io

def para_parse(argv):
    try:
        opts, args = getopt.getopt(argv, "hi:o:", ["help", "ifile=", "ofile"])
    except getopt.GetoptError:
        print('python test.py -i <inputfile> -o <outputfile>')
        sys.exit(2)
    for opt, arg in opts:
        if opt == ("-h", "--help"):
            print('python test.py -i <inputfile> -o <outputfile>')
            sys.exit()
        elif opt in ("-i", "--ifile"):
            inputfile = arg
        elif opt in ("-o", "--ofile"):
            outputfile = arg
        
    return inputfile, outputfile


def write_result_output(filename, latency_list):
    with open(filename, "a") as f:
        for i in range(len(latency_list)):
            f.write(' ')
            f.write(latency_list[i])
        f.write('\n')


def build_list(line, data_list, number):
    line.strip()
    data_list.append(line.split()[number])

