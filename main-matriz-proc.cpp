#include <bits/chrono.h>
#include <chrono>
#include <cstddef>
#include <ctime>
#include <iostream>
#include <memory>
#include <random>
#include <unistd.h>
// #include <thread>

#include "matriz.hpp"
#include "types.hpp"

namespace chrono = std::chrono;

template <usize N> 
void preencherMatriz(Matriz<N> &m){
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
Matriz<N> posMultiThr(Matriz<N> const &a, Matriz<N> const &b) {//usei duas thread só

  auto res = Matriz<N>{};
  usize tam = N * N;

  auto matData = res.data;//otimização, ao envez de ir por valor ir pra memória pelo endereço direto

  auto p1 = std::thread([&, matData]() {//essa thread vai até a metade da matriz
    for (usize i = 0; i < (N / 2); i += 1) {
      matData[i] = a.data[i] * b.data[i];
    }
  });

  for (usize i = N / 2; i < tam; i += 1) {//essa parte quem ta fazendo é a thread criada no main
    matData[i] = a.data[i] * b.data[i];
  }

  p1.join();

  return res;
}

template <usize N>
Matriz<N> matMultiThr(Matriz<N> const &a, Matriz<N> const &b) {

  auto res = Matriz<N>{};
  usize meio = N/2;

  auto p1 = std::thread([&]() {//essa thread vai até a metade da matriz
    for(usize li = 0; li < meio; li++){
        for(usize col = 0; col < N; col++){
            res.at(li, col) = a.at(li, col) * b.at(col, li);
        }
    }
  });

  for(usize li = meio; li < N; li++){//essa parte quem ta fazendo é a thread main
      for(usize col = 0; col < N; col++){
          res.at(li, col) = a.at(li, col) * b.at(col, li);
      }
  }

  p1.join();

  return res;
}

int main() {

  constexpr usize M = 5000;//tamanho da lateral da matriz

  Matriz<M> *a = mmap(NULL, sizeof(Matriz<M>), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
  Matriz<M> *b = mmap(NULL, sizeof(Matriz<M>), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);

  // auto a = std::make_unique<Matriz<M>>();//cria as matriz que vão ser acessadas
  // auto b = std::make_unique<Matriz<M>>();

  preencherMatriz(*a);//preenche elas
  preencherMatriz(*b);

  Matriz<M> multiPosSingle;//cria as matrizes as quais vão ser botadas os valores
  Matriz<M> multiPosMulti; 
  Matriz<M> multiMatSingle;
  Matriz<M> multiMatMulti;

  chrono::nanoseconds elapsedSecondsPosSingle;//os tempos totais de cada multiplicação
  chrono::nanoseconds elapsedSecondsPosMulti;
  chrono::nanoseconds elapsedSecondsMatSingle;
  chrono::nanoseconds elapsedSecondsMatMulti;

  pid_t pid = fork();

  //ta em uma thread e o outro não pq o multithread ta utilizando a thread do main(a thread do processo)
  //assim a thread do main não fica em repouso fazendo nada esperando as outras thread terminarem terminarem
  if(pid > 0) {
    const auto startPosSingle{chrono::steady_clock::now()};//começa a contar daqui
    multiPosSingle = multilpicaMatrizPosicional(*a, *b);//essa função ta vindo do matriz.hpp
    const auto endPosSingle{chrono::steady_clock::now()};//e vai até aqui, só pega o tempo de execução da função(não vou repetir pros outros)
    elapsedSecondsPosSingle = chrono::duration_cast<chrono::nanoseconds>(endPosSingle - startPosSingle);//calcula o tempo total
  }

  else{
    const auto startPosMulti{chrono::steady_clock::now()};
    multiPosMulti = posMultiThr(*a, *b);
    const auto endPosMulti{chrono::steady_clock::now()};
    elapsedSecondsPosMulti = chrono::duration_cast<chrono::nanoseconds>(endPosMulti - startPosMulti);
  }

//   t1.join();//espera a thread t1 terminar pra liberar o resto

  if(pid > 0){
    const auto startMatSingle{std::chrono::steady_clock::now()};
    *multiMatSingle = multilpicaMatrizMatricial(*a, *b);//essa função ta vindo do matriz.hpp
    const auto endMatSingle{std::chrono::steady_clock::now()};//escolhi nanosegundo pq em segundo e outros tipos de conversão ficam no estilo 1e-16 e é feio
    *elapsedSecondsMatSingle = chrono::duration_cast<chrono::nanoseconds>(endMatSingle - startMatSingle);
  }

  else{
    const auto startMatMulti{std::chrono::steady_clock::now()};
    *multiMatMulti = matMultiThr(*a, *b);
    const auto endMatMulti{std::chrono::steady_clock::now()};
    *elapsedSecondsMatMulti = chrono::duration_cast<chrono::nanoseconds>(endMatMulti - startMatMulti);
  }

  // t2.join();


  std::cout << "Posicional Sigle\n";
  // imprimeMatriz(*multiPosSingle);
  std::cout << elapsedSecondsPosSingle->count()<< "\n";

  std::cout << "Posicional Multi\n";//se o matriz for pequena a multithread fica uma porra pq custa criar thread, esse custo fica sombreado quando a matriz fica grandona
  // imprimeMatriz(*multiPosMulti);
  std::cout << elapsedSecondsPosMulti->count() << "\n";


  std::cout << "Matricial Sigle\n";
  // imprimeMatriz(*multiMatSingle);
  std::cout << elapsedSecondsMatSingle->count() << "\n";

  std::cout << "Matricial Multi\n";
  // imprimeMatriz(*multiMatMulti);
  std::cout << elapsedSecondsMatMulti->count() << "\n";

  return 0;
}