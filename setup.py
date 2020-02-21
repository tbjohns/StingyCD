import os

from distutils.core import setup, Extension


source_list = ["src/%s" % f for f in os.listdir("src")
               if f.endswith("cpp")]

ext = Extension("stingycd.cd",
                include_dirs=["src"],
                extra_compile_args=['-O3'],
                sources=source_list)

descr = 'TODO'

version = '0.1'

DISTNAME = 'stingycd'
DESCRIPTION = descr
MAINTAINER = 'T. B. Johnson/Mathurin Massias'
MAINTAINER_EMAIL = 'TODO'
DOWNLOAD_URL = 'https://github.com/tbjohns/singycd.git'
VERSION = version

setup(name='stingycd',
      version=VERSION,
      description=DESCRIPTION,
      package_dir={"stingycd": "stingycd"},
      packages=["stingycd"],
      ext_modules=[ext],
      long_description=open('README.md').read(),
      maintainer=MAINTAINER,
      maintainer_email=MAINTAINER_EMAIL,
      download_url=DOWNLOAD_URL,
      )
