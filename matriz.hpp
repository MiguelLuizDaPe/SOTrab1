#pragma once
#include <iostream>
#include <utility>

#include "types.hpp"

template<typename T, usize N, usize M>
struct Mat {
	T* data;

    constexpr
    usize height() const { return N; }

    constexpr
    usize width() const { return M; }

    constexpr
    T& at(usize row, usize col){
        return data[(row * M) + col];
    }

    constexpr
    const T& at(usize row, usize col) const {
        return data[(row * M) + col];
    }

	Mat(){
        data = new T[N * M];
    }

	Mat(Mat&& m){
		data = std::exchange(m.data, nullptr);
	}

	void operator=(Mat&& m){
		delete[] data;
		data = std::exchange(m.data, nullptr);
	}

    ~Mat(){
        delete[] data;
    }
};

template<usize N>
using Matriz = Mat<f64, N, N>;

//const& é pra não copiar a memória
template<usize N>
void imprimeMatriz(Matriz<N> const& ma){
    for(usize li = 0; li < N; li++){
        for(usize col = 0; col < N; col++){
            std::cerr << ma.at(li, col) << " ";
        }
        std::cerr << "\n";
    }
}

template<usize N>
Matriz<N> multilpicaMatrizPosicional(Matriz<N> const& a, Matriz<N> const& b){
    Matriz<N> n;
    for(usize li = 0; li < N; li++){
        for(usize col = 0; col < N; col++){
            n.at(li, col) = a.at(li, col) * b.at(li, col);
        }
    }
    return n;
}

template<usize N>
Matriz<N> multilpicaMatrizMatricial(Matriz<N> const& a, Matriz<N> const& b){
    Matriz<N> n;
    for(usize li = 0; li < N; li++){
        for(usize col = 0; col < N; col++){
            n.at(li, col) = a.at(li, col) * b.at(col, li);
        }
    }
    return n;
}