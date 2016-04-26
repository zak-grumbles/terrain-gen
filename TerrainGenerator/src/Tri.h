#ifndef TRI_H
#define TRI_H

#include <gmtl/Point.h>
#include <gmtl/Vec.h>
#include <gmtl/VecOps.h>

using namespace gmtl;

struct Tri{
	Point3f verts[3];
	Vec3f norm;

	Tri(Point3f p1, Point3f p2, Point3f p3){
		verts[0] = p1;
		verts[1] = p2;
		verts[2] = p3;

		Vec3f v1 = p2 - p1;
		Vec3f v2 = p3 - p1;

		cross(norm, v1, v2);
		normalize(norm);
	}

	Tri(){
		verts[0] = verts[1] = verts[2] = Point3f(0, 0, 0);
	}

	Point3f operator[](const int i){
		return verts[i];
	}
};

#endif /*TRI_H*/