#include "TerrainGenerator.h"
#include "tables.h"
#include <GL/glut.h>

TerrainGenerator::TerrainGenerator(int num, float res){
	num_cells = num;
	cell_size = res / num;
	target = 0.0f;
	verts = std::vector<Point>();

	update = true;
}

TerrainGenerator::~TerrainGenerator(){
	verts.clear();
}

void TerrainGenerator::draw(){
	glBegin(GL_TRIANGLES);

	for (int i = 0; i < verts.size(); i++){
		float y = verts[i][1] *10.0;

		if (y < 0.5f)
			glColor3f(0.871f, 0.878f, 0.706f);
		else if (y > 0.5f && y <= 5.0f)
			glColor3f(0.114f, 0.420f, 0.153f);
		else if (y > 5.0f && y <= 7.0f){
			glColor3f(0.710f, 0.710f, 0.710f);
		}
		else if (y > 7.0f)
			glColor3f(0.969f, 1.0f, 0.980f);

		glVertex3f(verts[i][0], verts[i][1]*10.0, verts[i][2]);

	}
	glEnd();
}

void TerrainGenerator::init(){
	start[0] = 0;
	start[1] = start[0];
	start[2] = start[0];

	init_height_map(start);
	MarchingCubes();
}

void TerrainGenerator::setResolution(float s){
	float n = s / num_cells;
	if (n != cell_size){
		cell_size = n;
		update = true;
	}
}

void TerrainGenerator::MarchingCubes(){
	if (!update)
		return;

	verts.clear();
	printf("Hang on a moment, I'm playing God. . .\n");
	for (int x = 0; x < num_cells; x++){
		if (x % 100 == 0){
			printf("%i ", x);
		}
		if (x % 500 == 0 & x != 0){
			printf("\n");
		}
		for (int y = 0; y < num_cells; y++){
			for (int z = 0; z < num_cells; z++){
				Point p;
				p[0] = start[0] + x * cell_size;
				p[1] = start[1] + y * cell_size;
				p[2] = start[2] + z * cell_size;

				march_cube(p);
			}
		}
	}
	printf("Alright, it is finished.\nRendered %i vertices.\n", verts.size());
	update = false;
}

void TerrainGenerator::setSeed(float a, float b, float c, float d){
	seeds[0] = a;
	seeds[1] = b;
	seeds[2] = c;
	seeds[3] = d;
	update = true;
}

float TerrainGenerator::sample(Point p){
	return (map.GetValue(p[0], p[2])) - p[1];
}

void TerrainGenerator::march_cube(Point p){
	float cube_value[8];
	Point edge_vertex[12];

	for (int vertex = 0; vertex < 8; vertex++){
		Point temp = Point(
			p[0] + offsets[vertex][0] * cell_size,
			p[1] + offsets[vertex][1] * cell_size,
			p[2] + offsets[vertex][2] * cell_size
		);

		cube_value[vertex] = sample(temp);
	}

	int index = 0;
	for (int test = 0; test < 8; test++){
		if (cube_value[test] > target){
			index |= 1 << test;
		}
	}

	int edgeFlags = edge_table[index];
	if (edgeFlags == 0)
		return;

	float offset = 0;
	for (int edge = 0; edge < 12; edge++){
		if (edgeFlags & (1 << edge)){
			offset = get_offset(cube_value[edge_connection[edge][0]],
				cube_value[edge_connection[edge][1]]);

			edge_vertex[edge][0] = p[0] + (offsets[edge_connection[edge][0]][0]
				+ offset * edge_direction[edge][0]) * cell_size;
			edge_vertex[edge][1] = p[1] + (offsets[edge_connection[edge][0]][1]
				+ offset * edge_direction[edge][1]) * cell_size;
			edge_vertex[edge][2] = p[2] + (offsets[edge_connection[edge][0]][2]
				+ offset * edge_direction[edge][2]) * cell_size;
		}
	}

	for (int tri = 0; tri < 5; tri++){
		if (triTable[index][3 * tri] < 0)
			break;

		for (int corner = 0; corner < 3; corner++){
			int vert = triTable[index][3 * tri + corner];

			verts.push_back(edge_vertex[vert]);
		}
	}
}

void TerrainGenerator::init_height_map(Point p){
	module::Perlin mod;
	utils::NoiseMapBuilderPlane builder;
	builder.SetSourceModule(mod);
	builder.SetDestNoiseMap(map);
	builder.SetDestSize(num_cells, num_cells);
	builder.SetBounds(seeds[0], seeds[1], seeds[2], seeds[3]);
	builder.Build();
}

float TerrainGenerator::get_offset(float a, float b){
	double d = b - a;
	if (d == 0.0)
		return 0.5;

	return (target - a) / d;
}