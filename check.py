#!/usr/bin/env python

from sys import argv
import re

renumber = re.compile(r'^\d+$')

handle = open("andrew.txt")
lines = handle.readlines()
handle.close()

while lines:
    line = lines.pop()
    print line

