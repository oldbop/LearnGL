#!/bin/bash
cmake -DCMAKE_C_COMPILER=clang \
-DCMAKE_CXX_COMPILER=clang++ \
-DSYS_GL_HEADERS=yes \
-S . -B build/
