from setuptools import setup, Extension
import os

# Read the README for long description
with open("README.md", "r", encoding="utf-8") as fh:
    long_description = fh.read()

# Define the C extension module
rbtree_module = Extension(
    'rbtree',
    sources=['src/rbtree_python.c', 'src/rbtree.c'],
    include_dirs=['src'],
    extra_compile_args=['-std=c17', '-Wall', '-Wextra'],
)

setup(
    name='py-red-black-tree',
    version='1.0.1',
    author='Vinícius Yuji Guima',
    author_email='viyugui@gmail.com',
    description='High-performance red-black tree implementation in C with Python bindings',
    long_description=long_description,
    long_description_content_type='text/markdown',
    url='https://github.com/viniyuji/py-rbtree',
    ext_modules=[rbtree_module],
    python_requires='>=3.7',
    classifiers=[
        'Development Status :: 4 - Beta',
        'Intended Audience :: Developers',
        'Topic :: Software Development :: Libraries :: Python Modules',
        'License :: OSI Approved :: MIT License',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.7',
        'Programming Language :: Python :: 3.8',
        'Programming Language :: Python :: 3.9',
        'Programming Language :: Python :: 3.10',
        'Programming Language :: Python :: 3.11',
        'Programming Language :: Python :: 3.12',
        'Programming Language :: C',
    ],
    keywords='rbtree red-black-tree data-structures tree binary-search-tree',
)
