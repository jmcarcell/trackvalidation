#!/usr/bin/env python3
import fileinput
import argparse

parser = argparse.ArgumentParser(description='Produce pdf summary file starting from release_summary_template.tex')
parser.add_argument('--filein', help='Name file input')
parser.add_argument('--fileout', help='Name file output')
parser.add_argument('--release', help='Release for which the validation was performed')
parser.add_argument('--path', help='Path for plots folder')
parser.add_argument('--authorname', help='Author name')
parser.add_argument('--authorinstitute', help='Author institute')
parser.add_argument('--date', help='Date of the presentation')
parser.add_argument('--minHits_singlePart', help='Minim number of hits in single particles')
parser.add_argument('--minHits_complex', help='Minim number of hits in complex events')
parser.add_argument('--release2', help='Comparison release for which the validation was also performed', default='')
parser.add_argument('--path2', help='Path for plots in comparison folder', default='')
args = parser.parse_args()
print(args)

NAMEINFILE  = args.filein
NAMEOUTFILE = args.fileout
REPLACEARGS = {"#RELEASE#": args.release, "#PATHPLOTS#": args.path, "#DATE#": args.date, 
               "#AUTHORNAME#": args.authorname, "#AUTHORINSTITUTE#": args.authorinstitute,
               "#MINHITS_SINGLE#": args.minHits_singlePart, "#MINHITS_COMPLEX#": args.minHits_complex,
               "#RELEASE2#": args.release2, "#PATHPLOTS2#": args.path2}

#Open file
f = open(NAMEINFILE,'r')
filedata = f.read()
f.close()

#Copy file and replace
newdata = filedata
for original,new in REPLACEARGS.iteritems():
  newdata = newdata.replace(original, new)

#Write new file
f = open(NAMEOUTFILE,'w')
f.write(newdata)
f.close()

print('%s was created'%NAMEOUTFILE)
