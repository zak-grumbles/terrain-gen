#ifndef ALGEBRA_H
#define ALGEBRA_H

#include <math.h>
#include <stdlib.h>
#include <string>

const double EPSILON = 1e-30;
#define PI 3.1415926535897932384626433832795028841971693993751058209749445923
#define RAD 0.0174532925199432954743716805978692718782
#define DBL_MAX 1.7976931348623158e+308  //this defines the max double

#define IN_RANGE(a,b)   (((a>(b-EPSILON))&&(a<(b+EPSILON)))?1:0)
#define DEG_TO_RAD(a)	(a*RAD)
#define RAD_TO_DEG(a)	(a/RAD)
#define SQR(a)	 (a*a)

class Point {
public:
	Point() {
		p[0] = 0;
		p[1] = 0;
		p[2] = 0;
		p[3] = 1;
	};

	Point(double x, double y, double z) {
		p[0] = x;
		p[1] = y;
		p[2] = z;
		p[3] = 1;
	};

	Point(const Point& v) {
		p[0] = v[0];
		p[1] = v[1];
		p[2] = v[2];
		p[3] = 1;
	};

	~Point() {};

	void normalize() {
		double l = sqrt((p[0] * p[0]) + (p[1] * p[1]) + (p[2] * p[2]));
		p[0] = p[0] / l;
		p[1] = p[1] / l;
		p[2] = p[2] / l;
		p[3] = 1;
	};

	Point& operator= (const Point v) {
		p[0] = v[0];
		p[1] = v[1];
		p[2] = v[2];
		p[3] = 1;
		return *this;
	};

	bool operator!= (const Point v) {
		if ((IN_RANGE(p[0], v[0]) == false) || (IN_RANGE(p[1], v[1]) == false) || (IN_RANGE(p[2], v[2]) == false)) {
			return true;
		}
		return false;
	};

	bool operator== (const Point v) {
		if ((IN_RANGE(p[0], v[0]) == true) && (IN_RANGE(p[1], v[1]) == true) && (IN_RANGE(p[2], v[2]) == true)) {
			return true;
		}
		return false;
	};

	double at(int i) const {
		return p[i];
	}

	double& at(int i) {
		return p[i];
	}

	double operator[] (int i) const {
		return p[i];
	};

	double& operator[] (int i) {
		return p[i];
	}

	void unpack(double* d) {
		d[0] = p[0];
		d[1] = p[1];
		d[2] = p[2];
		d[3] = p[3];
	};

	double* unpack() {
		return p;
	}

	void init() {
		p[0] = 0;
		p[1] = 0;
		p[2] = 0;
		p[3] = 1;
	}

private:
	double p[4];
};



class Vector {
public:
	Vector() {
		p[0] = 0;
		p[1] = 0;
		p[2] = 0;
		p[3] = 0;
	};

	Vector(double x, double y, double z) {
		p[0] = x;
		p[1] = y;
		p[2] = z;
		p[3] = 0;
	};

	Vector(const Vector& v) {
		p[0] = v[0];
		p[1] = v[1];
		p[2] = v[2];
		p[3] = 0;
	};

	~Vector() {};

	void normalize() {
		double l = sqrt((p[0] * p[0]) + (p[1] * p[1]) + (p[2] * p[2]));
		if (l != 0) {
			p[0] = p[0] / l;
			p[1] = p[1] / l;
			p[2] = p[2] / l;
			p[3] = 0;
		}
	};

	void negate() {
		p[0] = -p[0];
		p[1] = -p[1];
		p[2] = -p[2];
		//p[3] = 0;
	}


	Vector& operator= (const Vector v) {
		p[0] = v[0];
		p[1] = v[1];
		p[2] = v[2];
		p[3] = 0;
		return *this;
	};

	bool operator!= (const Vector v) {
		if ((IN_RANGE(p[0], v[0]) == false) || (IN_RANGE(p[1], v[1]) == false) || (IN_RANGE(p[2], v[2]) == false)) {
			return true;
		}
		return false;
	};

