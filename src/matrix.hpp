#include <cstddef>

#ifndef SRC_MATRIX_H_
#define SRC_MATRIX_H_

#ifndef uint
#define uint unsigned int
#endif

using namespace std;

template<uint M, uint N, typename T>
class Matrix {

private:
	T _values[M][N];

public:
	Matrix(){}
	~Matrix(){}

	T valueOn(uint row, uint col) {
		T value;

		if (row < M && col < N)
			value = _values[row][col];

		return value;
	}

	void setValueOn(uint row, uint col, T value) {
		//if (row < M && col < N)
			_values[row][col] = value;
	}

	static Matrix<M, M, T> buildIdentity() {
		Matrix<M,M, T> matrix;

		for (int row = 0; row < M; ++row) {
			for (int col = 0; col < M; ++col) {
				matrix.setValueOn(row, col, 0);
			}
		}

		for(int diagonal = 0; diagonal < M; diagonal++) {
			matrix.setValueOn(diagonal, diagonal, 1);
		}

		return matrix;
	}

	Matrix<M, N, T> operator+(Matrix<M, N, T> other) {
		Matrix<M,N, T> sum;

		for (int row = 0; row < M; ++row) {
			for (int col = 0; col < N; ++col) {
				sum._values[row][col] = _values[row][col] + other._values[row][col];
			}
		}

		return sum;
	}

	template<uint N1>
	Matrix<M, N1, T> operator*(Matrix<N, N1, T> other) {
		Matrix<M,N1, T> mult;
		double value;

		for (uint row = 0; row < M; ++row) {
			for (uint col = 0; col < N1; ++col) {
				value = 0;

				for (uint n = 0; n < N; ++n) {
					value += _values[row][n] * other.valueOn(n, col);
				}

				mult.setValueOn(row, col, value);
			}
		}

		return mult;
	}

	template<uint N1, typename T2>
	Matrix<M, N1, T2> operator*(Matrix<N, N1, T2> other) {
		Matrix<M,N1, T2> mult;

		for (uint row = 0; row < M; ++row) {
			for (uint col = 0; col < N1; ++col) {
				T2 value;

				for (uint n = 0; n < N; ++n) {
					value += _values[row][n] * other.valueOn(n, col);
				}

				mult.setValueOn(row, col, value);
			}
		}

		return mult;
	}

	friend Matrix<M, N, T> operator*(Matrix<M,N,T> matrix, double scalar) {
		Matrix<M,N, T> mult;

		for (int row = 0; row < M; ++row) {
			for (int col = 0; col < N; ++col) {
				mult._values[row][col] = matrix._values[row][col] * scalar;
			}
		}

		return mult;
	}

	friend Matrix<M, N, T> operator*(double scalar, Matrix<M,N,T> matrix) {
		Matrix<M,N, T> mult;

		for (int row = 0; row < M; ++row) {
			for (int col = 0; col < N; ++col) {
				mult._values[row][col] = matrix._values[row][col] * scalar;
			}
		}

		return mult;
	}
};



#endif /* SRC_MATRIX_H_ */
