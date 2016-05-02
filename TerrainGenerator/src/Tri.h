#ifndef TRI_H
#define TRI_H

#include <gmtl/Point.h>
#include <gmtl/Vec.h>
#include <gmtl/VecOps.h>

using namespace gmtl;

struct Tri{
	Vec3f norm;
	int i[3];
};

#endif /*TRI_H*/