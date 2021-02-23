import sys
import getopt
import os
import io
import module

def monitor_text_parse(filename, latency_list, name):
    with open(filename, "r") as f:
        lines = f.readlines()
        for line in lines:
            if name in line:
                module.build_list(line, latency_list, 1)
    
def main(argv):
    inputfile, outputfile = module.para_parse(argv)
    latency_list = ["latency"]
    monitor_text_parse(inputfile, latency_list, '0.99:')
    monitor_text_parse(inputfile, latency_list, '0.9990:')
    print(latency_list)
    if os.path.exists(outputfile):
        os.remove(outputfile)
    x = ['parcent', '99%', '99.9%']
    module.write_result_output(outputfile, x)
    module.write_result_output(outputfile, latency_list)

if __name__ == "__main__":
    main(sys.argv[1:])
