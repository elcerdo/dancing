#!/usr/bin/env python

import scipy as ss
from sys import argv
import re

renumber = re.compile(r'^(\d+)$')
rerow    = re.compile(r'^ *\d+\| +(\d+) +(\d+) +(\d+) +(\d+)$')

handle = open("andrew.txt")
lines = handle.readlines()
handle.close()

lines.reverse()

arrays  = []
darrays = {}
sarrays = []
while lines:
    line = lines.pop()
    match = renumber.match(line)
    groups = match.groups()
    number = int(groups[0])

    line = lines.pop()
    line = lines.pop()

    array = []
    for i in xrange(16):
        line = lines.pop()
        match = rerow.match(line)
        groups = match.groups()
        array.append(map(int,groups))

    line = lines.pop()

    arrays.append(array)
    sarrays.append(ss.array(array,dtype=ss.integer))

    if repr(array) in darrays:
        print "duplicate array"
    darrays[repr(array)] = number

