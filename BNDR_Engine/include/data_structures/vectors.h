#pragma once
#include <pch.h>

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

namespace bndr {

	// a template method to compute the square root of a class T
	// accuracyLevel simply refers to the number of times the division algorithm will be called
	// hint: the lower the accuracyLevel, the faster the performance but also the lesser accurate the answer
	// whereas the higher the accuracyLevel, the slower the performance but also the more accurate the answer
	template <class T>
	constexpr float squareRoot(const T& element, int accuracyLevel = 5);

	template <class T>
	constexpr float squareRoot(const T& element, int accuracyLevel) {

		// handle base cases
		if (element <= static_cast<T>(1)) { return static_cast<float>(element); }
		if (element == static_cast<T>(2)) { return 1.414214f; }

		// if the number is greater than or equal to 3 start with the number half of it
		float number = static_cast<float>(element);
		float estimate = number / 2.0f;
		for (int i = 0; i < accuracyLevel; i++) {

			float division = number / estimate;
			estimate = (estimate + division) / 2.0f;
		}
		return estimate;
	}


	// bndr::BaseVector
	// Description: base class for all vectors in the engine. It is a template class, however the type T will need to
	// have operator overloaders for +, -, /, *, +=, -=, /=, *=, =, ==, !=, >, <, >=, and <= or else errors will be raised at runtime
	template <class T>
	class BaseVector {

	protected:
		// array of coordinates
		T* data;

		inline T& operator[](int index) { return data[index]; }
	public:

		// default constructor sets each of the values to their default constructor
		BaseVector() : data(new T[2]) {}
		// assign the objects in the array to the new objects
		BaseVector(const T& x, const T& y);
		// delete the vector BaseVector<T>::data in the array
		~BaseVector();
	};

	template <class T>
	BaseVector<T>::BaseVector(const T& x, const T& y) {

		data = new T[2];
		data[0] = x;
		data[1] = y;
	}

	template <class T>
	BaseVector<T>::~BaseVector() {

		delete[] data;
	}

	// bndr::Vec2
	// Description: Builds off of the abstract vector class
	template <class T>
	class Vec2 : public BaseVector<T> {

	public:
		Vec2() : BaseVector<T>() {}
		Vec2(const T& x, const T& y) : BaseVector<T>(x,y) {}
		// add two Vec2s together and return their sum
		inline Vec2<T> operator+(const Vec2<T>& vec) { return Vec2<T>(BaseVector<T>::data[0] + vec[0], BaseVector<T>::data[1] + vec[1]); }
		// subtract two Vec2s and return their difference
		inline Vec2<T> operator-(const Vec2<T>& vec) { return Vec2<T>(BaseVector<T>::data[0] - vec[0], BaseVector<T>::data[1] - vec[1]); }
		// return a vector with the components incremented by a scalar
		inline Vec2<T> operator+(const T& scalar) { return Vec2<T>(BaseVector<T>::data[0] + scalar, BaseVector<T>::data[0] + scalar); }
		// return a vector with the components decremented by a scalar
		inline Vec2<T> operator-(const T& scalar) { return (*this) + (-scalar); }
		// compute the dot product of two Vec2s
		inline T operator*(const Vec2<T>& vec) { return T(BaseVector<T>::data[0]*vec[0] + BaseVector<T>::data[1]*vec[1]); }
		// compute the cross product of two Vec2s
		inline Vec2<T> operator%(const Vec2<T>& vec) { return Vec2<T>(BaseVector<T>::data[0] * vec[1], -vec[0] * BaseVector<T>::data[1]); }
		// scale a Vec2 up
		inline Vec2<T> operator*(const T& scalar) { return Vec2<T>(scalar * BaseVector<T>::data[0], scalar * BaseVector<T>::data[1]); }
		// scale a Vec2 down
		inline Vec2<T> operator/(const T& scalar) { return (*this) * static_cast<T>(1.0)/scalar; }
		// increment this vector by a scalar
		inline void operator+=(const T& scalar) { BaseVector<T>::data[0] += scalar; BaseVector<T>::data[1] += scalar; }
		// decrement this vector by a scalar
		inline void operator-=(const T& scalar) { (*this) += -scalar; }
		// multiply this vector by a scalar
		inline void operator*=(const T& scalar) { BaseVector<T>::data[0] *= scalar; BaseVector<T>::data[1] *= scalar; }
		// divide this vector by a scalar
		inline void operator/=(const T& scalar) { (*this) *= static_cast<T>(1.0)/scalar; }
		// add Vec2 to this
		inline void operator+=(const Vec2<T>& vec) { BaseVector<T>::data[0] += vec[0]; BaseVector<T>::data[1] += vec[1]; }
		// subtract Vec2 from this
		inline void operator-=(const Vec2<T>& vec) { BaseVector<T>::data[0] -= vec[0]; BaseVector<T>::data[1] -= vec[1]; }
		// compute the cross product in place
		inline void operator%=(const Vec2<T>& vec) { BaseVector<T>::data[0] = BaseVector<T>::data[0] * vec[1]; BaseVector<T>::data[1] = -vec[0] * BaseVector<T>::data[1]; }
		// get the square of the magnitude of a Vec2
		static T magSquared(const Vec2<T>& vec) {

			return (vec[0] * vec[0] + vec[1] * vec[1]);
		}
		// returns the unit normal of the two vectors
		static Vec2<T> normal(const Vec2<T>& vecA, const Vec2<T>& vecB) {

			Vec2<T> crossProd = vecA % vecB;
			T mag = magSquared(crossProd);
			if (mag == 0) {
				
				BNDR_EXCEPTION("Computing the normal of these two vectors resulted in a divide by zero error.");
			}
			crossProd /= mag;
			return crossProd;
		}
		// output the Vec2 to cout or an ostream instance
		friend std::ostream& operator<<(std::ostream& out, const Vec2<T>& vec);

	};

	template <class T>
	std::ostream& operator<<(std::ostream& out, const Vec2<T>& vec) {

		// out << "{ " << vec[0] << ' ' << vec[1] << " }";
		out << "Hello World!\n";
		return out;
	}
}

