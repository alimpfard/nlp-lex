from distutils.core import setup
setup(
  name = 'nlex',
  packages = ['nlex', 'nlex.wrap'],
  version = '0.0.3',
  license='bsd-2-clause',
  description = 'nlex remote compiler wrapper',
  author = 'AnotherTest',
  author_email = 'devanothertest@gmail.com',
  url = 'https://github.com/alimpfard/nlp-lex',
  download_url = 'https://github.com/alimpfard/nlp-lex/archive/pywrapper-0.0.3.tar.gz',
  keywords = [],
  install_requires=[
          'requests',
      ],
  classifiers=[
    'Development Status :: 3 - Alpha',
    'Intended Audience :: Developers',
    'Topic :: Software Development :: Build Tools',
    'License :: OSI Approved :: BSD License',
    'Programming Language :: Python :: 3',
  ],
)
