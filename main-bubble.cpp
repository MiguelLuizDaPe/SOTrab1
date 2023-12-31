#include <array>
#include <bits/chrono.h>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

#include "types.hpp"

namespace chrono = std::chrono;

template <usize N> void preencherArray(usize *a) {
  for (usize i = 0; i < N; i++) {
    a[i] = N - i;
  }
}

// void swap(usize *a, usize *b){
//   usize *c;
//   c = a;
//   a = b;
//   b = c;
// }

template <usize N> void imprimirArray(usize *a) {
  for (usize i = 0; i < N; i++) {
    std::cout << a[i] << "\n";
  }
}

template <usize N> void bubbleSortSigle(usize *a) {
  for (usize base = 0; base < N; base++) {
    for (usize bubble = 0; bubble < N - base - 1; bubble++) {
      if (a[bubble] > a[bubble + 1]) {
        std::swap(a[bubble], a[bubble + 1]);
      }
    }
  }
}

template <usize N> void bubbleSortMulti(usize *buf) {

  usize terco1 = N / 3;//define onde acaba o primeiro terço pelo indice do array
  usize terco2 = terco1 * 2;//define onde acaba o segundo terço
  // std::cout << terco1 << terco2 << N << " fora\n";


  auto p1 = std::thread([&]() {//thread do primeiro terço
    for (usize base = 0; base < terco1; base++) {
      for (usize bubble = 0; bubble < terco1 - 1; bubble++) {
        if (buf[bubble] > buf[bubble + 1]) {
          std::swap(buf[bubble], buf[bubble + 1]);
        }
      }
    }
  // std::cout << terco1 << terco2 << N << " dentro1\n";

  });

  auto p2 = std::thread([&]() {//thread do segundo terço
    for (usize base = terco1; base < terco2; base++) {
      for (usize bubble = terco1; bubble < terco2 - 1; bubble++) {
        if (buf[bubble] > buf[bubble + 1]) {
          std::swap(buf[bubble], buf[bubble + 1]);
        }
      }
    }
    // imprimirArray<N>(buf);
  // std::cout << terco1 << terco2 << N << " dentro2\n";

  });

  for (usize base = terco2; base < N; base++) {//thread do último terço(terceiro)
    for (usize bubble = terco2; bubble < N - 1; bubble++) {
      if (buf[bubble] > buf[bubble + 1]) {
        std::swap(buf[bubble], buf[bubble + 1]);
      }
    }
  }
  // std::cout << terco1 << terco2 << N << " fora2\n";


  p1.join();//espera as thread terminar
  p2.join();

  for (usize base = 0; base < N; base++) {//ordena todos os terços, mas como é bubble não tem muito ganho disso
    for (usize bubble = 0; bubble < N - base - 1; bubble++) {
      if (buf[bubble] > buf[bubble + 1]) {
        std::swap(buf[bubble], buf[bubble + 1]);
      }
    }
  }
}

int main(int argc, char **argv) {
  if (argc < 2)//bagulhinho pra testar single ou multi thread um por cada rodada
    return 1;
  char mode = argv[1][0];

  constexpr usize M = 300;
  auto a = std::unique_ptr<usize[]>(new usize[M]);
  auto b = std::unique_ptr<usize[]>(new usize[M]);

  preencherArray<M>(a.get());
  preencherArray<M>(b.get());

  auto elapsedSeconds = std::make_unique<chrono::nanoseconds>();//cria só um pq só sentra no if ou no else if

  if (mode == 's') {
    // imprimirArray<M>(a.get());
    const auto start{chrono::steady_clock::now()};
    bubbleSortSigle<M>(a.get());
    const auto end{chrono::steady_clock::now()};
    *elapsedSeconds = chrono::duration_cast<chrono::nanoseconds>(end - start);
    std::cout << "Bubble Single\n";
    // imprimirArray<M>(a.get());
    std::cout << elapsedSeconds->count() << "\n";
  } else if (mode == 'm') {
    // imprimirArray<M>(b.get());
    const auto start{chrono::steady_clock::now()};
    bubbleSortMulti<M>(b.get());
    const auto end{chrono::steady_clock::now()};
    *elapsedSeconds = chrono::duration_cast<chrono::nanoseconds>(end - start);
    std::cout << "Bubble Multi\n";
    // imprimirArray<M>(b.get());
    std::cout << elapsedSeconds->count() << "\n";
  }
}