/*
	Zachary Grumbles

	Class definition for TerrainGenerator class.
*/

#include "TerrainGenerator.h"
#include "tables.h"
#include <GL/glut.h>

using namespace glm;

/*
	Constructor. Set variables.
*/
TerrainGenerator::TerrainGenerator(int num, float res){
	num_cells = num;
	cell_size = res;
	target = 0.0f;
	verts = std::vector<vec3>();
}

/*
	Destructor. Clear vertices.
*/
TerrainGenerator::~TerrainGenerator(){
	verts.clear();
}

/*
	Renders terrain.
*/
void TerrainGenerator::draw(){
	glBegin(GL_TRIANGLES);

	for (int i = 0; i < verts.size(); i++){
		float y = verts[i][1] *10.0;

		//Set vertex color based on height. (This sucks. I need textures)
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

/*
	Initializes other variables and the noise map, then runs algorithm.
*/
void TerrainGenerator::init(){
	start[0] = 0;
	start[1] = start[0];
	start[2] = start[0];

	//generate noise map and run algorithm
	init_noise_map(start);
	MarchingCubes();
}

/*
	Sets the cell size. Bad name, I know.
*/
void TerrainGenerator::setCellSize(float s){
	if (cell_size != s)
		cell_size = s;
}

/*
	Runs the algorithm over the entire cell grid.
*/
void TerrainGenerator::MarchingCubes(){

	verts.clear();

	//iterate over the grid, running algorithm on each cube.
	printf("Rendering %ix%ix%i cubes. . .\n", num_cells, num_cells, num_cells);
	for (int x = 0; x < num_cells; x++){
		if (x % 128 == 0){
			printf("%i ", x);
		}
		if (x % 1024 == 0 & x != 0){
			printf("\n");
		}
		for (int y = 0; y < num_cells; y++){
			for (int z = 0; z < num_cells; z++){
				vec3 p;
				p[0] = start[0] + x * cell_size;
				p[1] = start[1] + y * cell_size;
				p[2] = start[2] + z * cell_size;

				march_cube(p);
			}
		}
	}
	printf("\nTriangles Rendered: %i\nVertices Rendered: %i\n", verts.size() / 3, verts.size());
	printf("==============================\n");
}

/*
	Sets the seed values.
*/
void TerrainGenerator::setSeed(float a, float b, float c, float d){
	//set x sample ranges
	if (a < b){
		seeds[0] = a;
		seeds[1] = b;
	}
	else{
		seeds[0] = b;
		seeds[1] = a;
	}

	//set y sample ranges
	if (c < d){
		seeds[2] = c;
		seeds[3] = d;
	}
	else{
		seeds[2] = d;
		seeds[3] = c;
	}
}

/*
	Sample the noise map at a given point.
*/
float TerrainGenerator::sample(vec3 p){
	//get height at the given point
	return (map.GetValue(p[0], p[2])) - p[1];
}

/*
	Run the algorithm for a given cube with the bottom, front, left vertex at p.
*/
void TerrainGenerator::march_cube(vec3 p){
	float cube_value[8];
	vec3 edge_vertex[12];

	//get value at corners of cube.
	for (int vertex = 0; vertex < 8; vertex++){

		//temp represents the current corner. 
		//it takes the value of the starting point, p, plus the current
		//offset as given by the offsets table in tables.h
		vec3 temp = vec3(
			p[0] + offsets[vertex][0] * cell_size,
			p[1] + offsets[vertex][1] * cell_size,
			p[2] + offsets[vertex][2] * cell_size
		);

		//sample
		cube_value[vertex] = sample(temp);
	}

	//determine edge table index based on values at corners
	int index = 0;
	for (int test = 0; test < 8; test++){
		if (cube_value[test] > target){
			index |= 1 << test;
		}
	}

	int edgeFlags = edge_table[index];
	
	//if edgeFlags is 0, there are no faces in this cube
	if (edgeFlags == 0)
		return;

	//this loop determines if a given edge of the cube should contain a vertex and,
	//if so, the position of that vertex.
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

	//Based on the triangle table in tables.h, determine which of our
	//place our generated vertices in the vertex vector in the proper order.
	//iterating up to 5 because there will only ever be a maximum of 5
	//triangles in a given cube.
	for (int tri = 0; tri < 5; tri++){

		//if we reach a negative number, we have already found all
		//of our triangles.
		if (triTable[index][3 * tri] < 0)
			break;

		//add the 3 vertices of the triangle.
		for (int corner = 0; corner < 3; corner++){
			int vert = triTable[index][3 * tri + corner];

			verts.push_back(edge_vertex[vert]);
		}
	}
}

/*
	Generate the noise map.

	For more info on the functions/objects used here, check the
	libnoise documentation. They've got some cool stuff over there.
*/
void TerrainGenerator::init_noise_map(vec3 p){
	//create a perlin module and a noise map builder.
	module::Perlin mod;
	utils::NoiseMapBuilderPlane builder;

	//set the builder's source and destination
	builder.SetSourceModule(mod);
	builder.SetDestNoiseMap(map);

	//the noise map should be the same dimensions (x and z) as our
	//grid.
	builder.SetDestSize(num_cells, num_cells);

	//set sampling ranges
	builder.SetBounds(seeds[0], seeds[1], seeds[2], seeds[3]);

	//generate that stuff, boi.
	builder.Build();
}

/*
	Get the offset between two values.
*/
float TerrainGenerator::get_offset(float a, float b){
	double d = b - a;

	//are these points the same?
	//if so, return 0.5
	if (d == 0.0)
		return 0.5;

	return (target - a) / d;
}