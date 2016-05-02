#include "Renderer.h"
#include "TerrainGenerator.h"
#include "Camera.h"
#include "utilities.h"
#include <GL/glui.h>
#include <gmtl/VecOps.h>
#include <gmtl/Generate.h>
#include "Mesh.h"

int monitor_width;
int monitor_height;
int window_width = 640;
int window_height = 480;
float v_angle = 45.0f;

float eye_x = 0;
float eye_y = 0;
float eye_z = 10;
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
	
    applyProjection();
	updateCamOrient(Point3f(eye_x, eye_y, eye_z), Vec3f(look_x, look_y, look_z), Vec3f(0, 1, 0));

    applyModelView();

	if (wire){
        setRenderMode(WIRE);
	}
	else{
        setRenderMode(FILL);
	}

	glColor3f(0.8f, 0.8f, 0.8f);
	tg->draw();

    glutSwapBuffers();
}


void keyboardFunc(unsigned char k, int x, int y){
	switch (k){
	case 'w':
		moveForward();
		break;
	case 'a':
		moveLeft();
		break;
	case 'd':
		moveRight();
		break;
	case 's':
		moveBackward();
		break;
	case 'q':
		moveDown();
		break;
	case 'e':
		moveUp();
		break;
	}
	Point3f e = c->GetEyePoint();
	eye_x = e[0]; eye_y = e[1]; eye_z = e[2];
}

int main(int argc, char* argv[]){
	Renderer::init(window_width, window_height, 45.0f, argc, argv);
	glutKeyboardFunc(keyboardFunc);
	

    tg = new TerrainGenerator(10, 10, 10, 0.2f);
    Renderer::setDisplayFunction(displayFunction);;
    Renderer::start();
	return 0;
}