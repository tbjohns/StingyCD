import os

from distutils.core import setup


descr = 'TODO'

version = '0.1'

DISTNAME = 'stingycd'
DESCRIPTION = descr
MAINTAINER = 'T. B. Johnson'
MAINTAINER_EMAIL = 'TODO'
# LICENSE = 'BSD (3-clause)'
DOWNLOAD_URL = 'https://github.com/tbjohns/singycd.git'
VERSION = version

setup(name='stingycd',
      version=VERSION,
      description=DESCRIPTION,
      long_description=open('README.md').read(),
      #   license=LICENSE,
      maintainer=MAINTAINER,
      maintainer_email=MAINTAINER_EMAIL,
      # url=URL,
      download_url=DOWNLOAD_URL,
      packages=['./'],
      )
