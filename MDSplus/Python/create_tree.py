#!/usr/bin/env python

from MDSplus import *
from numpy import *

os.environ['mytree_path'] = '/home/marte2/workspace/Trees/mytree'

t = Tree('mytree', -1, 'NEW')

t.addNode("NUM1", "NUMERIC")
t.addNode("SIG1", "SIGNAL")
t.addNode("TXT1", "TEXT")
t.addNode("SUB2", "STRUCTURE")
t.addNode("SUB2:NUM2", "NUMERIC")
t.addNode("SUB2:SIG2", "SIGNAL")
t.addNode("SUB2:TXT2", "TEXT")
t.addNode("SUB2:SUB3", "STRUCTURE")
t.addNode("SUB2:SUB3:NUM3", "NUMERIC")
t.addNode("SUB2:SUB3:SIG3", "SIGNAL")
t.addNode("SUB2:SUB3:TXT3", "TEXT")

t.write()
