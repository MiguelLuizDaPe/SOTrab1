#!/usr/bin/env sh

cc="clang++"
cflags="-std=c++17 -pipe -O2 -Wall -Wextra -fno-strict-aliasing -L. -I. -pthread"

#2> /dev/null é pra enganar o compilador pra acreditar que vamo usar a matriz que ele acha que a gente não vai usar
# $cc $cflags  main-single-thread.cpp -o main-st.bin && ./main-st.bin 2> /dev/null
$cc $cflags main-multi-thread.cpp -o main-mt.bin && ./main-mt.bin 2> /dev/null 