	bool operator== (const Vector v) {
		//if ((p[0] == v[0]) && (p[1] == v[1]) && (p[2] == v[2])) {
		//	return 1;
		//	}
		//return 0;
		if ((IN_RANGE(p[0], v[0]) == true) && (IN_RANGE(p[1], v[1]) == true) && (IN_RANGE(p[2], v[2]) == true)) {
			return true;
		}
		return false;
	};

	double at(int i) const {
		return p[i];
	}

	double& at(int i) {
		return p[i];
	}

	double operator[] (int i) const {
		return p[i];
	};

	double& operator[] (int i) {
		return p[i];
	}

	double length() {
		double d = sqrt(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]);
		return d;
	};

	void unpack(double* d) {
		d[0] = p[0];
		d[1] = p[1];
		d[2] = p[2];
		d[3] = p[3];
	};

	double* unpack() {
		return p;
	}

	void init() {
		p[0] = 0;
		p[1] = 0;
		p[2] = 0;
		p[3] = 0;
	}

private:
	double p[4];
};


class Matrix {
public:
	Matrix() {
		p[0] = 1; p[1] = 0; p[2] = 0; p[3] = 0;
		p[4] = 0; p[5] = 1; p[6] = 0; p[7] = 0;
		p[8] = 0; p[9] = 0; p[10] = 1; p[11] = 0;
		p[12] = 0; p[13] = 0; p[14] = 0; p[15] = 1;
	};

	Matrix(double a, double b, double c, double d,
		double e, double f, double g, double h,
		double i, double j, double k, double l,
		double m, double n, double o, double q) {
		p[0] = a; p[1] = e; p[2] = i; p[3] = m;
		p[4] = b; p[5] = f; p[6] = j; p[7] = n;
		p[8] = c; p[9] = g; p[10] = k; p[11] = o;
		p[12] = d; p[13] = h; p[14] = l; p[15] = q;
	};


	Matrix(const Matrix& m) {
		p[0] = m[0]; p[1] = m[1]; p[2] = m[2]; p[3] = m[3];
		p[4] = m[4]; p[5] = m[5]; p[6] = m[6]; p[7] = m[7];
		p[8] = m[8]; p[9] = m[9]; p[10] = m[10]; p[11] = m[11];
		p[12] = m[12]; p[13] = m[13]; p[14] = m[14]; p[15] = m[15];
	}

	Matrix(double* m) {
		p[0] = m[0]; p[1] = m[1]; p[2] = m[2]; p[3] = m[3];
		p[4] = m[4]; p[5] = m[5]; p[6] = m[6]; p[7] = m[7];
		p[8] = m[8]; p[9] = m[9]; p[10] = m[10]; p[11] = m[11];
		p[12] = m[12]; p[13] = m[13]; p[14] = m[14]; p[15] = m[15];
	}

	~Matrix() {
	};

	void unpack(double* m) const {
		m[0] = p[0];   m[1] = p[1];   m[2] = p[2];   m[3] = p[3];
		m[4] = p[4];   m[5] = p[5];   m[6] = p[6];   m[7] = p[7];
		m[8] = p[8];   m[9] = p[9];   m[10] = p[10]; m[11] = p[11];
		m[12] = p[12]; m[13] = p[13]; m[14] = p[14]; m[15] = p[15];
	};

	double* unpack() {
		return p;
	}

	bool operator==(const Matrix m) const {
		if ((p[0] == m[0]) && (p[1] == m[1]) && (p[2] == m[2]) && (p[3] == m[3]) &&
			(p[4] == m[4]) && (p[5] == m[5]) && (p[6] == m[6]) && (p[7] == m[7]) &&
			(p[8] == m[8]) && (p[9] == m[9]) && (p[10] == m[10]) && (p[11] == m[11]) &&
			(p[12] == m[12]) && (p[13] == m[13]) && (p[14] == m[14]) && (p[15] == m[15]))
			return 1;
		return 0;
	};

