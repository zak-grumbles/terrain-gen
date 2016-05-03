#ifndef SAMPLE_FUNCTIONS_H
#define SAMPLE_FUNCTIONS_H

#include <gmtl/Point.h>
#include <GL/glut.h>

using namespace gmtl;

#define SPHERE_TARGET 0.5f
inline void SphereFunction(GLfloat& f, Point3f p){
	float x_2 = p[0] * p[0];
	float y_2 = p[1] * p[1];
	float z_2 = p[2] * p[2];

	f = x_2 + y_2 + z_2 - 0.5f;
}


inline void SinCosFunction(GLfloat& f, Point3f p){
	GLfloat h = 20.0*(sqrt((0.5 - p[0]) * (0.5 - p[0]) + (0.5 - p[1]) * (0.5*p[1])));
	h = 1.5 + 0.1*(sinf(h) + cosf(h));
	GLdouble result = (h - p[2]) * 50.0;

	f = result;
}
#endif