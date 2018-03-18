#!/bin/python

import pdb
from _lights import *
import sys
if sys.version_info[0] == 3: # python 3
    from configparser import ConfigParser
else: # python 2
    from ConfigParser import ConfigParser

def load_file(filename):
    # pdb.set_trace()
    config = ConfigParser()
    parsedFiles = config.read(filename)
    if not filename in parsedFiles:
        raise ValueError('Could not parse files: %s' % 
                         repr(filenameSet - parsedFilesSet))

    if not 'greyscale' in config.sections():
        raise ValueError("The section 'greyscale' must be present in *.ini file.")

    greyscaleItems = config.items('greyscale')
    for item in greyscaleItems:
        t = eval(item[1])
        if type(t) is tuple:
            t = list(t)
        elif not type(t) is list:
            raise ValueError("The value for '%s' in config file %s is not tuple or list" % 
                             (t[0], repr(filename)))
        load_vals(t)