	bool operator!=(const Matrix m) const {
		if ((p[0] != m[0]) || (p[1] != m[1]) || (p[2] != m[2]) || (p[3] != m[3]) ||
			(p[4] != m[4]) || (p[5] != m[5]) || (p[6] != m[6]) || (p[7] != m[7]) ||
			(p[8] != m[8]) || (p[9] != m[9]) || (p[10] != m[10]) || (p[11] != m[11]) ||
			(p[12] != m[12]) || (p[13] != m[13]) || (p[14] != m[14]) || (p[15] != m[15]))
			return 1;
		return 0;
	};

	Matrix& operator=(const Matrix m) {
		p[0] = m[0];  p[1] = m[1]; p[2] = m[2]; p[3] = m[3];
		p[4] = m[4];  p[5] = m[5]; p[6] = m[6]; p[7] = m[7];
		p[8] = m[8];  p[9] = m[9]; p[10] = m[10]; p[11] = m[11];
		p[12] = m[12]; p[13] = m[13];  p[14] = m[14]; p[15] = m[15];
		return *this;
	};

	double operator[] (int i) const {
		return p[i];
	}

	double& operator[] (int i) {
		return p[i];
	}

	double operator() (const int i, const int j) const {
		return p[j * 4 + i];
	};

	double& operator() (const int i, const int j) {
		return p[j * 4 + i];
	};

	void init() {
		p[0] = 1; p[1] = 0; p[2] = 0; p[3] = 0;
		p[4] = 0; p[5] = 1; p[6] = 0; p[7] = 0;
		p[8] = 0; p[9] = 0; p[10] = 1; p[11] = 0;
		p[12] = 0; p[13] = 0; p[14] = 0; p[15] = 1;
	}
private:
	double p[16];
};


// ======================================================================
//
// Functions for Arithmetic Operators
//
// ======================================================================



// --- Vectors and Points

// This adds a point to a point, and returns the resultant point
inline Point operator+(const Point& v, const Point& p) {
	Point t;
	t[0] = p[0] + v[0];  t[1] = p[1] + v[1];  t[2] = p[2] + v[2];  t[3] = p[3];
	return t;
};

// This adds a vector to a point, and returns the resultant point
inline Point operator+(const Vector& v, const Point& p) {
	Point t;
	t[0] = p[0] + v[0];  t[1] = p[1] + v[1];  t[2] = p[2] + v[2];  t[3] = p[3];
	return t;
};

// This does the same.
inline Point operator+(const Point& p, const Vector& v) {
	Point t;
	t[0] = p[0] + v[0];  t[1] = p[1] + v[1];  t[2] = p[2] + v[2];  t[3] = p[3];
	return t;
};

// This adds two vectors, and returns the resultant one
inline Vector operator+(const Vector& v1, const Vector& v2) {
	Vector t;
	t[0] = v1[0] + v2[0];  t[1] = v1[1] + v2[1];  t[2] = v1[2] + v2[2];  t[3] = v1[3] + v2[3];
	return t;
};

// Note: no (point + point) operator



// This subtracts a vector from a point, and returns the resultant point.
inline Point operator-(const Point& p, const Vector& v) {
	Point t;
	t[0] = p[0] - v[0];  t[1] = p[1] - v[1];  t[2] = p[2] - v[2];  t[3] = p[3];
	return t;
};

// This returns the negated Point
inline Point operator-(const Point& v) {
	Point t;
	t[0] = v[0] * -1;  t[1] = v[1] * -1;  t[2] = v[2] * -1;  t[3] = 1;
	return t;
};


// This returns the negated vector
inline Vector operator-(const Vector& v) {
	Vector t;
	t[0] = v[0] * -1;  t[1] = v[1] * -1;  t[2] = v[2] * -1;  t[3] = 0;
	return t;
};

// This subtracts the second vector from the first, and returns the resultant.
inline Vector operator-(const Vector& v1, const Vector& v2) {
	Vector t;
	t[0] = v1[0] - v2[0];  t[1] = v1[1] - v2[1];  t[2] = v1[2] - v2[2];  t[3] = 0;
	return t;
};

