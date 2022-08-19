# Overview

A C++ implementation of a chess engine using modern chess program techniques

# Demo

![](demos/examplegame.gif)

# Dependencies

- wxWidgets

# Installation fom Source
To build the engine, gui, and tests, run
```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=clang++ ../
make
```
Currently, the search algorithm completes in a reasonable time in clang, and the GUI program 
only runs when built in release mode. LTO is enabled by default for the best runtime results

# TODO
- [] UCI-compatible CLI
- [] Implement command-line options (eg. Hash Table Size)
- [] Additional search techniques (eg. Aspiration Windows)
- [] Fixing compatibility issues between compilers and build types
- [] Implementing Multithreading in both GUI and CLI