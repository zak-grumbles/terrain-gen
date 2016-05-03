#include <GL/glut.h>
#include <math.h>
#include <noise/noise.h>
#include "noiseutils.h"
#include "tables.h"
#include "utilities.h"

using namespace noise;

struct Vec3{
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

struct GLPoint{
	GLfloat x;
	GLfloat y;
	GLfloat z;

	GLPoint(){
		x = y = z = 0.0;
	}
};

static const GLfloat a2fVertexOffset[8][3] =
{
	{ 0.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 }, { 1.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 },
	{ 0.0, 0.0, 1.0 }, { 1.0, 0.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 0.0, 1.0, 1.0 }
};

static const GLint a2iEdgeConnection[12][2] =
{
	{ 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 0 },
	{ 4, 5 }, { 5, 6 }, { 6, 7 }, { 7, 4 },
	{ 0, 4 }, { 1, 5 }, { 2, 6 }, { 3, 7 }
};

static const GLfloat a2fEdgeDirection[12][3] =
{
	{ 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, { -1.0, 0.0, 0.0 }, { 0.0, -1.0, 0.0 },
	{ 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, { -1.0, 0.0, 0.0 }, { 0.0, -1.0, 0.0 },
	{ 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 }
};

static const GLfloat afAmbientWhite[] = { 0.25, 0.25, 0.25, 1.00 };
static const GLfloat afAmbientRed[] = { 0.25, 0.00, 0.00, 1.00 };
static const GLfloat afAmbientGreen[] = { 0.00, 0.25, 0.00, 1.00 };
static const GLfloat afAmbientBlue[] = { 0.00, 0.00, 0.25, 1.00 };
static const GLfloat afDiffuseWhite[] = { 0.75, 0.75, 0.75, 1.00 };
static const GLfloat afDiffuseRed[] = { 0.75, 0.00, 0.00, 1.00 };
static const GLfloat afDiffuseGreen[] = { 0.00, 0.75, 0.00, 1.00 };
static const GLfloat afDiffuseBlue[] = { 0.00, 0.00, 0.75, 1.00 };
static const GLfloat afSpecularWhite[] = { 1.00, 1.00, 1.00, 1.00 };
static const GLfloat afSpecularRed[] = { 1.00, 0.25, 0.25, 1.00 };
static const GLfloat afSpecularGreen[] = { 0.25, 1.00, 0.25, 1.00 };
static const GLfloat afSpecularBlue[] = { 0.25, 0.25, 1.00, 1.00 };

GLint size = 16;
GLfloat step_size = 2.0 / size;
GLfloat target = 48.0;

void idle();
void draw();
void resize(GLsizei, GLsizei);
void keyboard(unsigned char k, int x, int y);

GLfloat sample_1(GLfloat, GLfloat, GLfloat);
GLfloat sample_sin_cos(GLfloat, GLfloat, GLfloat);
GLfloat sample_height_map(GLfloat, GLfloat, GLfloat);
GLfloat(*sample)(GLfloat, GLfloat, GLfloat) = sample_1;

GLvoid MarchingCubes();
GLvoid march_cube(GLPoint, GLfloat);
void init_height_map(GLPoint);
GLPoint start;

utils::NoiseMap map;

std::vector<GLPoint> verts = std::vector<GLPoint>();

void idle(){
	glutPostRedisplay();
}

void draw(){
	glBegin(GL_TRIANGLES);
	glColor3f(0.8, 0.8, 0.8);
	for (int i = 0; i < verts.size(); i++){
		glVertex3f(verts[i].x, verts[i].y, verts[i].z);
		
	}
	glEnd();
}

GLfloat get_offset(GLfloat v1, GLfloat v2, GLfloat target){
	GLdouble d = v2 - v1;
	if (d == 0.0)
		return 0.5;

	return (target - v1) / d;
}

void normalize(Vec3& v){
	GLfloat len = sqrtf(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));
	
	if (len == 1.0)
		return;
	
	v.x /= len;
	v.y /= len;
	v.z /= len;
}

GLfloat sample_1(GLfloat x, GLfloat y, GLfloat z){
	target = 0.0f;
	return (pow(x, 2) + pow(y, 2) + pow(z, 2) - 1.0);
}

GLfloat sample_sin_cos(GLfloat x, GLfloat y, GLfloat z){
	GLfloat h = 20.0 * (sqrt((0.5 - x)*(0.5 - x) + (0.5 - y)*(0.5 - y)));
	h = 1.5 + 0.1 *(sinf(h) + cosf(h));
	return (h - z)*50.0;
}

GLfloat sample_height_map(GLfloat x, GLfloat y, GLfloat z){
	float new_x = normalize_coordinate(x, 1, 0);
	float new_z = normalize_coordinate(z, 1, 0);

	target = map.GetValue(new_x, new_z);

	return y;
}

void march_cube(GLPoint p, GLfloat scale){
	extern GLint aiCubeEdgeFlags[256];
	extern GLint a2iTriangleConnectionTable[256][16];

	GLfloat cube_value[8];
	GLPoint edge_vertex[12];

	for (int vertex = 0; vertex < 8; vertex++){
		cube_value[vertex] = sample(p.x + a2fVertexOffset[vertex][0] * scale,
			p.y + a2fVertexOffset[vertex][1] * scale,
			p.z + a2fVertexOffset[vertex][2] * scale);
	}

	bool yup = false;
	if (target != 0.0){
		yup = true;
	}

	GLint index = 0;
	for (int vert_test = 0; vert_test < 8; vert_test++){
		if (cube_value[vert_test] <= target){
			index |= 1 << vert_test;
		}
	}

	GLint edgeFlags = aiCubeEdgeFlags[index];

	if (edgeFlags == 0)
		return;

	GLfloat offset = 0;
	for (int edge = 0; edge < 12; edge++){
		if (edgeFlags & (1 << edge)){
			offset = get_offset(cube_value[a2iEdgeConnection[edge][0]],
				cube_value[a2iEdgeConnection[edge][1]],
				target);

			edge_vertex[edge].x = p.x + (a2fVertexOffset[a2iEdgeConnection[edge][0]][0] + offset * a2fEdgeDirection[edge][0]) * scale;
			edge_vertex[edge].y = p.y + (a2fVertexOffset[a2iEdgeConnection[edge][0]][1] + offset * a2fEdgeDirection[edge][1]) * scale;
			edge_vertex[edge].z = p.z + (a2fVertexOffset[a2iEdgeConnection[edge][0]][2] + offset * a2fEdgeDirection[edge][2]) * scale;
		}
	}

	for (int tri = 0; tri < 5; tri++){
		if (a2iTriangleConnectionTable[index][3 * tri] < 0)
			break;

		for (int corner = 0; corner < 3; corner++){
			int vertex = a2iTriangleConnectionTable[index][3 * tri + corner];

			//glVertex3f(edge_vertex[vertex].x, edge_vertex[vertex].y, edge_vertex[vertex].z);
			verts.push_back(edge_vertex[vertex]);
			printf("\tVertex at (%f, %f, %f)\n", edge_vertex[vertex].x, edge_vertex[vertex].y, edge_vertex[vertex].z);
		}
	}
}

void MarchingCubes(){
	printf("Hang on, I'm playing God...\n");
	for (int x = 0; x < size; x++){
		for (int y = 0; y < size; y++){
			for (int z = 0; z < size; z++){
				GLPoint p;
				p.x = start.x + x * step_size;
				p.y = start.y + y * step_size;
				p.z = start.z + z * step_size;

				march_cube(p, step_size);
			}
		}
	}
	printf("Alright, it is finished.\n");
}

void init(){
	start.x = (float)size * step_size / 2.0f;
	start.y = start.x;
	start.z = start.x;

	start.x *= -1;
	start.y *= -1;
	start.z *= -1;

	init_height_map(start);
	MarchingCubes();
}

void init_height_map(GLPoint p){
	module::RidgedMulti mountains;
	module::Billow base;
	base.SetFrequency(2.0);

	module::ScaleBias flat;
	flat.SetSourceModule(0, base);
	flat.SetScale(0.125);
	flat.SetBias(-0.75);

	module::Perlin type;
	type.SetFrequency(0.6);
	type.SetPersistence(0.25);

	module::Select f;
	f.SetSourceModule(0, flat);
	f.SetSourceModule(1, mountains); 
	f.SetControlModule(type);
	f.SetBounds(0.0, 1000.0);
	f.SetEdgeFalloff(0.125);
	
	utils::NoiseMapBuilderPlane builder;
	builder.SetSourceModule(f);
	builder.SetDestNoiseMap(map);
	builder.SetDestSize(256, 256);
	builder.SetBounds(0, 2, 0, 2);
	builder.Build();

	utils::RendererImage r;
	utils::Image i;
	r.SetSourceNoiseMap(map);
	r.SetDestImage(i);
	r.ClearGradient();
	r.AddGradientPoint(-1.00, utils::Color(32, 160, 0, 255)); // grass
	r.AddGradientPoint(-0.25, utils::Color(224, 224, 0, 255)); // dirt
	r.AddGradientPoint(0.25, utils::Color(128, 128, 128, 255)); // rock
	r.AddGradientPoint(1.00, utils::Color(255, 255, 255, 255)); // snow
	r.EnableLight();
	r.SetLightContrast(3.0);
	r.SetLightBrightness(2.0);
	r.Render();

	utils::WriterBMP writer;
	writer.SetSourceImage(i);
	writer.SetDestFilename("h_map.bmp");
	writer.WriteDestFile();
}