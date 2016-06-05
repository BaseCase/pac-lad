#!/bin/bash

set -e

clang -lSDL2 -lc++ main.cpp

./a.out
