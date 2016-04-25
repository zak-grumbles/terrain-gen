#include "TerrainGenerator.h"
#include "tables.h"
#include <gmtl/Vec.h>
#include <noise/noise.h>
#include <stdio.h>

using namespace gmtl;

std::vector<gmtl::Point3f> Voxel::getPolygonAt(gmtl::Point3f bottom_front_left, float voxel_size)
{
	std::vector<gmtl::Point3f> verts = std::vector<gmtl::Point3f>();

	float x = bottom_front_left[0];
	float y = bottom_front_left[1];
	float z = bottom_front_left[2];

	verts.push_back(bottom_front_left);
	verts.push_back(Point3f(x, y + voxel_size, z));
	verts.push_back(Point3f(x+voxel_size, y + voxel_size, z));
	verts.push_back(Point3f(x+voxel_size, y, z));
	verts.push_back(Point3f(x, y, z - voxel_size));
	verts.push_back(Point3f(x, y + voxel_size, z - voxel_size));
	verts.push_back(Point3f(x + voxel_size, y + voxel_size, z - voxel_size));
	verts.push_back(Point3f(x + voxel_size, y, z - voxel_size));

	float* densities = densityFunction(verts);

	int index = 0;
	if (densities[0] <= 0.0f) index |= 1;
	if (densities[1] <= 0.0f) index |= 2;
	if (densities[2] <= 0.0f) index |= 4;
	if (densities[3] <= 0.0f) index |= 8;
	if (densities[4] <= 0.0f) index |= 16;
	if (densities[5] <= 0.0f) index |= 32;
	if (densities[6] <= 0.0f) index |= 64;
	if (densities[7] <= 0.0f) index |= 128;

	int edgeVal = edgeTable[index];
	if (edgeVal == 0){
		return std::vector<Point3f>();
	}

	std::vector<Point3f> new_verts = std::vector<Point3f>();
	if (edgeVal & 1)
		new_verts.push_back(linear_interp(verts[0], verts[1], densities[0], densities[1]));
	else
		new_verts.push_back(Point3f());
	if (edgeVal & 2)
		new_verts.push_back(linear_interp(verts[1], verts[2], densities[1], densities[2]));
	else
		new_verts.push_back(Point3f());
	if (edgeVal & 4)
		new_verts.push_back(linear_interp(verts[2], verts[3], densities[2], densities[3]));
	else
		new_verts.push_back(Point3f());
	if (edgeVal & 8)
		new_verts.push_back(linear_interp(verts[3], verts[0], densities[3], densities[0]));
	else
		new_verts.push_back(Point3f());
	if (edgeVal & 16)
		new_verts.push_back(linear_interp(verts[4], verts[5], densities[4], densities[5]));
	else
		new_verts.push_back(Point3f());
	if (edgeVal & 32)
		new_verts.push_back(linear_interp(verts[5], verts[6], densities[5], densities[6]));
	else
		new_verts.push_back(Point3f());
	if (edgeVal & 64)
		new_verts.push_back(linear_interp(verts[6], verts[7], densities[6], densities[7]));
	else
		new_verts.push_back(Point3f());
	if (edgeVal & 128)
		new_verts.push_back(linear_interp(verts[7], verts[4], densities[7], densities[4]));
	else
		new_verts.push_back(Point3f());
	if (edgeVal & 256)
		new_verts.push_back(linear_interp(verts[0], verts[4], densities[0], densities[4]));
	else
		new_verts.push_back(Point3f());
	if (edgeVal & 512)
		new_verts.push_back(linear_interp(verts[1], verts[5], densities[1], densities[5]));
	else
		new_verts.push_back(Point3f());
	if (edgeVal & 1024)
		new_verts.push_back(linear_interp(verts[2], verts[6], densities[2], densities[6]));
	else
		new_verts.push_back(Point3f());
	if (edgeVal & 2048)
		new_verts.push_back(linear_interp(verts[3], verts[7], densities[3], densities[7]));
	else
		new_verts.push_back(Point3f());

	int new_count = 0;
	int remap[] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };


	std::vector<Point3f> final_verts = std::vector<Point3f>();
	for (int i = 0; triTable[index][i] != -1; i++){
		if (remap[triTable[index][i]] == -1){
			final_verts.push_back(verts[triTable[index][i]]);
			remap[triTable[index][i]] = new_count;
			new_count++;
		}
	}

	return final_verts;
}

Point3f Voxel::linear_interp(Point3f p1, Point3f p2, float d1, float d2){
	float x = p1[0] + (-d1 / (d2 - d1)) * (p2[0] - p1[0]);
	float y = p1[1] + (-d1 / (d2 - d1)) * (p2[1] - p1[1]);
	float z = p1[2] + (-d1 / (d2 - d1)) * (p2[2] - p1[2]);

	return Point3f(x, y, z);
}

float* Voxel::densityFunction(std::vector<gmtl::Point3f> verts){
	float* d = new float[8];
	noise::module::Perlin mod;


	for (int i = 0; i < 8; i++){
		d[i] = -verts[i][1];
		//d[i] += 0.0f * (mod.GetValue(verts[i][0] + 0.1f, verts[i][1] + 0.1f, verts[i][2] + 0.1f));
	}

	return d;
}

Cell::Cell(gmtl::Point3f bottom_front_left, int vCount){
    voxel_count = vCount;
    voxel_size = CELL_SIZE / (float)voxel_count;

    int x = bottom_front_left[0];
    int y = bottom_front_left[1];
    int z = bottom_front_left[2];

    for (int i = 0; i < voxel_count; i++){
        x += i * voxel_size;



        for (int j = 0; j < voxel_count; j++){
            y += j * voxel_size;

            for (int k = 0; k < voxel_count; k++){
                z += k * voxel_size;

                std::vector<gmtl::Point3f> v = Voxel::getPolygonAt(
                    gmtl::Point3f(x, y, z), voxel_size);
                vertices.insert(vertices.end(), v.begin(), v.end());
            }

        }

    }
}

TerrainGenerator::TerrainGenerator(int w, int l, int h){
    grid_w = w;
    grid_l = l;
	grid_h = h;
    
	float start_x = ((float)w * VOXEL_SIZE) / 2.0f;
	start_x *= -1.0f;

	float start_y = ((float)h * VOXEL_SIZE) / 2.0f;
	start_y *= -1.0f;

	float start_z = ((float)l * VOXEL_SIZE) / 2.0f;
	start_z *= -1.0f;

	startPoint = Point3f(start_x, start_y, start_z);
}

std::vector<Point3f> TerrainGenerator::getVerts(){
	std::vector<Point3f> verts = std::vector<Point3f>();

	for (int i = 0; i < grid_h; i++){

		float y = startPoint[1] + i * VOXEL_SIZE;

		for (int j = 0; j < grid_l; j++){

			float z = startPoint[2] + j * VOXEL_SIZE;

			for (int k = 0; k < grid_w; k++){

				float x = startPoint[0] + k * VOXEL_SIZE;

				std::vector<Point3f> v = Voxel::getPolygonAt(Point3f(x, y, z), VOXEL_SIZE);
				verts.insert(verts.end(), v.begin(), v.end());

			}

		}
	}

	printf("Done\n");

	return verts;
}