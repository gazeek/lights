#!/bin/python

from distutils.core import setup, Extension
from shutil import copyfile


lights_module = Extension('_lights',
						  libraries = ['wiringPi', 'pthread'],
                          include_dirs = ['queue'],
						  sources = ['lights.c', 'queue/queue.c'])
setup(name = 'Lights Internal',
      version = '0.1',
      description = 'This is the internal C functions for the lights package',
      ext_modules = [lights_module],
      py_modules = ['lights'])

