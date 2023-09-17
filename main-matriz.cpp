#include <bits/chrono.h>
#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <thread>

#include "matriz.hpp"
#include "types.hpp"

namespace chrono = std::chrono;

template <usize N> void preencherMatriz(Matriz<N> &m){
  auto rd = std::random_device{};
  auto randDev = std::mt19937(rd()); // de onde ele pega o numero
  auto dist = std::uniform_real_distribution<>(-5.0, 5.0); // especialização dum gerador de distribuição uniforme

  for (usize li = 0; li < N; li++) {
    for (usize col = 0; col < N; col++) {
      m.at(li, col) = dist(randDev); // gera um numero;
    }
  }
}

template <usize N>
Matriz<N> posMultiThr(Matriz<N> const &a, Matriz<N> const &b) {

  auto res = Matriz<N>{}; // não sei se ta certo
  usize tam = N * N;

  auto matData = res.data;

  auto p1 = std::thread([&, matData]() {
    for (usize i = 0; i < (N / 2); i += 1) {
      matData[i] = a.data[i] * b.data[i];
    }
  });

  for (usize i = N / 2; i < tam; i += 1) {
    matData[i] = a.data[i] * b.data[i];
  }

  p1.join();

  return res;
}

template <usize N>
Matriz<N> matMultiThr(Matriz<N> const &a, Matriz<N> const &b) {

  auto res = Matriz<N>{}; // não sei se ta certo
  usize meio = N/2;

  auto p1 = std::thread([&]() {
    for(usize li = 0; li < meio; li++){
        for(usize col = 0; col < meio; col++){
            res.at(li, col) = a.at(li, col) * b.at(col, li);
        }
    }
  });

  for(usize li = meio; li < N; li++){
      for(usize col = meio; col < N; col++){
          res.at(li, col) = a.at(li, col) * b.at(col, li);
      }
  }

  p1.join();

  return res;
}

int main() {

  constexpr usize M = 5000;

  // botado no heap
  auto a = std::make_unique<Matriz<M>>();
  auto b = std::make_unique<Matriz<M>>();

  preencherMatriz(*a);
  preencherMatriz(*b);

  auto multiPosSingle = std::make_unique<Matriz<M>>();
  auto multiPosMulti = std::make_unique<Matriz<M>>(); // se mata no final do escopo
  auto multiMatSingle = std::make_unique<Matriz<M>>();
  auto multiMatMulti = std::make_unique<Matriz<M>>();

  auto elapsedSecondsPosSingle = std::make_unique<chrono::nanoseconds>();
  auto elapsedSecondsPosMulti = std::make_unique<chrono::nanoseconds>();
  auto elapsedSecondsMatSingle = std::make_unique<chrono::nanoseconds>();
  auto elapsedSecondsMatMulti = std::make_unique<chrono::nanoseconds>();

  auto t1 = std::thread([&]() {
    const auto startPosSingle{chrono::steady_clock::now()};
    *multiPosSingle = multilpicaMatrizPosicional(*a, *b);
    const auto endPosSingle{chrono::steady_clock::now()};
    *elapsedSecondsPosSingle = chrono::duration_cast<chrono::nanoseconds>(endPosSingle - startPosSingle);
  });

  const auto startPosMulti{chrono::steady_clock::now()};
  *multiPosMulti = posMultiThr(*a, *b);
  const auto endPosMulti{chrono::steady_clock::now()};
  *elapsedSecondsPosMulti = chrono::duration_cast<chrono::nanoseconds>(endPosMulti - startPosMulti);

  t1.join();


  auto t2 = std::thread([&](){
    const auto startMatSingle{std::chrono::steady_clock::now()};
    *multiMatSingle = multilpicaMatrizMatricial(*a, *b);
    const auto endMatSingle{std::chrono::steady_clock::now()};
    *elapsedSecondsMatSingle = chrono::duration_cast<chrono::nanoseconds>(endMatSingle - startMatSingle);
  });

  const auto startMatMulti{std::chrono::steady_clock::now()};
  *multiMatMulti = matMultiThr(*a, *b);
  const auto endMatMulti{std::chrono::steady_clock::now()};
  *elapsedSecondsMatMulti = chrono::duration_cast<chrono::nanoseconds>(endMatMulti - startMatMulti);

  t2.join();


  std::cout << "Posicional Sigle\n";
  imprimeMatriz(*multiPosSingle);
  std::cout << elapsedSecondsPosSingle->count() << "\n";

  std::cout << "Posicional Multi\n";
  imprimeMatriz(*multiPosMulti);
  std::cout << elapsedSecondsPosMulti->count() << "\n";


  std::cout << "Matricial Sigle\n";
  imprimeMatriz(*multiMatSingle);
  std::cout << elapsedSecondsMatSingle->count() << "\n";

  std::cout << "Matricial Multi\n";
  imprimeMatriz(*multiMatMulti);
  std::cout << elapsedSecondsMatMulti->count() << "\n";

  return 0;
}