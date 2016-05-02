#include "Mesh.h"
#include <GL/glut.h>

Mesh::Mesh(){
	//do nothing
}

Mesh::Mesh(int vert_count, int ind_count){
	num_vert = vert_count;
	num_indices = ind_count;

	vertices = new Point3f[num_vert];
	indices = new int[num_indices];
}

Mesh::~Mesh(){
	if (vertices != NULL)
		delete[] vertices;
	if (indices != NULL)
		delete[] indices;
}

Point3f* Mesh::Vertices(){
	return vertices;
}

int* Mesh::Indices(){
	return indices;
}

void Mesh::render(){
	for (int i = 0; i < num_indices; i++){
		Point3f p = vertices[indices[i]];
		glVertex3f(p[0], p[1], p[2]);
	}
}

int Mesh::VertexCount(){
	return num_vert;
}

int Mesh::IndexCount(){
	return num_indices;
}