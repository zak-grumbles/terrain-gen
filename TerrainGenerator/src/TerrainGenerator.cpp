#include "TerrainGenerator.h"
#include "utilities.h"
#include "tables.h"
#include <GL/glut.h>

TerrainGenerator::TerrainGenerator(int x, int y, int z, float v_size){
	x_count = x;
	y_count = y;
	z_count = z;

	vox_size = v_size;

	x_bound = ((float)x_count * vox_size) / 2.0f;
	y_bound = ((float)y_count * vox_size) / 2.0f;
	z_bound = ((float)z_count * vox_size) / 2.0f;

	start = Point3f(-1.0f * x_bound, -1.0f * y_bound, -1.0f * z_bound);

	update();

	should_update = false;
}

void TerrainGenerator::update(){
	float *top_slab = new float[x_count * y_count];
	float *bot_slab = new float[x_count * y_count];

	this->initSlab(top_slab, z_bound);
	for (int i = 0; i < z_count; i++){
		initSlab(bot_slab, i);
		makePolysFromSlabs(top_slab, bot_slab, i);
		swap_pointers(top_slab, bot_slab);
	}
	delete[] top_slab;
	delete[] bot_slab;
}

void TerrainGenerator::initSlab(float *s, int slab_num){
	for (int i = 0; i < x_count; i++){

		for (int j = 0; j < y_count; j++){

			Point3f p = Point3f(start[0] + i * vox_size,
				start[1] + j * vox_size,
				start[2] + slab_num * vox_size);
			s[i * y_count + j] = densityFunction(p);

		}
	}
}

void TerrainGenerator::makePolysFromSlabs(float *top, float *bot, int slab_num){

	for (int i = 0; i < x_count; i++){

		for (int j = 0; j < y_count; j++){

			Voxel v;
			v.p[0] = Point3f(start[0] + vox_size * i, start[1] + vox_size * j, start[2] + vox_size * slab_num);
			v.p[1] = Point3f(start[0] + vox_size * (i + 1), start[1] + vox_size * j, start[2] + vox_size * slab_num);
			v.p[2] = Point3f(start[0] + vox_size * (i + 1), start[1] + vox_size * (j + 1), start[2] + vox_size * slab_num);
			v.p[3] = Point3f(start[0] + vox_size * i, start[1] + vox_size * (j + 1), start[2] + vox_size * slab_num);

			v.d[0] = top[i*y_count + j];
			v.d[1] = top[(i + 1)*y_count + j];
			v.d[2] = top[(i + 1)*y_count + j + 1];
			v.d[3] = top[i*y_count + j + 1];

			v.p[4] = Point3f(start[0] + vox_size * i, start[1] + vox_size * j, start[2] + vox_size * (slab_num + 1));
			v.p[5] = Point3f(start[0] + vox_size * (i + 1), start[1] + vox_size * j, start[2] + vox_size * (slab_num + 1));
			v.p[6] = Point3f(start[0] + vox_size * (i + 1), start[1] + vox_size * (j + 1), start[2] + vox_size * (slab_num + 1));
			v.p[7] = Point3f(start[0] + vox_size * i, start[1] + vox_size * (j + 1), start[2] + vox_size * (slab_num + 1));

			v.d[4] = bot[i*y_count + j];
			v.d[5] = bot[(i + 1)*y_count + j];
			v.d[6] = bot[(i + 1)*y_count + j + 1];
			v.d[7] = bot[i * y_count + j + 1];

			bool its_all_good = true;
			for (int k = 0; k < 8 && its_all_good; k++){
				if (v.d[k] == FLT_MAX)
					its_all_good = false;
			}
			if (its_all_good){
				addTriangles(v);
			}
		}
	}
}

