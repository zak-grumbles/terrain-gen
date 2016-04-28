#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>

using namespace gmtl;


inline Matrix44f init_mat_44f(float* values){
	Matrix44f m;
	m[0][0] = values[0]; m[0][1] = values[1]; m[0][2] = values[2]; m[0][3] = values[3];
	m[1][0] = values[4]; m[1][1] = values[5]; m[1][2] = values[6]; m[1][3] = values[7];
	m[2][0] = values[8]; m[2][1] = values[9]; m[2][2] = values[10]; m[2][3] = values[11];
	m[3][0] = values[12]; m[3][1] = values[13]; m[3][2] = values[14]; m[3][3] = values[15];
	return m;
}

inline Matrix44f inv_trans_mat(float x, float y, float z){
	float values[] = {
		1, 0, 0, -x,
		0, 1, 0, -y,
		0, 0, 1, -z,
		0, 0, 0, 1
	};

	return init_mat_44f(values);
}

inline Matrix44f trans_mat(float x, float y, float z){
	float values[] = {
		1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z,
		0, 0, 0, 1
	};
	return init_mat_44f(values);
}

inline Matrix44f rot_mat(Point3f p, Vec3f axis, float angle){
	Vec3f n = axis;
	normalize(n);

	float c = cos(angle);
	float s = sin(angle);

	float values[] = {
		n[0] * n[0] + c * (1.0 - n[0] * n[0]),
		n[0] * n[1] * (1.0 - c) - n[2] * s,
		n[2] * n[0] * (1.0 - c) + n[1] * s,
		0,

		n[0] * n[1] * (1.0 - c) + n[2] * s,
		n[1] * n[1] + c * (1.0 - n[1] * n[1]),
		n[1] * n[2] * (1.0 - c) - n[0] * s,
		0,

		n[2] * n[0] * (1.0 - c) - n[1] * s,
		n[1] * n[2] * (1.0 - c) + n[0] * s,
		n[2] * n[2] + c * (1.0 - n[2] * n[2]),
		0,

		0,0,0, 1.0
	};
	Matrix44f m = init_mat_44f(values);

	Matrix44f fin = trans_mat(p[0], p[1], p[2]) * m * trans_mat(-p[0], -p[1], -p[2]);
	return fin;
}

inline Matrix44f inv_rot_mat(Point3f p, Vec3f v, float a){
	Matrix44f m = rot_mat(p, v, a);
	invert(m);
	return m;
}


inline Point3f add_point_vec(const Point3f& p, const Vec3f& v){
	return Point3f(p[0] + v[0], p[1] + v[1], p[2] + v[2]);
}

inline void scale_vec(Vec3f& v, float scalar){
	v[0] *= scalar;
	v[1] *= scalar;
	v[2] *= scalar;
}

inline float normalize_coordinate(float a, float max, float min){
	return (1) / (max - min)*(a - max) + 1;
}

inline Point3f linear_interp(Point3f p1, Point3f p2, float d1, float d2){
	float x = p1[0] + (-d1 / (d2 - d1)) * (p2[0] - p1[0]);
	float y = p1[1] + (-d1 / (d2 - d1)) * (p2[1] - p1[1]);
	float z = p1[2] + (-d1 / (d2 - d1)) * (p2[2] - p1[2]);

	return Point3f(x, y, z);
}

inline bool in_range(float a, float b){
	return abs(a - b) < FLT_EPSILON;
}

template <typename T>
inline void swap_pointers(T *p1, T *p2){
	T *temp = p1;
	p1 = p2;
	p2 = temp;
	temp = NULL;
}