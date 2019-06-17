OS=$(shell uname)

all: build

build:
	cd src && qmake-qt5 -makefile -o qmakefile
	cd src && make -f qmakefile CONFIG+=Release

run: build
	./src/chess

clean:
	cd src && make -f qmakefile clean
	rm -rf doc

doxygen:
	doxygen
