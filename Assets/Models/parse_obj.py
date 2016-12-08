# -*- coding: utf-8 -*-
"""
Created on Thu Dec 08 09:25:55 2016

@author: alan
"""

is_stone = False
is_brick = False
is_wood_4 = False
is_wood_3 = False
is_light2 = False

f_stonev = open('Light2v.txt', 'w')
f_stonevt = open('Light2vt.txt', 'w')
f_stonef = open('Light2f.txt', 'w')

f = open('map.obj', 'r')
for line in f:
    if line.find('usemtl Light2') != -1:
        is_stone = True
        continue
    elif line.find('usemtl stone08') != -1:
        is_brick = True
        is_stone = False
    elif line.find('usemtl brick01') != -1:
        is_wood_3 = True
        is_stone = False
    elif line.find('usemtl floor_wood_3') != -1:
        is_wood_4 = True
        is_stone = False
    elif line.find('usemtl floor_wood_4') != -1:
        is_light2 = True
        is_stone = False    
    
    if is_stone:
        print line
        if line.find('f') != -1:
            f_stonef.write(line)
        if line.find('v ') != -1:
            f_stonev.write(line)
        if line.find('vt') != -1:
            f_stonevt.write(line)
    
