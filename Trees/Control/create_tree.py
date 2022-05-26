#!/usr/bin/env python

import os
import MDSplus as mds

tree_name = 'control'
os.environ[tree_name + '_path'] = os.path.abspath(os.getcwd())

T = mds.Tree(tree_name, -1, 'NEW')
T.addNode('REFERENCE', 'SIGNAL')
T.addNode('ERROR', 'SIGNAL')
T.addNode('INPUT', 'SIGNAL')
T.addNode('OUTPUT', 'SIGNAL')
T.write()

