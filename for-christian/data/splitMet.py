#! /usr/bin/env python

import argparse

parser = argparse.ArgumentParser(description='')


parser.add_argument("-f", "--file", dest="filename",
                    help="input file", metavar="FILE")

parser.add_argument("-d", "--debug", type=int,
                  dest="debug", default=0,
                  help="activate debug printing")

parser.add_argument("-v", "--verbose", type=int,
                  dest="verbose", default=0,
                  help="activate verbose printing levels 1, 2 and 3 (very verbose!)")

parser.add_argument("-c" , "--comments",  action="store_true",
                     dest="comments" )

parser.add_argument("-dc" , "--dumpcomments",  action="store_true",
                     dest="dumpcomments" )
args = parser.parse_args()
file = open("splitedMet","w") 
if  args.filename == None and sys.argv[1] != None:
     infile = sys.argv[1]
else:
     infile = args.filename
try:
    with open ( infile , "r" ) as myfile:
            for line in myfile:
                values = line.split("#")
                file.write(values[0]+"#" + "\n")
                #print(values[0]+"#" + "\n")
                
except Exception:
    print ( "\nUnable to read file: " + Exception )
