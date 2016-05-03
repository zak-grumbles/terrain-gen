#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include <noise/noise.h>
#include <SOIL/SOIL.h>
#include "noiseutils.h"
#include "tables.h"
#include "utilities.h"
#include "ShaderManager.h"

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
	GLfloat r;
	GLfloat g;
	GLfloat b;

	GLPoint(){
		x = y = z = 0.0;
		r = g = b = 0.0;
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

static GLfloat height_map[256][256];

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

GLint size = 512;
GLfloat step_size = 512 / size;
GLfloat target = 0.0;

void idle();
void draw();
void resize(GLsizei, GLsizei);
void keyboard(unsigned char k, int x, int y);

GLfloat sample_1(GLfloat, GLfloat, GLfloat);
GLfloat sample_sin_cos(GLfloat, GLfloat, GLfloat);
GLfloat sample_height_map(GLfloat, GLfloat, GLfloat);
GLfloat(*sample)(GLfloat, GLfloat, GLfloat) = sample_height_map;
void setResolution(float);

GLvoid MarchingCubes();
GLvoid march_cube(GLPoint, GLfloat);
void init_height_map(GLPoint);
GLPoint start;
GLfloat s0, s1, s2, s3;

utils::NoiseMap map;
utils::Image image;

std::vector<GLPoint> verts = std::vector<GLPoint>();

GLuint grass[2];
unsigned char* grass_tex;
unsigned char* grass_bmp;
GLint grass_shader;

Vec3 lightPos;

void idle(){
	glutPostRedisplay();
}

void draw(){
	glBegin(GL_TRIANGLES);

	for (int i = 0; i < verts.size(); i++){
		float y = verts[i].y *10.0;

		if (y < 0.5f)
			glColor3f(0.871f, 0.878f, 0.706f);
		else if (y > 0.5f && y <= 5.0f)
			glColor3f(0.114f, 0.420f, 0.153f);
		else if (y > 5.0f && y <= 7.0f){
			glColor3f(0.710f, 0.710f, 0.710f);
		}
		else if (y > 7.0f)
			glColor3f(0.969f, 1.0f, 0.980f);

		glVertex3f(verts[i].x, verts[i].y*10.0, verts[i].z);
		
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
	return (map.GetValue(x, z)) - y;
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
		if (cube_value[vert_test] > target){
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
			
			utils::Color c = image.GetValue(p.x, p.z);
			edge_vertex[edge].r = c.red;
			edge_vertex[edge].g = c.green;
			edge_vertex[edge].b = c.blue;
		}
	}

	for (int tri = 0; tri < 5; tri++){
		if (a2iTriangleConnectionTable[index][3 * tri] < 0)
			break;

		for (int corner = 0; corner < 3; corner++){
			int vertex = a2iTriangleConnectionTable[index][3 * tri + corner];

			//glVertex3f(edge_vertex[vertex].x, edge_vertex[vertex].y, edge_vertex[vertex].z);
			verts.push_back(edge_vertex[vertex]);
			//printf("\tVertex at (%f, %f, %f)\n", edge_vertex[vertex].x, edge_vertex[vertex].y, edge_vertex[vertex].z);
		}
	}
}

void MarchingCubes(){
	verts.clear();
	printf("Hang on, I'm playing God...\n");
	printf("Size is %i\n", size);
	for (int x = 0; x < size; x++){
		if (x % 100 == 0){
			printf("%i ", x);
		}
		if (x % 500 == 0 & x != 0){
			printf("\n");
		}
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
	printf("Alright, it is finished.\nRendered %i vertices.\n", verts.size());
}

void init(){
	start.x = 0;
	start.y = start.x;
	start.z = start.x;

	init_height_map(start);
	MarchingCubes();

	ShaderManager m = ShaderManager();
	grass_shader = m.initShader("shaders/vert.glsl", "shaders/terrain_frag.glsl");

	glGenTextures(2, grass);

	int w, h;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, grass[0]);
	grass_tex = SOIL_load_image("textures/grass.jpg", &w, &h, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, grass_tex);
	SOIL_free_image_data(grass_tex);
	glUniform1i(glGetUniformLocation(grass_shader, "grass"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, grass[1]);
	grass_bmp = SOIL_load_image("textures/grass_n.png", &w, &h, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, grass_bmp);
	SOIL_free_image_data(grass_bmp);
	glUniform1i(glGetUniformLocation(grass_shader, "grass_bmp"), 1);

	lightPos.x = 2;
	lightPos.y = 2;
	lightPos.z = 22;

	glUniform3f(glGetUniformLocation(grass_shader, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	//glUseProgram(grass_shader);
}

void init_height_map(GLPoint p){
	module::Perlin mod;
	
	utils::NoiseMapBuilderPlane builder;
	builder.SetSourceModule(mod);
	builder.SetDestNoiseMap(map);
	builder.SetDestSize(size, size);
	builder.SetBounds(s0, s1, s2, s3);
	builder.Build();

	utils::RendererImage renderer;

	renderer.SetSourceNoiseMap(map);
	renderer.SetDestImage(image);
	renderer.ClearGradient();
	renderer.AddGradientPoint(-1.0000, utils::Color(0, 0, 128, 255)); // deeps
	renderer.AddGradientPoint(-0.2500, utils::Color(0, 0, 255, 255)); // shallow
	renderer.AddGradientPoint(0.0000, utils::Color(0, 128, 255, 255)); // shore
	renderer.AddGradientPoint(0.0625, utils::Color(240, 240, 64, 255)); // sand
	renderer.AddGradientPoint(0.1250, utils::Color(32, 160, 0, 255)); // grass
	renderer.AddGradientPoint(0.3750, utils::Color(224, 224, 0, 255)); // dirt
	renderer.AddGradientPoint(0.7500, utils::Color(128, 128, 128, 255)); // rock
	renderer.AddGradientPoint(1.0000, utils::Color(255, 255, 255, 255)); // snow
	renderer.Render();

	utils::WriterBMP w;
	w.SetSourceImage(image);
	w.SetDestFilename("h_map.bmp");
	w.WriteDestFile();
}

void setResolution(float r){
	if (r > 0.0f){
		step_size = r / size;
	}
}

int getVertexCount(){
	return verts.size();
}

void setSeed(float a, float b, float c, float d){
	if (a < b){
		s0 = a;
		s1 = b;
	}
	else{
		s0 = b;
		s1 = a;
	}

	if (c < d){
		s2 = c;
		s3 = d;
	}
	else{
		s2 = d;
		s3 = c;
	}

	init_height_map(start);
}