// This returns the vector spanning point 1 and point 2
inline Vector operator-(const Point& p1, const Point& p2) {
	Vector t;
	t[0] = p1[0] - p2[0];  t[1] = p1[1] - p2[1];  t[2] = p1[2] - p2[2];  t[3] = 0;
	return t;
};

// Note: no (vector - point) or (- point) operators



// --- Scalars

inline Point operator*(const double s, const Point& v) {
	Point t;
	t[0] = v[0] * s;  t[1] = v[1] * s;  t[2] = v[2] * s;  t[3] = 0;
	return t;
};

inline Point operator*(const Point& v, const double s) {
	Point t;
	t[0] = v[0] * s;  t[1] = v[1] * s;  t[2] = v[2] * s;  t[3] = 0;
	return t;
};


inline Point operator/(const Point& v, const double s) {
	Point t;
	t[0] = v[0] / s;  t[1] = v[1] / s;  t[2] = v[2] / s;  t[3] = 0;
	return t;
};


// This divides a vector by a scalar
inline Vector operator/(const Vector& v, const double s) {
	Vector t;
	t[0] = v[0] / s;  t[1] = v[1] / s;  t[2] = v[2] / s;  t[3] = 0;
	return t;
};

// This should multiply the vector by the scalar, returning a vector
inline Vector operator*(const double s, const Vector &v) {
	Vector t;
	t[0] = v[0] * s;  t[1] = v[1] * s;  t[2] = v[2] * s;  t[3] = 0;
	return t;
};

// This does the same.
inline Vector operator*(Vector& v, const double s) {
	Vector t;
	t[0] = v[0] * s;  t[1] = v[1] * s;  t[2] = v[2] * s;  t[3] = 0;
	return t;
};



// --- Matrix Operator

// Applies the matrix to a point, returns the new point
inline Point operator*(const Matrix& m, const Point& p) {
	Point t;
	t[0] = p[0] * m(0, 0) + p[1] * m(0, 1) + p[2] * m(0, 2) + p[3] * m(0, 3);
	t[1] = p[0] * m(1, 0) + p[1] * m(1, 1) + p[2] * m(1, 2) + p[3] * m(1, 3);
	t[2] = p[0] * m(2, 0) + p[1] * m(2, 1) + p[2] * m(2, 2) + p[3] * m(2, 3);
	t[3] = p[0] * m(3, 0) + p[1] * m(3, 1) + p[2] * m(3, 2) + p[3] * m(3, 3);
	return t;
};

// Applies the matrix to the vector, returns the new vector.
inline Vector operator*(const Matrix& m, const Vector& v) {
	Vector t;
	t[0] = v[0] * m(0, 0) + v[1] * m(0, 1) + v[2] * m(0, 2);
	t[1] = v[0] * m(1, 0) + v[1] * m(1, 1) + v[2] * m(1, 2);
	t[2] = v[0] * m(2, 0) + v[1] * m(2, 1) + v[2] * m(2, 2);
	t[3] = 0;
	return t;
};

