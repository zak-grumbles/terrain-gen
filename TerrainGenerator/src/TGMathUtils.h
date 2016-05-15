#ifndef TG_MATH_UTILS_H
#define TG_MATH_UTILS_H

#include <gmtl/Vec.h>
#include <gmtl/VecOps.h>
#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>

using namespace gmtl;

inline Matrix44f make_scale_mat(const Vec3f& v){
	Matrix44f scale;
	scale[0][0] = v[0];
	scale[1][1] = v[1];
	scale[2][2] = v[2];
	return scale;
}

inline Matrix44f make_trans_mat(const Vec3f& v){
	Matrix44f trans;
	trans[0][3] = v[0];
	trans[1][3] = v[1];
	trans[2][3] = v[2];
	return trans;
}

inline Matrix44f make_inv_trans_mat(const Vec3f& v){
    Matrix44f trans;
    trans[0][3] = -v[0];
    trans[1][3] = -v[1];
    trans[2][3] = -v[2];
    return trans;
}

inline Matrix44f make_rot_mat(Vec3f v, float deg){
	float a = Math::deg2Rad(deg);
	
	float s = sin(a);
	float c = cos(a);

	if (!isNormalized(v))
		normalize(v);

	Matrix44f rot;
	rot[0][0] = v[0] * v[0] + c * (1.0f - v[0] * v[0]);
	rot[0][1] = v[0] * v[1] * (1.0f - c) - v[2] * s;
	rot[0][2] = v[2] * v[0] * (1.0f - c) + v[1] * s;
	rot[0][3] = 0;

	rot[1][0] = v[0] * v[1] * (1.0f - c) + v[2] * s;
	rot[1][1] = v[1] * v[1] + c * (1.0f - v[1] * v[1]);
	rot[1][2] = v[1] * v[2] * (1.0f - c) - v[0] * s;
	rot[1][3] = 0;

	rot[2][0] = v[2] * v[1] * (1.0f - c) - v[1] * s;
	rot[2][1] = v[1] * v[2] * (1.0f - c) + v[0] * s;
	rot[2][2] = v[2] * v[2] + c * (1.0f - v[2] * v[2]);
	rot[2][3] = 0;

	rot[3][0] = 0;
	rot[3][1] = 0;
	rot[3][2] = 0;
	rot[3][4] = 1.0f;

	return rot;
}

inline Matrix44f make_rot_mat(Point3f &p, Vec3f &v, float a){
    Vec3f nv = makeNormal(v);

    Matrix44f trans_1 = make_trans_mat(Vec3f(p[0], p[1], p[2]));
    Matrix44f trans_2 = make_trans_mat(Vec3f(-p[0], -p[1], -p[2]));

    Matrix44f m;
    m[0][0] = nv[0] * nv[0] + cos(a)*(1.0 - nv[0] * nv[0]);
    m[0][1] = nv[0] * nv[1] * (1.0 - cos(a)) - nv[2] * sin(a);
    m[0][2] = nv[2] * nv[0] * (1.0 - cos(a)) + nv[1] * sin(a);
    m[0][3] = 0.0f;

    m[1][0] = nv[0] * nv[1] * (1.0 - cos(a)) + nv[2] * sin(a);
    m[1][1] = nv[1] * nv[1] + cos(a)*(1.0 - nv[1] * nv[1]);
    m[1][2] = nv[1] * nv[2] * (1.0 - cos(a)) - nv[0] * sin(a);
    m[1][3] = 0.0f;

    m[2][0] = nv[2] * nv[0] * (1.0 - cos(a)) - nv[1] * sin(a);
    m[2][1] = nv[1] * nv[2] * (1.0 - cos(a)) + nv[0] * sin(a);
    m[2][2] = nv[2] * nv[2] + cos(a)*(1.0 - nv[2] * nv[2]);
    m[2][3] = 0.0f;

    m[3][0] = 0.0f;
    m[3][1] = 0.0f;
    m[3][2] = 0.0f;
    m[3][3] = 1.0f;

    return trans_1 * m * trans_2;
}

inline Matrix44f make_inv_rot_mat(Point3f& p, Vec3f&v, float a){
    Matrix44f m = make_rot_mat(p, v, a);
    return makeInvert(m);
}

#endif