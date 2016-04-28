#include "TerrainGenerator.h"
#include "utilities.h"
#include "tables.h"

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

	initSlab(top_slab, z_bound);
	for (int i = 1; i < z_count; i++){
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
	if (v.d[0] < 0.0f) index |= 1;
	if (v.d[1] < 0.0f) index |= 2;
	if (v.d[2] < 0.0f) index |= 4;
	if (v.d[3] < 0.0f) index |= 8;
	if (v.d[4] < 0.0f) index |= 16;
	if (v.d[5] < 0.0f) index |= 32;
	if (v.d[6] < 0.0f) index |= 64;
	if (v.d[7] < 0.0f) index |= 128;

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

	std::vector<Tri> new_t = std::vector<Tri>();

	for (int i = 0; i < new_count; i+= 3){
		Point3f p1 = v_list[remap[i]];
		Point3f p2 = v_list[remap[i + 1]];
		Point3f p3 = v_list[remap[i + 2]];

		new_t.push_back(Tri(p1, p2, p3));
	}

	triangles.insert(triangles.end(), new_t.begin(), new_t.end());
}

float TerrainGenerator::densityFunction(Point3f p){
	return -p[1];
}

std::vector<Tri> TerrainGenerator::getTriangles(){
	return triangles;
}

bool TerrainGenerator::shouldUpdate(){
	return should_update;
}