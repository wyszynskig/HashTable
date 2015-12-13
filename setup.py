from distutils.core import setup, Extension
setup(name="hashtable", version="0.2",
      ext_modules=[Extension("hashtable", ["hashtable-python.c"])])