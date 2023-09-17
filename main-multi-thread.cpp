#include <bits/chrono.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <random>

#include "types.hpp"
#include "matriz.hpp"

namespace chrono = std::chrono;

template<usize N>
void preencherMatriz(Matriz<N> &m){
    auto rd = std::random_device{};
    auto randDev = std::mt19937(rd());//de onde ele pega o numero
    auto dist = std::uniform_real_distribution<>(-5.0, 5.0);//especialização dum gerador de distribuição uniforme
    
    for(usize li = 0; li < N; li++){
        for(usize col = 0; col < N; col++){
            m.at(li, col) = dist(randDev); // gera um numero;
        }
    }
}

template<usize N>
Matriz<N> matMultiThr(Matriz<N> *a, Matriz<N> *b){
    std::vector<std::thread> threads;

    
    
    for(int i = 0; i < N; i += 1){
        threads.push_back(std::thread(/* arggs da thread */));
    }
}

int main(){

    constexpr usize M = 1000;

    //botado no heap
    auto a = new Matriz<M>;

    auto b = new Matriz<M>;

    preencherMatriz(*a);
    preencherMatriz(*b);


    auto multiPos = new Matriz<M>;
    auto multiMat = new Matriz<M>;
    auto elapsedSecondsPos = new chrono::nanoseconds;
    auto elapsedSecondsMat = new chrono::nanoseconds;

    auto t1 = std::thread([&](){
        const auto startPos{std::chrono::steady_clock::now()};
        *multiPos = multilpicaMatrizPosicional(*a, *b);
        const auto endPos{std::chrono::steady_clock::now()};
        *elapsedSecondsPos = chrono::duration_cast<chrono::nanoseconds>(endPos - startPos);
    });

    // auto t2 = std::thread([&](){
        const auto startMat{std::chrono::steady_clock::now()};
        *multiMat = multilpicaMatrizMatricial(*a, *b);
        const auto endMat{std::chrono::steady_clock::now()};
        *elapsedSecondsMat = chrono::duration_cast<chrono::nanoseconds>(endMat - startMat);
    // });

    t1.join();
    // t2.join();
    



    std::cout << "Posicional Sigle\n";
    imprimeMatriz(*multiPos);
    std::cout << elapsedSecondsPos->count() << "\n";
    std::cout << "Matricial Sigle\n";
    imprimeMatriz(*multiMat);
    std::cout << elapsedSecondsMat->count() << "\n";

    delete a;
    delete b;
    delete multiPos;
    delete multiMat;

    return 0;
}