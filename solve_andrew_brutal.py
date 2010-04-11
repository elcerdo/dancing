#!/usr/bin/env python

from copy import copy

def permutation(elems):
    if len(elems) == 1:
        yield copy(elems)
        return
    for elem in elems:
        elemsp = copy(elems)
        elemsp.remove(elem)
        for perm in permutation(elemsp):
            perm.append(elem)
            yield perm

possible_lines = [
[2,4,5,9],
[1,3,6,10],
[2,4,7,11],
[1,3,8,12],
[1,6,8,13],
[2,5,7,14],
[3,6,8,15],
[4,5,7,16],
[1,10,12,13],
[2,9,11,14],
[3,10,12,15],
[4,9,11,16],
[5,9,14,16],
[6,10,13,15],
[7,11,14,16],
[8,12,13,15]]

def possible_array(lines):
    if not lines:
        yield []
        return
    for perm in permutation(lines[0]):
        for perms in possible_array(lines[1:]):
            yield [perm] + perms


for k,perm in enumerate(permutation([1,2,3,4])):
    print k,perm
print 4*3*2

for k,array in enumerate(possible_array(possible_lines[:5])):
    print k,array
print (4*3*2)**5
