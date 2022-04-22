#!/usr/bin/env python

import os
import MDSplus as mds

tree_name = 'log'
tree_path = '/home/marte2/workspace/Trees/Log/'
os.environ[tree_name + '_path'] = tree_path

T = mds.Tree(tree_name, -1, 'NEW')
T.addNode('COUNTER', 'SIGNAL')
T.addNode('CYCLETIME', 'SIGNAL')
T.write()
