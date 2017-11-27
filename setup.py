#!/bin/python

from distutils.core import setup, Extension

lights_module = Extension('lights', sources = ['lights.c'])
setup(name = 'Lights',
      version = '1.0',
      description = 'This is lights package',
      ext_modules = [lights_module])
