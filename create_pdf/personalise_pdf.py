#!/usr/bin/env python3
import fileinput
import argparse

parser = argparse.ArgumentParser(description='Produce pdf summary file starting from release_summary_template.tex')
parser.add_argument('--release', help='Release for which the validation was performed')
parser.add_argument('--authorname', help='Author name')
parser.add_argument('--authorinstitute', help='Author institute')
parser.add_argument('--date', help='Date of the presentation')
args = parser.parse_args()
print(args)

NAMEINFILE  = "release_summary_template.tex"
NAMEOUTFILE = "release_summary.tex"
REPLACEARGS = {"RELEASE": args.release, "AUTHORNAME": args.authorname,
               "AUTHORINSTITUTE": args.authorinstitute, "DATE": args.date}

#Open file
f = open(NAMEINFILE,'r')
filedata = f.read()
f.close()

#Copy file and replace
newdata = filedata
for original,new in REPLACEARGS.iteritems():
  newdata = newdata.replace(original,new)

#Write new file
f = open(NAMEOUTFILE,'w')
f.write(newdata)
f.close()
