#!/usr/bin/env sh

cc="clang++"
cflags="-std=c++17 -pipe -O2 -Wall -Wextra -fno-strict-aliasing -L. -I. -pthread"

#2> /dev/null é pra não ler o que o imprimirMatriz ta imprimindo
# $cc $cflags  main-single-thread.cpp -o main-st.bin && ./main-st.bin 2> /dev/null
$cc $cflags main-matriz.cpp -o main-mt.bin && ./main-mt.bin 2> /dev/null