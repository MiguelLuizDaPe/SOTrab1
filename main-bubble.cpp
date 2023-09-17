#include <array>
#include <bits/chrono.h>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
// #include <type_traits>

#include "types.hpp"

namespace chrono = std::chrono;

template <usize N> void preencherArray(usize *a) {
  for (usize i = 0; i < N; i++) {
    a[i] = N - i;
  }
}

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

  usize terco1 = N / 3;
  usize terco2 = terco1 * 2;

  auto p1 = std::thread([=]() {
    for (usize base = 0; base < terco1; base++) {
      for (usize bubble = 0; bubble < terco1 - base - 1; bubble++) {
        if (buf[bubble] > buf[bubble + 1]) {
          std::swap(buf[bubble], buf[bubble + 1]);
        }
      }
    }
  });

  auto p2 = std::thread([=]() {
    for (usize base = terco1; base < terco2; base++) {
      for (usize bubble = terco1; bubble < terco2 - base - 1; bubble++) {
        if (buf[bubble] > buf[bubble + 1]) {
          std::swap(buf[bubble], buf[bubble + 1]);
        }
      }
    }
  });

  for (usize base = terco2; base < N; base++) {
    for (usize bubble = terco2; bubble < N - base - 1; bubble++) {
      if (buf[bubble] > buf[bubble + 1]) {
        std::swap(buf[bubble], buf[bubble + 1]);
      }
    }
  }

  p1.join();
  p2.join();

  for (usize base = 0; base < N; base++) {
    for (usize bubble = 0; bubble < N - base - 1; bubble++) {
      if (buf[bubble] > buf[bubble + 1]) {
        std::swap(buf[bubble], buf[bubble + 1]);
      }
    }
  }
}

int main(int argc, char **argv) {
  if (argc < 2)
    return 1;
  char mode = argv[1][0];

  constexpr usize M = 3000;
  auto a = std::unique_ptr<usize[]>(new usize[M]);
  auto b = std::unique_ptr<usize[]>(new usize[M]);

  preencherArray<M>(a.get());
  preencherArray<M>(b.get());

  // imprimirArray(*a);

  auto elapsedSeconds = std::make_unique<chrono::nanoseconds>();

  // auto t1 = std::thread([&]() {
  if (mode == 's') {
    const auto start{chrono::steady_clock::now()};
    bubbleSortSigle<M>(a.get());
    const auto end{chrono::steady_clock::now()};
    *elapsedSeconds = chrono::duration_cast<chrono::nanoseconds>(end - start);
    std::cout << "Bubble Single\n";
    // imprimirArray(*a);
    std::cout << elapsedSeconds->count() << "\n";
  } else if (mode == 'm') {
    const auto start{chrono::steady_clock::now()};
    bubbleSortMulti<M>(b.get());
    const auto end{chrono::steady_clock::now()};
    *elapsedSeconds = chrono::duration_cast<chrono::nanoseconds>(end - start);
    std::cout << "Bubble Multi\n";
    // imprimirArray(*b);
    std::cout << elapsedSeconds->count() << "\n";
  }

  // });

  // t1.join();
}