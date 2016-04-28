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
float v_angle = 45.0f;

float eye_x = 0;
float eye_y = 1;
float eye_z = 5;
float look_x = 0;
float look_y = 0;
float look_z = -1;
float rot_u = 0.0f;
float rot_v = 0;
float rot_w = 0;

int wire = 0;

TerrainGenerator* tg;

using namespace Renderer;

void displayFunction(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
    updateViewAngle(v_angle);
    applyProjection();

	updateCamOrient(Point3f(eye_x, eye_y, eye_z), Vec3f(look_x, look_y, look_z), Vec3f(0, 1, 0));
    updateCamRot(rot_u, rot_v, rot_w);

    applyModelView();

	if (wire){
        setRenderMode(WIRE);
	}
	else{
        setRenderMode(FILL);
	}

	glColor3f(0.8f, 0.8f, 0.8f);

	setTriangles(tg->getTriangles());

    draw();
    glutSwapBuffers();
}


void keyboardFunc(unsigned char k, int x, int y){

	Point3f e = Point3f(eye_x, eye_y, eye_z);
	Vec3f l = Vec3f(look_x, look_y, look_z);
    Vec3f u = getCamUpVector();
	normalize(u);
	normalize(l);

	Vec3f r;
	Vec3f left;

	switch (k){
	case 'w':
		scale_vec(l, 0.05f);
		e = add_point_vec(e, l);
		break;
	case 'a':
		break;
	case 'd':
        r = getCamRightVector();
		scale_vec(r, 0.05f);
		e = add_point_vec(e, r);
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
	Renderer::init(window_width, window_height, 45.0f, argc, argv);
	glutKeyboardFunc(keyboardFunc);
	

    tg = new TerrainGenerator(100, 100, 100, 0.1f);
    Renderer::setDisplayFunction(displayFunction);

    Renderer::start();
	return 0;
}