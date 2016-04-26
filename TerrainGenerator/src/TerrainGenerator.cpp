#include "TerrainGenerator.h"
#include "tables.h"
#include <gmtl/Vec.h>
#include <noise/noise.h>
#include <stdio.h>

using namespace gmtl;

std::vector<Tri> Voxel::getPolygonAt(gmtl::Point3f start, float voxel_size)
{
	std::vector<Point3f> v = std::vector<Point3f>();
	Point3f next;
	v.push_back(start);	//0 
	v.push_back(Point3f(start[0] + voxel_size, start[1], start[2])); //1
	v.push_back(Point3f(start[0] + voxel_size, start[1], start[2] + voxel_size)); //2
	v.push_back(Point3f(start[0], start[1], start[2] + voxel_size)); //3
	v.push_back(Point3f(start[0], start[1] + voxel_size, start[2])); //4
	v.push_back(Point3f(start[0] + voxel_size, start[1] + voxel_size, start[2])); //5
	v.push_back(Point3f(start[0] + voxel_size, start[1] + voxel_size, start[2] + voxel_size)); //6
	v.push_back(Point3f(start[0], start[1] + voxel_size, start[2] + voxel_size)); //7

	float *val = densityFunction(v);

	int tri_count;
	int index;

	Vec3f vert_list[12];
	Vec3f new_vert_list[12];
	char remap[12];

	index = 0;
	if (val[0] < 0.0f) index |= 1;
	if (val[1] < 0.0f) index |= 2;
	if (val[2] < 0.0f) index |= 4;
	if (val[3] < 0.0f) index |= 8;
	if (val[4] < 0.0f) index |= 16;
	if (val[5] < 0.0f) index |= 32;
	if (val[6] < 0.0f) index |= 64;
	if (val[7] < 0.0f) index |= 128;

	if (edgeTable[index] == 0){
		return std::vector<Tri>();
	}

	if (edgeTable[index] & 1)
		vert_list[0] =
		linear_interp(v[0], v[1], val[0], val[1]);
	if (edgeTable[index] & 2)
		vert_list[1] =
		linear_interp(v[1], v[2], val[1], val[2]);
	if (edgeTable[index] & 4)
		vert_list[2] =
		linear_interp(v[2], v[3], val[2], val[3]);
	if (edgeTable[index] & 8)
		vert_list[3] =
		linear_interp(v[3], v[0], val[3], val[0]);
	if (edgeTable[index] & 16)
		vert_list[4] =
		linear_interp(v[4], v[5], val[4], val[5]);
	if (edgeTable[index] & 32)
		vert_list[5] =
		linear_interp(v[5], v[6], val[5], val[6]);
	if (edgeTable[index] & 64)
		vert_list[6] =
		linear_interp(v[6], v[7], val[6], val[7]);
	if (edgeTable[index] & 128)
		vert_list[7] =
		linear_interp(v[7], v[4], val[7], val[4]);
	if (edgeTable[index] & 256)
		vert_list[8] =
		linear_interp(v[0], v[4], val[0], val[4]);
	if (edgeTable[index] & 512)
		vert_list[9] =
		linear_interp(v[1], v[5], val[1], val[5]);
	if (edgeTable[index] & 1024)
		vert_list[10] =
		linear_interp(v[2], v[6], val[2], val[6]);
	if (edgeTable[index] & 2048)
		vert_list[11] =
		linear_interp(v[3], v[7], val[3], val[7]);

	int num_tri = 0;
	std::vector<Tri> result = std::vector<Tri>();
	for (int i = 0; triTable[index][i] != -1; i += 3){
		Point3f p0 = vert_list[triTable[index][i]];
		Point3f p1 = vert_list[triTable[index][i + 1]];
		Point3f p2 = vert_list[triTable[index][i + 2]];
		result.push_back(Tri(p0, p1, p2));
		num_tri++;
	}

	return result;
}

