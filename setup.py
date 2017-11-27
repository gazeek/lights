#!/bin/python

from distutils.core import setup, Extension
from shutil import copyfile


lights_module = Extension('lights',
						  libraries = ['wiringPi', 'pthread'],
						  sources = ['lights.c'])
setup(name = 'Lights',
      version = '1.0',
      description = 'This is lights package',
      ext_modules = [lights_module])

copyfile("build/lib.linux-armv6l-2.7/lights.so", "lights.so")
