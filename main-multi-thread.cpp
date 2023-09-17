#include <bits/chrono.h>
#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <thread>

#include "matriz.hpp"
#include "types.hpp"

namespace chrono = std::chrono;

template <usize N> void preencherMatriz(Matriz<N> &m) {
  auto rd = std::random_device{};
  auto randDev = std::mt19937(rd()); // de onde ele pega o numero
  auto dist = std::uniform_real_distribution<>(
      -5.0, 5.0); // especialização dum gerador de distribuição uniforme

  for (usize li = 0; li < N; li++) {
    for (usize col = 0; col < N; col++) {
      m.at(li, col) = dist(randDev); // gera um numero;
    }
  }
}

template <usize N>
Matriz<N> posMultiThr(Matriz<N> const &a, Matriz<N> const &b) {
  // auto workers = std::vector<std::thread>();

  auto res = Matriz<N>{}; // não sei se ta certo
  usize tam = N * N;

  auto matData = res.data;
  // auto aData = a.data;
  // auto bData = b.data;

  auto p1 = std::thread([&, matData]() {
    for (usize i = 0; i < (N / 2); i += 1) {
      matData[i] = a.data[i] * b.data[i];
    }
  });

  // auto p2 = std::thread([&]() {
    for (usize i = N / 2; i < tam; i += 1) {
      matData[i] = a.data[i] * b.data[i];
    }
  // });

  p1.join();
  // p2.join();

  return res;
}

// template<usize N>
// Matriz<N> posMultiThr(Matriz<N> const& a, Matriz<N> const& b){
//     auto workers = std::vector<std::thread>();

//     auto res = Matriz<N>{};
//     auto buf = new Matriz<N>;

//     auto mulRows = [](Matriz<N> *dest, int rowIdx, Matriz<N> const& a,
//     Matriz<N> const& b){
//         for(usize i = 0; i < N; i += 1){
//             dest->at(rowIdx, i) = a.at(rowIdx, i) * b.at(rowIdx, i);
//         }
//     };

//     for(int i = 0; i < N; i += 1){
//         // Don't pass everything by ref otherwhise shared memory chokes a lot
//         workers.push_back(std::thread([=, &a, &b](){ mulRows(buf, i, a, b);
//         }));
//     }

//     for(int i = 0; i < N; i += 1){ workers[i].join(); }

//     res = *buf; // Copy back from shared buf

//     delete buf;

//     return res;
// }

// template<usize N>
// Matriz<N> matMulti(Matriz<N> *a, Matriz<N> *b, Matriz<N> *c){
//     std::vector<std::thread> threads;

//     for(int i = 0; i < N; i += 1){
//         threads.push_back(std::thread([&](){
//             matThread(*a, *b, *c, N, N);
//         }));
//     }

//     for (auto& t : threads){
//     t.join();
//     }
// }

int main() {

  constexpr usize M = 3000;

  // botado no heap
  auto a = std::make_unique<Matriz<M>>();

  auto b = std::make_unique<Matriz<M>>();

  preencherMatriz(*a);
  preencherMatriz(*b);

  auto multiPosSingle = std::make_unique<Matriz<M>>();
  auto multiPosMulti =
      std::make_unique<Matriz<M>>(); // se mata no final do escopo
  auto multiMat = std::make_unique<Matriz<M>>();
  auto elapsedSecondsPosSingle = std::make_unique<chrono::nanoseconds>();
  auto elapsedSecondsPosMulti = std::make_unique<chrono::nanoseconds>();
  auto elapsedSecondsMat = std::make_unique<chrono::nanoseconds>();

  auto t1 = std::thread([&]() {
    const auto startPosSingle{std::chrono::steady_clock::now()};
    *multiPosSingle = multilpicaMatrizPosicional(*a, *b);
    const auto endPosSingle{std::chrono::steady_clock::now()};
    *elapsedSecondsPosSingle = chrono::duration_cast<chrono::nanoseconds>(
        endPosSingle - startPosSingle);
  });

  // auto m1 = std::thread([&](){
  const auto startPosMulti{std::chrono::steady_clock::now()};
  *multiPosMulti = posMultiThr(*a, *b);
  const auto endPosMulti{std::chrono::steady_clock::now()};
  *elapsedSecondsPosMulti =
      chrono::duration_cast<chrono::nanoseconds>(endPosMulti - startPosMulti);
  // });

  // auto t2 = std::thread([&](){
  // const auto startMat{std::chrono::steady_clock::now()};
  // *multiMat = multilpicaMatrizMatricial(*a, *b);
  // const auto endMat{std::chrono::steady_clock::now()};
  // *elapsedSecondsMat = chrono::duration_cast<chrono::nanoseconds>(endMat -
  // startMat);
  // });

  t1.join();
  // m1.join();
  // t2.join();

  std::cout << "Posicional Sigle\n";
  imprimeMatriz(*multiPosSingle);
  std::cout << elapsedSecondsPosSingle->count() << "\n";
  std::cout << "Posicional Multi\n";
  imprimeMatriz(*multiPosMulti);
  std::cout << elapsedSecondsPosMulti->count() << "\n";

  // std::cout << "Matricial Sigle\n";
  // imprimeMatriz(*multiMat);
  // std::cout << elapsedSecondsMat->count() << "\n";

  return 0;
}