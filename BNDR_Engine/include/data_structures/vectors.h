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
	constexpr float squareRoot(const T& element, int accuracyLevel = 7);

	template <class T>
	constexpr float squareRoot(const T& element, int accuracyLevel) {

		// handle base cases
		if (element <= static_cast<T>(1)) { return static_cast<float>(element); }
		if (element == static_cast<T>(2)) { return 1.414214f; }

		accuracyLevel += static_cast<int>(static_cast<float>(element)*static_cast<float>(element)*0.0000001f);
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
	// BaseVector is also not meant to have an instance so it is technically abstract
	template <class T>
	class BaseVector {

	protected:
		// array of coordinates
		T* data;

	public:

		inline T operator[](int index) const { return data[index]; }
		// default constructor sets each of the values to their default constructor
		BaseVector() : data(new T[2]) {}
		// assign the objects in the array to the new objects
		BaseVector(const T& x, const T& y, int numCoords = 2);
		// delete the vector BaseVector<T>::data in the array
		~BaseVector();
	};

	template <class T>
	BaseVector<T>::BaseVector(const T& x, const T& y, int numCoords) {

		data = new T[numCoords];
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

		using base = typename BaseVector<T>;
	public:


		// constructors/assignment

		// default constructor
		Vec2() : BaseVector<T>() {}
		// constructor
		Vec2(const T& x, const T& y, int numCoords = 2) : BaseVector<T>(x, y, numCoords) {}
		// copy constructor
		Vec2(const Vec2<T>& vec);
		// move constructor
		Vec2(Vec2&& vec);
		// assignment operator
		void operator=(const Vec2<T>& vec);

		// vector operations

		// add two Vec2s together and return their sum
		inline Vec2<T> operator+(const Vec2<T>& vec) { return Vec2<T>(base::data[0] + vec[0], base::data[1] + vec[1]); }
		// subtract two Vec2s and return their difference
		inline Vec2<T> operator-(const Vec2<T>& vec) { return Vec2<T>(base::data[0] - vec[0], base::data[1] - vec[1]); }
		// return a vector with the components incremented by a scalar
		inline Vec2<T> operator+(const T& scalar) { return Vec2<T>(base::data[0] + scalar, base::data[1] + scalar); }
		// return a vector with the components decremented by a scalar
		inline Vec2<T> operator-(const T& scalar) { return (*this) + (-scalar); }
		// compute the dot product of two Vec2s
		inline T operator*(const Vec2<T>& vec) { return T(base::data[0]*vec[0] + base::data[1]*vec[1]); }
		// scale a Vec2 up
		inline Vec2<T> operator*(const T& scalar) { return Vec2<T>(scalar * base::data[0], scalar * base::data[1]); }
		// scale a Vec2 down
		inline Vec2<T> operator/(const T& scalar) { return Vec2<T>(base::data[0] / scalar, base::data[1] / scalar); }
		// increment this vector by a scalar
		inline void operator+=(const T& scalar) { base::data[0] += scalar; base::data[1] += scalar; }
		// decrement this vector by a scalar
		inline void operator-=(const T& scalar) { (*this) += -scalar; }
		// multiply this vector by a scalar
		inline void operator*=(const T& scalar) { base::data[0] *= scalar; base::data[1] *= scalar; }
		// divide this vector by a scalar
		inline void operator/=(const T& scalar) { base::data[0] /= scalar; base::data[1] /= scalar; }
		// add Vec2 to this
		inline void operator+=(const Vec2<T>& vec) { base::data[0] += vec[0]; base::data[1] += vec[1]; }
		// subtract Vec2 from this
		inline void operator-=(const Vec2<T>& vec) { base::data[0] -= vec[0]; base::data[1] -= vec[1]; }
		// get the square of the distance of a Vec2
		static T distanceSquared(const Vec2<T>& vec) {

			return (vec[0] * vec[0] + vec[1] * vec[1]);
		}
		// get the distance itself
		static T distance(const Vec2<T>& vec) {

			return static_cast<T>(squareRoot(distanceSquared(vec)));
		}
		// get the unit vector
		static Vec2<T> unit(const Vec2<T>& vec) {

			T mag = distance(vec);
			if (mag == static_cast<T>(0)) {

				BNDR_EXCEPTION("Computing the unit vector resulted in a divide by zero error.");
			}
			return vec / mag;
		}

	};

	template <class T>
	Vec2<T>::Vec2(const Vec2<T>& vec) {

		base::data = new T[2];
		base::data[0] = vec[0];
		base::data[1] = vec[1];
	}

	template <class T>
	Vec2<T>::Vec2(Vec2<T>&& vec) {

		base::data = new T[2];
		base::data[0] = vec[0];
		base::data[1] = vec[1];
	}

	template <class T>
	void Vec2<T>::operator=(const Vec2<T>& vec) {

		base::data[0] = vec[0];
		base::data[1] = vec[1];
	}

	// bndr::Vec3
	// Description: inherits from Vec2 and has the same functionality with one extra coordinate 
	template <class T>
	class Vec3 : public Vec2<T> {

		using base = typename BaseVector<T>;
	public:

		// constructors/assignment

		Vec3() : base::data(new T[3]) {}
		Vec3(const T& x, const T& y, const T& z);
		// copy constructor
		Vec3(const Vec3<T>& vec);
		// move constructor
		Vec3(Vec3&& vec);
		// assignment operator
		void operator=(const Vec3<T>& vec);

		// vector operations

		// add two Vec3s together and return their sum
		inline Vec3<T> operator+(const Vec3<T>& vec) { return Vec3<T>(base::data[0] + vec[0], base::data[1] + vec[1], base::data[2] + vec[2]); }
		// subtract two Vec3s and return their difference
		inline Vec3<T> operator-(const Vec3<T>& vec) { return Vec3<T>(base::data[0] - vec[0], base::data[1] - vec[1], base::data[2] - vec[2]); }
		// return a vector with the components incremented by a scalar
		inline Vec3<T> operator+(const T& scalar) { return Vec3<T>(base::data[0] + scalar, base::data[1] + scalar, base::data[2] + scalar); }
		// return a vector with the components decremented by a scalar
		inline Vec3<T> operator-(const T& scalar) { return (*this) + (-scalar); }
		// compute the dot product of two Vec3s
		inline T operator*(const Vec3<T>& vec) { return T(base::data[0] * vec[0] + base::data[1] * vec[1] + base::data[2] * vec[2]); }
		// compute the cross product of two Vec3s
		inline Vec3<T> operator%(const Vec3<T>& vec) { return Vec3<T>(base::data[1] * vec[2] - base::data[2] * vec[1], base::data[2] * vec[0] - base::data[0] * vec[2], base::data[0] * vec[1] - base::data[1] * vec[0]); }
		// scale a Vec3 up
		inline Vec3<T> operator*(const T& scalar) { return Vec3<T>(scalar * base::data[0], scalar * base::data[1], scalar * base::data[2]); }
		// scale a Vec3 down
		inline Vec3<T> operator/(const T& scalar) { return Vec3<T>(base::data[0] / scalar, base::data[1] / scalar, base::data[2] / scalar); }
		// increment this vector by a scalar
		inline void operator+=(const T& scalar) { base::data[0] += scalar; base::data[1] += scalar; base::data[2] += scalar; }
		// decrement this vector by a scalar
		inline void operator-=(const T& scalar) { (*this) += -scalar; }
		// multiply this vector by a scalar
		inline void operator*=(const T& scalar) { base::data[0] *= scalar; base::data[1] *= scalar; base::data[2] *= scalar; }
		// divide this vector by a scalar
		inline void operator/=(const T& scalar) { base::data[0] /= scalar; base::data[1] /= scalar; base::data[2] /= scalar; }
		// add Vec3 to this
		inline void operator+=(const Vec3<T>& vec) { base::data[0] += vec[0]; base::data[1] += vec[1]; base::data[2] += vec[2]; }
		// subtract Vec3 from this
		inline void operator-=(const Vec3<T>& vec) { base::data[0] -= vec[0]; base::data[1] -= vec[1]; base::data[2] -= vec[2]; }
		// compute the cross product in place
		void operator%=(const Vec3<T>& vec);
		// get the square of the distance of a Vec3
		static T distanceSquared(const Vec3<T>& vec) {

			return (vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
		}
		// get the distance itself
		static T distance(const Vec3<T>& vec) {

			return static_cast<T>(squareRoot(distanceSquared(vec)));
		}
		// get the unit vector
		static Vec3<T> unit(const Vec3<T>& vec) {

			T mag = distance(vec);
			if (mag == static_cast<T>(0)) {

				BNDR_EXCEPTION("Computing the unit vector resulted in a divide by zero error.");
			}
			return vec / mag;
		}
		// returns the unit normal of the two vectors
		static Vec3<T> normal(const Vec3<T>& vecA, const Vec3<T>& vecB) {

			Vec3<T> crossProd = vecA % vecB;
			T mag = distance(crossProd);
			if (mag == static_cast<T>(0)) {

				BNDR_EXCEPTION("Computing the normal of these two vectors resulted in a divide by zero error.");
			}
			crossProd /= mag;
			return crossProd;
		}


	};

	template <class T>
	Vec3<T>::Vec3(const T& x, const T& y, const T& z) {

		base::data = new T[3];
		base::data[0] = x;
		base::data[1] = y;
		base::data[2] = z;
	}

	template <class T>
	void Vec3<T>::operator%=(const Vec3<T>& vec) {

		base::data[0] = base::data[1] * vec[2] - base::data[2] * vec[1];
		base::data[1] = base::data[2] * vec[0] - base::data[0] * vec[2];
		base::data[2] = base::data[0] * vec[1] - base::data[1] * vec[0];
	}

	template <class T>
	Vec3<T>::Vec3(const Vec3<T>& vec) {

		base::data = new T[2];
		base::data[0] = vec[0];
		base::data[1] = vec[1];
		base::data[2] = vec[2];
	}

	template <class T>
	Vec3<T>::Vec3(Vec3<T>&& vec) {

		base::data = new T[2];
		base::data[0] = vec[0];
		base::data[1] = vec[1];
		base::data[2] = vec[2];
	}

	template <class T>
	void Vec3<T>::operator=(const Vec3<T>& vec) {

		base::data[0] = vec[0];
		base::data[1] = vec[1];
		base::data[2] = vec[2];
	}
}

// output bndr:Vec2<T> to an ostream
template <class T>
std::ostream& operator<<(std::ostream& out, const bndr::Vec2<T>& vec) {

	out << "{ " << vec[0] << ' ' << vec[1] << " }";
	return out;
}

// output bndr:Vec3<T> to an ostream
template <class T>
std::ostream& operator<<(std::ostream& out, const bndr::Vec3<T>& vec) {

	out << "{ " << vec[0] << ' ' << vec[1] << ' ' << vec[2] << " }";
	return out;
}