// Multiplies two matrices together, returns the resultant matrix
inline Matrix operator*(const Matrix& m1, const Matrix& m2) {
	Matrix t(
		m1(0, 0)*m2(0, 0) + m1(0, 1)*m2(1, 0) + m1(0, 2)*m2(2, 0) + m1(0, 3)*m2(3, 0),
		m1(0, 0)*m2(0, 1) + m1(0, 1)*m2(1, 1) + m1(0, 2)*m2(2, 1) + m1(0, 3)*m2(3, 1),
		m1(0, 0)*m2(0, 2) + m1(0, 1)*m2(1, 2) + m1(0, 2)*m2(2, 2) + m1(0, 3)*m2(3, 2),
		m1(0, 0)*m2(0, 3) + m1(0, 1)*m2(1, 3) + m1(0, 2)*m2(2, 3) + m1(0, 3)*m2(3, 3),

		m1(1, 0)*m2(0, 0) + m1(1, 1)*m2(1, 0) + m1(1, 2)*m2(2, 0) + m1(1, 3)*m2(3, 0),
		m1(1, 0)*m2(0, 1) + m1(1, 1)*m2(1, 1) + m1(1, 2)*m2(2, 1) + m1(1, 3)*m2(3, 1),
		m1(1, 0)*m2(0, 2) + m1(1, 1)*m2(1, 2) + m1(1, 2)*m2(2, 2) + m1(1, 3)*m2(3, 2),
		m1(1, 0)*m2(0, 3) + m1(1, 1)*m2(1, 3) + m1(1, 2)*m2(2, 3) + m1(1, 3)*m2(3, 3),

		m1(2, 0)*m2(0, 0) + m1(2, 1)*m2(1, 0) + m1(2, 2)*m2(2, 0) + m1(2, 3)*m2(3, 0),
		m1(2, 0)*m2(0, 1) + m1(2, 1)*m2(1, 1) + m1(2, 2)*m2(2, 1) + m1(2, 3)*m2(3, 1),
		m1(2, 0)*m2(0, 2) + m1(2, 1)*m2(1, 2) + m1(2, 2)*m2(2, 2) + m1(2, 3)*m2(3, 2),
		m1(2, 0)*m2(0, 3) + m1(2, 1)*m2(1, 3) + m1(2, 2)*m2(2, 3) + m1(2, 3)*m2(3, 3),

		m1(3, 0)*m2(0, 0) + m1(3, 1)*m2(1, 0) + m1(3, 2)*m2(2, 0) + m1(3, 3)*m2(3, 0),
		m1(3, 0)*m2(0, 1) + m1(3, 1)*m2(1, 1) + m1(3, 2)*m2(2, 1) + m1(3, 3)*m2(3, 1),
		m1(3, 0)*m2(0, 2) + m1(3, 1)*m2(1, 2) + m1(3, 2)*m2(2, 2) + m1(3, 3)*m2(3, 2),
		m1(3, 0)*m2(0, 3) + m1(3, 1)*m2(1, 3) + m1(3, 2)*m2(2, 3) + m1(3, 3)*m2(3, 3));
	return t;
};



// --- Length of a vector

// returns the length of the vector
inline double length(const Vector& v) {
	double d = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	return d;
};


// --- Vector dot and cross products

// Returns the dot product of the two vectors.
inline double dot(const Vector& u, const Vector& v) {
	double d = u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
	return d;
};

inline double angle(const Vector& u, const Vector& v) {
	double result = dot(u, v) / (length(u) * length(v));
	if (result >= 1)
		return 0;
	return acos(result);  //returns in radians
}

// Returns the cross product of the two vectors.
inline Vector cross(const Vector& u, const Vector& v) {
	Vector t(u[1] * v[2] - u[2] * v[1], u[2] * v[0] - u[0] * v[2], u[0] * v[1] - u[1] * v[0]);
	return t;
};


// --- Unit length vector pointing in the same direction

// Returns the normalized vector
inline Vector normalize(Vector& v) {
	Vector t(v);
	t.normalize();
	return t;
};

// Transforms --------------------------------------------------------

inline Matrix transpose(const Matrix& m) {
	Matrix t(m(0, 0), m(1, 0), m(2, 0), m(3, 0),
		m(0, 1), m(1, 1), m(2, 1), m(3, 1),
		m(0, 2), m(1, 2), m(2, 2), m(3, 2),
		m(0, 3), m(1, 3), m(2, 3), m(3, 3));
	return t;
};

// returns a scaling transformation matrix, which will scale by the vector v
inline Matrix scale_mat(const Vector& v) {
	Matrix m(v[0], 0, 0, 0,
		0, v[1], 0, 0,
		0, 0, v[2], 0,
		0, 0, 0, 1);
	return m;
};

// Returns a translaion matrix, which will translate by the vector v 
inline Matrix trans_mat(const Vector& v) {
	Matrix m(1, 0, 0, v[0],
		0, 1, 0, v[1],
		0, 0, 1, v[2],
		0, 0, 0, 1);
	return m;
};

