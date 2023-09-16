#!/usr/bin/env sh

cc="clang++"
cflags="-std=c++17 -pipe -O2 -Wall -Wextra -fno-strict-aliasing -L. -I. -pthread"

# $cc $cflags  main-single-thread.cpp -o main-st.bin && ./main-st.bin
$cc $cflags main-multi-thread.cpp -o main-mt.bin && ./main-mt.bin