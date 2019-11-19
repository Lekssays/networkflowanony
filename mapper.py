import redis
import csv
import re
import random
import argparse

r = redis.Redis(host='localhost', port=6379, db=0)

def parse_args():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument('-f', '--file',
                        dest = "file",
                        help = "Name of the file",
                        default = "flows.csv",
                        required = False)
    return parser.parse_args()  

def write(filename, entry):
    out = open(filename, "a")
    out.write(entry + "\n")

def loadCSV(filename):
    ipsSet = set()
    edges = set()
    ipsDict = {}
    idx = 0
    with open(filename) as csvfile:
        readCSV = csv.reader(csvfile)
        for row in readCSV:
            ip1 = row[0]
            ip2 = row[1]
            if re.match(r"^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}$",ip1) and re.match(r"^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}$",ip2):
                if ip1 not in ipsSet:
                    ipsSet.add(ip1)
                    ipsDict[ip1] = idx
                    r.set(idx, ip1)
                    idx += 1
                if ip2 not in ipsSet:
                    ipsSet.add(ip2)
                    ipsDict[ip2] = idx
                    r.set(idx, ip2)
                    idx += 1
            edges.add((ipsDict[ip1], ipsDict[ip2]))
    return ipsSet, edges
                
def main():
    filename = "mapping_" + parse_args().file
    ips, edges = loadCSV(filename)
    write("id_" + filename, str(len(ips)) + " " + str(len(edges)))
    for edge in edges:
        entry = str(edge[0]) + " " + str(edge[1])
        write("id_" + filename, entry)
        #print(entry)

if __name__ == "__main__":
  main()
