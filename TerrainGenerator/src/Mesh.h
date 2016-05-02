#ifndef MESH_H
#define MESH_H

#include "Tri.h"
#include <gmtl/Point.h>

using namespace gmtl;

class Mesh{
public:
	Mesh();
	Mesh(int, int);
	~Mesh();

	Point3f* Vertices();
	int* Indices();
	void render();

	int VertexCount();
	int IndexCount();

private:
	Point3f *vertices;
	int *indices;

	int num_vert;
	int num_indices;
};

#endif