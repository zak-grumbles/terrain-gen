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