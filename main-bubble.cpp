#include <array>
#include <bits/chrono.h>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include <type_traits>

#include "types.hpp"

namespace chrono = std::chrono;


template <usize N> void preencherArray(std::array<usize, N> &a) {
  for (usize i = 0; i < N; i++) {
    a[i] = N - i;
  }
}

template <usize N> void imprimirArray(std::array<usize, N> &a) {
  for (usize i = 0; i < N; i++) {
    std::cout << a[i] << "\n";
  }
}

template <usize N> void bubbleSortSigle(std::array<usize, N> &a) {
  for (usize base = 0; base < N; base++) {
    for (usize bubble = 0; bubble < N - base - 1; bubble++) {
      if (a[bubble] > a[bubble + 1]) {
        std::swap(a[bubble], a[bubble + 1]);
      }
    }
  }
}

int main() {
  constexpr usize M = 5;
  auto a = std::make_unique<std::array<usize, M>>();
  auto b = std::make_unique<std::array<usize, M>>();

  preencherArray(*a);
  preencherArray(*b);
  // imprimirArray(*a);

  auto elapsedSecondsBubbleSingle = std::make_unique<chrono::nanoseconds>();
  auto elapsedSecondsBubbleMulti = std::make_unique<chrono::nanoseconds>();

  auto t1 = std::thread([&]() { 
    const auto startBubbleSingle{chrono::steady_clock::now()};
    bubbleSortSigle(*a);
    const auto endBubbleSingle{chrono::steady_clock::now()};
    *elapsedSecondsBubbleSingle = chrono::duration_cast<chrono::nanoseconds>(endBubbleSingle - startBubbleSingle);
  });

  

  // imprimirArray(*a);
}