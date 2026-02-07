.PHONY: help build test clean install dev all

PYTHON = python3

all: build

help:
	@echo "Available targets:"
	@echo "  make build       - Build the C extension module"
	@echo "  make test        - Run the test suite"
	@echo "  make clean       - Remove build artifacts"
	@echo "  make install     - Install the package"
	@echo "  make dev         - Build and test in one step"
	@echo "  make rebuild     - Clean and rebuild from scratch"
	@echo "  make dist        - Build distribution packages for PyPI"
	@echo "  make upload-test - Upload to TestPyPI"
	@echo "  make upload      - Upload to PyPI (production)"

build:
	$(PYTHON) setup.py build_ext --inplace

test: build
	$(PYTHON) test_rbtree.py

clean:
	rm -rf build/
	rm -rf dist/
	rm -rf *.egg-info/
	rm -f *.so
	rm -f *.o
	rm -rf __pycache__/
	find . -type f -name '*.pyc' -delete
	find . -type d -name '__pycache__' -delete

install:
	$(PYTHON) setup.py install

dev: build test

rebuild: clean build

# PyPI publishing
dist:
	python3 -m build

check-dist: dist
	python3 -m twine check dist/*

upload-test: dist
	python3 -m twine upload --repository testpypi dist/*

upload: dist
	python3 -m twine upload dist/*