// returns a rotation matrix effecting a rotation around the X axis by 
// specified radians
inline Matrix rotX_mat(const double radians) {
	double cos_r = cos(radians);
	double sin_r = sin(radians);
	Matrix m(1, 0, 0, 0,
		0, cos_r, -sin_r, 0,
		0, sin_r, cos_r, 0,
		0, 0, 0, 1);
	return m;
};

// Returns a rotation matrix effecting rotation around the Y axis
inline Matrix rotY_mat(const double radians) {
	double cos_r = cos(radians);
	double sin_r = sin(radians);
	Matrix m(cos_r, 0, sin_r, 0,
		0, 1, 0, 0,
		-sin_r, 0, cos_r, 0,
		0, 0, 0, 1);
	return m;
};

// Returns a rotation matrix effecting rotation around the Z axis
inline Matrix rotZ_mat(double radians) {
	double cos_r = cos(radians);
	double sin_r = sin(radians);
	Matrix m(cos_r, -sin_r, 0, 0,
		sin_r, cos_r, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
	return m;
};

// Returns a rotation matrix effecting a rotation around the given vector and
// point, by the specified number of radians.

inline Matrix rot_mat(Vector &v, double a){
	Vector nv = normalize(v);

	return (
		Matrix(nv[0] * nv[0] + cos(a)*(1.0 - nv[0] * nv[0]),
		nv[0] * nv[1] * (1.0 - cos(a)) -
		nv[2] * sin(a),
		nv[2] * nv[0] * (1.0 - cos(a)) +
		nv[1] * sin(a),
		0,

		nv[0] * nv[1] * (1.0 - cos(a)) +
		nv[2] * sin(a),
		nv[1] * nv[1] + cos(a)*(1.0 -
		nv[1] * nv[1]),
		nv[1] * nv[2] * (1.0 - cos(a)) -
		nv[0] * sin(a),
		0,

		nv[2] * nv[0] * (1.0 - cos(a)) -
		nv[1] * sin(a),
		nv[1] * nv[2] * (1.0 - cos(a)) +
		nv[0] * sin(a),
		nv[2] * nv[2] + cos(a)*(1.0 -
		nv[2] * nv[2]),
		0,

		0, 0, 0, 1.0)
		);
};

inline Matrix rot_mat(Point  &p, Vector &v, double a){
	Vector nv = normalize(v);

	return (trans_mat(Vector(p[0], p[1], p[2])) *
		Matrix(nv[0] * nv[0] + cos(a)*(1.0 - nv[0] * nv[0]),
		nv[0] * nv[1] * (1.0 - cos(a)) -
		nv[2] * sin(a),
		nv[2] * nv[0] * (1.0 - cos(a)) +
		nv[1] * sin(a),
		0,

		nv[0] * nv[1] * (1.0 - cos(a)) +
		nv[2] * sin(a),
		nv[1] * nv[1] + cos(a)*(1.0 -
		nv[1] * nv[1]),
		nv[1] * nv[2] * (1.0 - cos(a)) -
		nv[0] * sin(a),
		0,

		nv[2] * nv[0] * (1.0 - cos(a)) -
		nv[1] * sin(a),
		nv[1] * nv[2] * (1.0 - cos(a)) +
		nv[0] * sin(a),
		nv[2] * nv[2] + cos(a)*(1.0 -
		nv[2] * nv[2]),
		0,

		0, 0, 0, 1.0)
		* trans_mat(Vector(-p[0], -p[1], -p[2])));
};

// Returns the inverse matrix of scale_mat()
inline Matrix inv_scale_mat(const Vector &v) {
	if ((v[0] != 0) && (v[1] != 0) && (v[2] != 0)) {
		Matrix m(1 / v[0], 0, 0, 0,
			0, 1 / v[1], 0, 0,
			0, 0, 1 / v[2], 0,
			0, 0, 0, 1);
		return m;
	}
	else {
		Matrix m;
		return m;
	}
};

// Returns the inverse matrix of trans_may()
inline Matrix inv_trans_mat(const Vector &v) {
	Matrix m(1, 0, 0, -v[0],
		0, 1, 0, -v[1],
		0, 0, 1, -v[2],
		0, 0, 0, 1);
	return m;
};

// Returns the inverse matrix of rotX_mat()
inline Matrix inv_rotX_mat(const double radians){
	double cos_r = cos(-radians);
	double sin_r = sin(-radians);
	Matrix m(1, 0, 0, 0,
		0, cos_r, -sin_r, 0,
		0, sin_r, cos_r, 0,
		0, 0, 0, 1);
	return m;
};

// Returns the inverse matrix of rotY_mat()
inline Matrix inv_rotY_mat(const double radians){
	double cos_r = cos(-radians);
	double sin_r = sin(-radians);
	Matrix m(cos_r, 0, sin_r, 0,
		0, 1, 0, 0,
		-sin_r, 0, cos_r, 0,
		0, 0, 0, 1);
	return m;
};

// Returns the inverse matrix of rotZ_mat()
inline Matrix inv_rotZ_mat(double radians){
	double cos_r = cos(-radians);
	double sin_r = sin(-radians);
	Matrix m(cos_r, -sin_r, 0, 0,
		sin_r, cos_r, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
	return m;
};


inline static void addRowToRow(double* mat, int row1, int row2) {
	mat[row2] += mat[row1];
	mat[row2 + 4] += mat[row1 + 4];
	mat[row2 + 8] += mat[row1 + 8];
	mat[row2 + 12] += mat[row1 + 12];
};

inline static void subMultRow(double* mat, int row1, int row2, double mult) {
	mat[row2] -= mult * mat[row1];
	mat[row2 + 4] -= mult * mat[row1 + 4];
	mat[row2 + 8] -= mult * mat[row1 + 8];
	mat[row2 + 12] -= mult * mat[row1 + 12];
};

inline static void multRow(double* mat, int row, double mult)  {
	mat[row] *= mult;
	mat[row + 4] *= mult;
	mat[row + 8] *= mult;
	mat[row + 12] *= mult;
};


inline Matrix invert(const Matrix& matrix) {
	// This does a Gauss-Jordan elimination.  Not fully tested.  May
	// produce
	// bizarre results if given non-invertible matrix, though 
	// behavior is just to return I.

	double left[16];
	double right[16];
	double temp;

	Matrix().unpack(right);
	matrix.unpack(left);

	int x, y;

	for (x = 0; x<4; x++) {
		y = x + 1;
		// Juxtapose things so that the current row (x) has a one in the 
		// xth column.

		while (fabs(left[x * 5]) < EPSILON) {
			if (y>3) return Matrix();
			addRowToRow(left, y, x);
			addRowToRow(right, y, x);
			y++;
		}

		temp = left[x * 5];
		multRow(left, x, 1 / temp);
		multRow(right, x, 1 / temp);

		// Eliminate the rest of the rows for that column.

		for (y = 0; y<4; y++)
			if (abs(y - x) > EPSILON) {
				temp = left[y + (x * 4)];
				if (fabs(temp) > EPSILON) {
					subMultRow(left, x, y, temp);
					subMultRow(right, x, y, temp);
				}
			}
	}

	return Matrix(right);
}

// Returns the inverse matrix of rot_mat()
inline Matrix inv_rot_mat(Point &p, Vector &v, double a){
	Matrix m = rot_mat(p, v, a);
	return (invert(m));
};

inline Vector calcNormal(Point p1, Point p2, Point p3){
	Vector u = p2 - p1;
	Vector v = p3 - p1;
	Vector n = cross(u, v);

	n.normalize();

	return n;
};

inline std::ostream& operator<<(std::ostream& os, const Vector& v){
    os << "X: " << v[0] << " Y: " << v[1] << " Z: " << v[2] << "\n";
    return os;
};
#endif
