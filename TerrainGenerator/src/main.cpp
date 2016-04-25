#include "Renderer.h"
#include "TerrainGenerator.h"
#include "Camera.h"
#include "utilities.h"
#include <GL/glui.h>
#include <gmtl/VecOps.h>
#include <gmtl/Generate.h>

int monitor_width;
int monitor_height;
int window_width = 640;
int window_height = 480;
float view_angle = 45.0f;

float eye_x = 0;
float eye_y = 10;
float eye_z = 10;
float look_x = 0;
float look_y = -10;
float look_z = -10;
float rot_u = 0.0f;
float rot_v = 0;
float rot_w = 0;

int wire = 0;

Camera *c;

GLUI_EditText* dFuncText = NULL;

void displayFunction(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	c->SetViewAngle(view_angle);
	glMatrixMode(GL_PROJECTION);
	Matrix44f proj = c->GetProjection();
	glLoadMatrixf(proj.getData());
	c->Orient(Point3f(eye_x, eye_y, eye_z), Vec3f(look_x, look_y, look_z), Vec3f(0, 1, 0));
	c->RotateV(rot_v);
	c->RotateU(rot_u);
	c->RotateW(rot_w);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(c->GetModelView().getData());

	if (wire){
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

    Renderer::draw();
    glutSwapBuffers();
}


void keyboardFunc(unsigned char k, int x, int y){

	Point3f e = Point3f(eye_x, eye_y, eye_z);
	Vec3f l = Vec3f(look_x, look_y, look_z);
	Vec3f r = makeCross(c->GetUpVector(), c->GetUpVector());
	Vec3f u = c->GetUpVector();
	normalize(u);
	normalize(l);
	normalize(r);

	switch (k){
	case 'w':
		scale_vec(l, 0.05f);
		e = add_point_vec(e, l);
		break;
	case 's':
		scale_vec(l, -0.05f);
		e = add_point_vec(e, l);
		break;
	case 'q':
		scale_vec(u, 0.05f);
		e = add_point_vec(e, u);
		break;
	case 'e':
		scale_vec(u, -0.05f);
		e = add_point_vec(e, u);
		break;
	}
	eye_x = e[0];
	eye_y = e[1];
	eye_z = e[2];
}

int main(int argc, char* argv[]){
	c = new Camera();
	c->SetScreenSize(window_width, window_height);
	c->Orient(Point3f(eye_x, eye_y, eye_z), Vec3f(look_x, look_y, look_z), Vec3f(0, 1, 0));

	Renderer::init(window_width, window_height, 45.0f, argc, argv);
	glutKeyboardFunc(keyboardFunc);
	

	TerrainGenerator *tg = new TerrainGenerator(10, 10, 10);
	std::vector<gmtl::Point3f> verts = tg->getVerts();
	Renderer::setVertices(verts);
    Renderer::setDisplayFunction(displayFunction);

    Renderer::start();
	return 0;
}