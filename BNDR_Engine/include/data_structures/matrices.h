/*MIT License

Copyright (c) 2021 Caleb Christopher Bender

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#pragma once
#include <pch.h>
#include "vectors.h"

namespace bndr {

	// bndr::BaseMatrix
	// Description: This class is the base class for all matrices. Matrices are primarily meant to translate, rotate,
	// and scale a bndr vector in a shader program
	template <class T>
	class BaseMatrix {

	protected:

		T* data;
		int nRows;
		int nCols;
		inline const T& operator[](int index) const { return BaseMatrix<T>::data[index]; }
	public:

		inline T getAt(int row, int col) const { return BaseMatrix<T>::data[row * nRows + col]; }
		inline T* getData() { return data; }
		~BaseMatrix();
	};

	template <class T>
	BaseMatrix<T>::~BaseMatrix() {

		delete[] data;
	}


	template <class T>
	class Mat3x3 : public BaseMatrix<T> {

		using base = typename BaseMatrix<T>;
		
	public:

		// constructors/assignment
		Mat3x3() : base(), base::nRows(3), base::nCols(3) {}
		// take in arguments from a vector
		Mat3x3(std::initializer_list<T>&& matData);
		// copy constructor
		Mat3x3(const Mat3x3<T>& mat);
		// move constructor
		Mat3x3(Mat3x3<T>&& mat);
		// assignment operator
		void operator=(const Mat3x3<T>& mat);

		// matrix operations

		// add two matrices
		Mat3x3<T> operator+(const Mat3x3<T>& mat);
		// subtract two matrices
		Mat3x3<T> operator-(const Mat3x3<T>& mat);
		// matrix multiplication
		Mat3x3<T> operator*(const Mat3x3<T>& mat);
		// matrix vector multiplication
		Vec3<T> operator*(const Vec3<T>& vec);
		// add in place
		void operator+=(const Mat3x3<T>& mat);
		// subtract in place
		void operator-=(const Mat3x3<T>& mat);
		// multiply in place
		void operator*=(const Mat3x3<T>& mat);

		// matrix templates allocated on stack

		// get a translation matrix
		static Mat3x3<T> TransMat(const T& xTrans, const T& yTrans) {
		
			return {

				(T)1.0f, (T)0.0f, (T)xTrans,
				(T)0.0f, (T)1.0f, (T)yTrans,
				(T)0.0f, (T)0.0f, (T)1.0f

			};
		}

		// get a rotational matrix
		// theta is in degrees
		static Mat3x3<T> RotMat(const T& theta) {

			// convert to radians
			T rad = theta * static_cast<T>(BNDR_PI / 180.0f);
			return {

				(T)cosf((float)rad), (T)-sinf((float)rad), (T)0.0f,
				(T)sinf((float)rad), (T)cosf((float)rad), (T)0.0f,
				(T)1.0f, (T)1.0f, (T)0.0f
			};
		}

		// get a scale matrix
		static Mat3x3<T> ScaleMat(const T& xScale, const T& yScale) {

			return {
			
				xScale, (T)0.0f, (T)0.0f,
				(T)0.0f, yScale, (T)0.0f,
				(T)0.0f, (T)0.0f, (T)1.0f,
			};

		}

		// matrix templates allocated on heap

		// get a translation matrix
		static Mat3x3<T>* HeapTransMat(const T& xTrans, const T& yTrans) {

			return new Mat3x3({

				(T)1.0f, (T)0.0f, (T)xTrans,
				(T)0.0f, (T)1.0f, (T)yTrans,
				(T)0.0f, (T)0.0f, (T)1.0f

			});
		}

		// get a rotational matrix
		// theta is in degrees
		static Mat3x3<T>* HeapRotMat(const T& theta) {

			// convert to radians
			T rad = theta * static_cast<T>(BNDR_PI / 180.0f);
			return new Mat3x3({

				(T)cosf((float)rad), (T)-sinf((float)rad), (T)0.0f,
				(T)sinf((float)rad), (T)cosf((float)rad), (T)0.0f,
				(T)1.0f, (T)1.0f, (T)0.0f
			});
		}

		// get a scale matrix
		static Mat3x3<T>* HeapScaleMat(const T& xScale, const T& yScale) {

			return new Mat3x3({

				xScale, (T)0.0f, (T)0.0f,
				(T)0.0f, yScale, (T)0.0f,
				(T)0.0f, (T)0.0f, (T)1.0f,
			});

		}

	};

	template <class T>
	Mat3x3<T>::Mat3x3(std::initializer_list<T>&& matData) {
	
		// raise an exception if the vector size does not match 9 (3x3)
		if (matData.size() != 9) {

			BNDR_EXCEPTION("std::vector<T> has incorrect size to initialize bndr::Mat3x3<T> instance");
		}
		base::data = new T[9];
		base::nRows = 3;
		base::nCols = 3;
		for (int i = 0; i < 9; i++) {

			base::data[i] = *(matData.begin() + i);
		}
	}

	template <class T>
	Mat3x3<T>::Mat3x3(const Mat3x3<T>& mat) {

		base::data = new T[9];
		base::nRows = 3;
		base::nCols = 3;
		// copy the new data and overwrite the existing data
		for (int i = 0; i < 9; i++) {

			base::data[i] = mat[i];
		}
	}

	template <class T>
	Mat3x3<T>::Mat3x3(Mat3x3<T>&& mat) {

		base::data = new T[9];
		base::nRows = 3;
		base::nCols = 3;
		// copy the new data and overwrite the existing data
		for (int i = 0; i < 9; i++) {

			base::data[i] = mat[i];
		}
		// make sure the Mat3x3<T> instance is deleted in this scope
		std::unique_ptr<Mat3x3<T>> ptr(&mat);
	}
	
	template <class T>
	void Mat3x3<T>::operator=(const Mat3x3<T>& mat) {

		// overwrite the existing data with the new data
		for (int i = 0; i < 9; i++) {

			base::data[i] = mat[i];
		}
	}
	
	template <class T>
	Mat3x3<T> Mat3x3<T>::operator+(const Mat3x3<T>& mat) {
	
		return {

			(*this)[0] + mat[0], (*this)[1] + mat[1], (*this)[2] + mat[2],
			(*this)[3] + mat[3], (*this)[4] + mat[4], (*this)[5] + mat[5],
			(*this)[6] + mat[6], (*this)[7] + mat[7], (*this)[8] + mat[8]
		};
	}

	template <class T>
	Mat3x3<T> Mat3x3<T>::operator-(const Mat3x3<T>& mat) {

		return {

			(*this)[0] - mat[0], (*this)[1] - mat[1], (*this)[2] - mat[2],
			(*this)[3] - mat[3], (*this)[4] - mat[4], (*this)[5] - mat[5],
			(*this)[6] - mat[6], (*this)[7] - mat[7], (*this)[8] - mat[8]
		};
	}

	template <class T>
	Mat3x3<T> Mat3x3<T>::operator*(const Mat3x3<T>& mat) {

		return {

			(*this)[0] * mat[0] + (*this)[1] * mat[3] + (*this)[2] * mat[6], (*this)[0] * mat[1] + (*this)[1] * mat[4] + (*this)[2] * mat[7], (*this)[0] * mat[2] + (*this)[1] * mat[5] + (*this)[2] * mat[8],
			(*this)[3] * mat[0] + (*this)[4] * mat[3] + (*this)[5] * mat[6], (*this)[3] * mat[1] + (*this)[4] * mat[4] + (*this)[5] * mat[7], (*this)[3] * mat[2] + (*this)[4] * mat[5] + (*this)[5] * mat[8],
			(*this)[6] * mat[0] + (*this)[7] * mat[3] + (*this)[8] * mat[6], (*this)[6] * mat[1] + (*this)[7] * mat[4] + (*this)[8] * mat[7], (*this)[6] * mat[2] + (*this)[7] * mat[5] + (*this)[8] * mat[8]
		};
	}

	template <class T>
	Vec3<T> Mat3x3<T>::operator*(const Vec3<T>& vec) {

		return Vec3<T>(
			(*this)[0] * vec.getValue(0) + (*this)[1] * vec.getValue(1) + (*this)[2] * vec.getValue(2),
			(*this)[3] * vec.getValue(0) + (*this)[4] * vec.getValue(1) + (*this)[5] * vec.getValue(2),
			(*this)[6] * vec.getValue(0) + (*this)[7] * vec.getValue(1) + (*this)[8] * vec.getValue(2)
			
			);
	}

	template <class T>
	void Mat3x3<T>::operator+=(const Mat3x3<T>& mat) {

		for (int i = 0; i < 9; i++) {

			base::data[i] += mat[i];
		}
	}
	
	template <class T>
	void Mat3x3<T>::operator-=(const Mat3x3<T>& mat) {

		for (int i = 0; i < 9; i++) {

			base::data[i] -= mat[i];
		}
	}

	template <class T>
	void Mat3x3<T>::operator*=(const Mat3x3<T>& mat) {

		std::vector<T> copy(9);
		copy[0] = (*this)[0] * mat[0] + (*this)[1] * mat[3] + (*this)[2] * mat[6];
		copy[1] = (*this)[0] * mat[1] + (*this)[1] * mat[4] + (*this)[2] * mat[7];
		copy[2] = (*this)[0] * mat[2] + (*this)[1] * mat[5] + (*this)[2] * mat[8];
		copy[3] = (*this)[3] * mat[0] + (*this)[4] * mat[3] + (*this)[5] * mat[6];
		copy[4] = (*this)[3] * mat[1] + (*this)[4] * mat[4] + (*this)[5] * mat[7];
		copy[5] = (*this)[3] * mat[2] + (*this)[4] * mat[5] + (*this)[5] * mat[8];
		copy[6] = (*this)[6] * mat[0] + (*this)[7] * mat[3] + (*this)[8] * mat[6];
		copy[7] = (*this)[6] * mat[1] + (*this)[7] * mat[4] + (*this)[8] * mat[7];
		copy[8] = (*this)[6] * mat[2] + (*this)[7] * mat[5] + (*this)[8] * mat[8];
		for (int i = 0; i < copy.size(); i++) {

			base::data[i] = copy[i];
		}
	}
}

template <class T>
std::ostream& operator<<(std::ostream& out, const bndr::Mat3x3<T>& mat) {

	out << "{ " << mat.getAt(0, 0) << ' ' << mat.getAt(0, 1) << ' ' << mat.getAt(0, 2) << " }\n";
	out << "{ " << mat.getAt(1, 0) << ' ' << mat.getAt(1, 1) << ' ' << mat.getAt(1, 2) << " }\n";
	out << "{ " << mat.getAt(2, 0) << ' ' << mat.getAt(2, 1) << ' ' << mat.getAt(2, 2) << " }\n";
	return out;
}