void TerrainGenerator::addTriangles(Voxel v){
	Point3f v_list[12];
	Point3f new_v_list[12];

	int index = 0;
	if (v.d[0] == 0.0f) 
		index |= 1;
	if (v.d[1] == 0.0f)
		index |= 2;
	if (v.d[2] == 0.0f)
		index |= 4;
	if (v.d[3] == 0.0f)
		index |= 8;
	if (v.d[4] == 0.0f)
		index |= 16;
	if (v.d[5] == 0.0f)
		index |= 32;
	if (v.d[6] == 0.0f)
		index |= 64;
	if (v.d[7] == 0.0f)
		index |= 128;

	if (edgeTable[index] == 0)
		return;

	int edge_val = edgeTable[index];

	if (edge_val & 1)
		v_list[0] = linear_interp(v.p[0], v.p[1], v.d[0], v.d[1]);
	if (edge_val & 2)
		v_list[1] = linear_interp(v.p[1], v.p[2], v.d[1], v.d[2]);
	if (edge_val & 4)
		v_list[2] = linear_interp(v.p[2], v.p[3], v.d[2], v.d[3]);
	if (edge_val & 8)
		v_list[3] = linear_interp(v.p[3], v.p[0], v.d[3], v.d[0]);
	if (edge_val & 16)
		v_list[4] = linear_interp(v.p[4], v.p[5], v.d[4], v.d[5]);
	if (edge_val & 32)
		v_list[5] = linear_interp(v.p[5], v.p[6], v.d[5], v.d[6]);
	if (edge_val & 64)
		v_list[6] = linear_interp(v.p[6], v.p[7], v.d[6], v.d[7]);
	if (edge_val & 128)
		v_list[7] = linear_interp(v.p[7], v.p[4], v.d[7], v.d[4]);
	if (edge_val & 256)
		v_list[8] = linear_interp(v.p[0], v.p[4], v.d[0], v.d[4]);
	if (edge_val & 512)
		v_list[9] = linear_interp(v.p[1], v.p[5], v.d[1], v.d[5]);
	if (edge_val & 1024)
		v_list[10] = linear_interp(v.p[2], v.p[6], v.d[2], v.d[6]);
	if (edge_val & 2048)
		v_list[11] = linear_interp(v.p[3], v.p[7], v.d[3], v.d[7]);

	int new_count = 0;
	char remap[] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };

	for (int i = 0; triTable[index][i] != -1; i++){

		if (remap[triTable[index][i]] == -1){
			new_v_list[new_count] = v_list[triTable[index][i]];
			remap[triTable[index][i]] = new_count;
			new_count++;
		}

	}

	for (int i = 0; i < new_count; i++){
		vert_storage[i] = new_v_list[i];
	}

	int tri_count = 0;
	for (int i = 0; triTable[index][i] != -1; i += 3){
		face_storage[tri_count].i[0] = remap[triTable[index][i + 0]];
		face_storage[tri_count].i[1] = remap[triTable[index][i + 1]];
		face_storage[tri_count].i[2] = remap[triTable[index][i + 2]];
		tri_count++;
	}

	int shift = triangles.size();
	if (tri_count){
		for (int i = 0; i < tri_count; i++){
			face_storage[i].i[0] += shift;
			face_storage[i].i[1] += shift;
			face_storage[i].i[2] += shift;
			triangles.push_back(face_storage[i]);
		}
		for (int j = 0; j < new_count; j++){
			verts.push_back(vert_storage[j]);
		}
	}
}

float TerrainGenerator::densityFunction(Point3f p){
	return p[0] * p[0] + p[1] * p[1] + p[2] * p[2] - 1.0f;
}

bool TerrainGenerator::shouldUpdate(){
	return should_update;
}

Mesh* TerrainGenerator::getMesh(){
	Mesh* m = new Mesh(verts.size(), triangles.size());

	Point3f* v = m->Vertices();
	int* i = m->Indices();

	for (int j = 0; j < verts.size(); j++){
		Point3f &m_temp = v[j];
		Point3f tg_temp = verts[j];
		m_temp[0] = tg_temp[0];
		m_temp[1] = tg_temp[1];
		m_temp[2] = tg_temp[2];
	}
	for (int j = 0; j < triangles.size(); j++){
		Tri cur_face = triangles[j];
		i[j * 3] = cur_face.i[0];
		i[j * 3 + 1] = cur_face.i[1];
		i[j * 3 + 2] = cur_face.i[2];
	}

	return m;
}

void TerrainGenerator::draw(){
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < triangles.size(); i++){
		Point3f p1 = verts[triangles[i].i[0]];
		Point3f p2 = verts[triangles[i].i[1]];
		Point3f p3 = verts[triangles[i].i[2]];

		glVertex3f(p1[0], p1[1], p1[2]);
		glVertex3f(p2[0], p2[1], p2[2]);
		glVertex3f(p3[0], p3[1], p3[2]);
	}
	glEnd();
}