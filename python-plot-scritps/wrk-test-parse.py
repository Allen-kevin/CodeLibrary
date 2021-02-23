import sys
import getopt
import os
import io
import re
import module


def get_tail_latency(line, tail_latency_list, number):
    line.strip()
    substr = line.split()[number]
    if "us" in substr:
        tail_latency_list.append(str(float(re.sub('us', '', substr))/1000))
    elif "ms" in substr:
        tail_latency_list.append(re.sub('ms', '', substr))

def text_parse(filename, data_list, name, number, fun):
    with open(filename, "r") as f:
        lines = f.readlines()
        for line in lines:
            if name in line:
                fun(line, data_list, number)

def main(argv):
    conn_list = ["conn"]
    rps_list = ["RPS"]
    tail_latency_list = ["99%latency"]

    inputfile, outputfile = module.para_parse(argv)    
    text_parse(inputfile, conn_list, "threads", 3, module.build_list)
    text_parse(inputfile, rps_list, "Requests/sec", 1, module.build_list)
    text_parse(inputfile, tail_latency_list, "99%", 1, get_tail_latency)
   # if os.path.exists(outputfile):
   #     os.remove(outputfile)
    module.write_result_output(outputfile, conn_list)
    module.write_result_output(outputfile, rps_list)
    module.write_result_output(outputfile, tail_latency_list)

if __name__ == "__main__":
    main(sys.argv[1:])
