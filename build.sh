#! /bin/bash

cwd=$(pwd)
cmake -S . -B ./build && cd build && make ; cd $cwd