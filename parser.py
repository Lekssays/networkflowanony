import redis
import csv
import re
import random
import argparse

r = redis.Redis(host='localhost', port=6379, db=0)

def parse_args():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument('-f', '--file',
                        dest = "filename",
                        help = "Name of the file",
                        default = "flows.csv",
                        required = False)
    return parser.parse_args() 

def write(filename, entry):
    out = open(filename, "a")
    out.write(entry + "\n")

def makeRow(ip1, ip2, row):
    tmp = []
    tmp.append(ip1)
    tmp.append(ip2)
    tmp.append(row[2])
    tmp.append(row[3])
    tmp.append(row[4])
    return tmp

def getPrefixPostfix(ip):
    tmpIp = ip.split(".")
    prefix = tmpIp[0] + "."  + tmpIp[1]
    postfix = tmpIp[2] + "."  + tmpIp[3]
    return prefix, postfix

def loadCSV(filename):
    rows = []
    with open(filename) as csvfile:
        readCSV = csv.reader(csvfile)
        for row in readCSV:
            rows.append(row)
    return rows           

def generateFakePrefix():
    fakePrefix = str(random.randint(1, 254)) + "." + str(random.randint(1, 254))
    return fakePrefix

def mapPrefix(prefixes):
    prefixDict = {}
    for prefix in prefixes:
        fakePrefix = generateFakePrefix()
        if prefix not in prefixDict.keys():
            prefixDict[prefix] = fakePrefix
            r.set(fakePrefix, prefix)
    return prefixDict

def processPrefix(rows):
    seen = set()

    for row in rows:
        ip1 = row[0]
        ip2 = row[1]
        if re.match(r"^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}$",ip1) and re.match(r"^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}$",ip2):
            prefix1, postfix1 = getPrefixPostfix(ip1)
            prefix2, postfix2 = getPrefixPostfix(ip2)
            seen.add(prefix1)
            seen.add(prefix2)
    return seen

def generateNewFile(filename, dictPrefixes, rows):
    for row in rows:
        ip1 = row[0]
        ip2 = row[1]
        if re.match(r"^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}$",ip1) and re.match(r"^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}$",ip2):
            prefix1, postfix1 = getPrefixPostfix(ip1)
            prefix2, postfix2 = getPrefixPostfix(ip2)        
            fakePrefix1 = dictPrefixes[prefix1]
            fakePrefix2 = dictPrefixes[prefix2]
            fakeIP1 = fakePrefix1 + "." + postfix1
            fakeIP2 = fakePrefix2 + "." + postfix2
            tmp = makeRow(fakeIP1, fakeIP2, row)
            fakeRow = ",".join(tmp)
            write("mapping_" + filename, fakeRow)
            #print(fakeRow)

def main():
    filename = parse_args().filename
    #filename = "flows.csv"
    rows = loadCSV(filename)
    prefixes = processPrefix(rows)
    dictPrefixes = mapPrefix(prefixes)
    generateNewFile(filename, dictPrefixes, rows)

if __name__ == "__main__":
  main()