Point3f Voxel::linear_interp(Point3f p1, Point3f p2, float d1, float d2){
	float x = p1[0] + (-d1 / (d2 - d1)) * (p2[0] - p1[0]);
	float y = p1[1] + (-d1 / (d2 - d1)) * (p2[1] - p1[1]);
	float z = p1[2] + (-d1 / (d2 - d1)) * (p2[2] - p1[2]);

	return Point3f(x, y, z);
}

float* Voxel::densityFunction(std::vector<gmtl::Point3f> verts){
	float* d = new float[8];

	for (int i = 0; i < 8; i++){
		d[i] = -verts[i][1];
	}

	return d;
}

TerrainGenerator::TerrainGenerator(int w, int l, int h, float v_size){
    if (v_size <= 0.0f)
        v_size = VOXEL_SIZE;

    grid_w = w;
    grid_l = l;
	grid_h = h;
    voxel_size = v_size;

	float start_x = ((float)w * v_size) / 2.0f;
	start_x *= -1.0f;

	float start_y = ((float)h * v_size) / 2.0f;
	start_y *= -1.0f;

	float start_z = ((float)l * v_size) / 2.0f;
	start_z *= -1.0f;

	startPoint = Point3f(start_x, start_y, start_z);

	generateHeightMap();

    should_update = true;
}

void TerrainGenerator::generateHeightMap(){
	
    float x_start = -((float)grid_w * voxel_size) / 2.0f;
    float z_start = -((float)grid_l * voxel_size) / 2.0f;
    float x_end = x_start * -1.0f;
    float z_end = z_start * -1.0f;


	utils::NoiseMapBuilderPlane h_map_builder;
	h_map_builder.SetSourceModule(plane_noise);
	h_map_builder.SetDestNoiseMap(height_map);
	h_map_builder.SetDestSize(grid_w, grid_l);
	h_map_builder.SetBounds(x_start, x_end, z_start, z_end);
	h_map_builder.Build();

	utils::RendererImage r;
	utils::Image i;
	r.SetSourceNoiseMap(height_map);
	r.SetDestImage(i);

	r.Render();

	utils::WriterBMP w;
	w.SetSourceImage(i);
	w.SetDestFilename(HEIGHT_MAP);
	w.WriteDestFile();
}

std::vector<Tri> TerrainGenerator::getTriangles(){
	std::vector<Tri> verts = std::vector<Tri>();

	for (int i = 0; i < grid_h; i++){

		float y = startPoint[1] + i * voxel_size;

		for (int j = 0; j < grid_l; j++){
            
			float z = startPoint[2] + j * voxel_size;

			for (int k = 0; k < grid_w; k++){

				float x = startPoint[0] + k * voxel_size;

				std::vector<Tri> v = Voxel::getPolygonAt(Point3f(x, y, z), voxel_size);
				verts.insert(verts.end(), v.begin(), v.end());

			}

		}
	}

	printf("Done\n");
    should_update = false;
	return verts;
}

void TerrainGenerator::setWidth(int w){
    if (w > 0.0f && w != grid_w){
        grid_w = w;
        should_update = true;
    }
}

int TerrainGenerator::getWidth(){
    return grid_w;
}

void TerrainGenerator::setHeight(int h){
    if (h > 0.0f && h != grid_h){
        grid_h = h;
        should_update = true;
    }
}

int TerrainGenerator::getHeight(){
    return grid_h;
}

void TerrainGenerator::setLength(int l){
    if (l > 0.0f && l != grid_l){
        grid_l = l;
        should_update = true;
    }
}

int TerrainGenerator::getLength(){
    return grid_l;
}

void TerrainGenerator::setVoxelSize(float v_size){
    if (v_size > 0.0f && v_size != voxel_size){
        voxel_size = v_size;
        should_update = true;
    }
}

float TerrainGenerator::getVoxelSize(){
    return voxel_size;
}

bool TerrainGenerator::shouldUpdate(){
    return should_update;
}