#!/usr/bin/env sh

cc="clang++"
cflags="-std=c++17 -pipe -O2 -Wall -Wextra -fno-strict-aliasing -L. -I. -pthread"

#2> /dev/null é pra não ler o que o imprimirMatriz ta imprimindo pra fingir que ta usando o matriz pra não da uma otimização que faz o programa dar errado
$cc $cflags  main-bubble.cpp -o main-bb.bin && ./main-bb.bin $@ 2> /dev/null
# $cc $cflags main-matriz.cpp -o main-mt.bin && ./main-mt.bin 2> /dev/null
# $cc $cflags main-matriz-proc.cpp -o main-mt-proc.bin && ./main-mt-proc.bin 2> /dev/null