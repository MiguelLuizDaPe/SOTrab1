#include <array>
#include <bits/chrono.h>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
// #include <type_traits>

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

template <usize N> void bubbleSortMulti(std::array<usize, N> &a) {

  usize terco1 = N / 3;
  usize terco2 = terco1 * 2;

  auto p1 = std::thread([&]() {
    for (usize base = 0; base < terco1; base++) {
      for (usize bubble = 0; bubble < terco1 - base - 1; bubble++) {
        if (a[bubble] > a[bubble + 1]) {
          std::swap(a[bubble], a[bubble + 1]);
        }
      }
    }
  });

  auto p2 = std::thread([&]() {
    for (usize base = terco1; base < terco2; base++) {
      for (usize bubble = terco1; bubble < terco2 - base - 1; bubble++) {
        if (a[bubble] > a[bubble + 1]) {
          std::swap(a[bubble], a[bubble + 1]);
        }
      }
    }
  });

  for (usize base = terco2; base < N; base++) {
    for (usize bubble = terco2; bubble < N - base - 1; bubble++) {
      if (a[bubble] > a[bubble + 1]) {
        std::swap(a[bubble], a[bubble + 1]);
      }
    }
  }

  p1.join();
  p2.join();

  for (usize base = 0; base < N; base++) {
    for (usize bubble = 0; bubble < N - base - 1; bubble++) {
      if (a[bubble] > a[bubble + 1]) {
        std::swap(a[bubble], a[bubble + 1]);
      }
    }
  }
}

int main() {
  constexpr usize M = 300;
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
    *elapsedSecondsBubbleSingle = chrono::duration_cast<chrono::nanoseconds>(
        endBubbleSingle - startBubbleSingle);
  });

  const auto startBubbleMulti{chrono::steady_clock::now()};
  bubbleSortMulti(*b);
  const auto endBubbleMulti{chrono::steady_clock::now()};
  *elapsedSecondsBubbleMulti = chrono::duration_cast<chrono::nanoseconds>(
      endBubbleMulti - startBubbleMulti);

  t1.join();

  std::cout << "Bubble Single\n";
  // imprimirArray(*a);
  std::cout << elapsedSecondsBubbleSingle->count() << "\n";
  std::cout << "Bubble Multi\n";
  // imprimirArray(*b);
  std::cout << elapsedSecondsBubbleMulti->count() << "\n";
}