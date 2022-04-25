#!/usr/bin/env python

import os
import MDSplus as mds

tree_name = 'pubsub'
os.environ[tree_name + '_path'] = os.path.abspath(os.getcwd())

T = mds.Tree(tree_name, -1, 'NEW')
T.addNode('COUNTER', 'SIGNAL')
T.addNode('CYCLETIME', 'SIGNAL')
T.write()
