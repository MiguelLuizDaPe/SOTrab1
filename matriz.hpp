#pragma once
#include <iostream>

#include "types.hpp"

template<typename T, usize N, usize M>
struct Mat {
	T data[N * M];

	constexpr
	usize height() const { return N; }

	constexpr
	usize width() const { return M; }

	// constexpr
	// Row row(usize idx) const {
	// 	Row r;
	// 	for(usize i = 0; i < M; i += 1){
	// 		r[i] = data[(idx * M) + i];
	// 	}
	// 	return r;
	// }

	// constexpr
	// Col col(usize idx) const {
	// 	Col c;
	// 	for(usize i = 0; i < N; i += 1){
	// 		c[i] = data[(i * M) + idx];
	// 	}
	// 	return c;
	// }

	constexpr
	T& at(usize row, usize col){
		return data[(row * M) + col];
	}

	constexpr
	const T& at(usize row, usize col) const {
		return data[(row * M) + col];
	}
};

template<usize N>
using Matriz = Mat<f64, N, N>;

//const& é pra não copiar a memória
template<usize N>
void imprimeMatriz(Matriz<N> const& ma){
    for(usize li = 0; li < N; li++){
        for(usize col = 0; col < N; col++){
            std::cout << ma.at(li, col) << " ";
        }
        std::cout << "\n";
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