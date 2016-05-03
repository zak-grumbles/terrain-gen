#include <GL/glut.h>
#include <GL/glui.h>
#include <gmtl/Generate.h>
#include "MarchingCubes.h"
#include "Camera.h"
#include "SampleFunctions.h"
#include "utilities.h"

int monitor_width;
int monitor_height;
int window_width = 640;
int window_height = 480;
float v_angle = 45.0f;
int window;

float eye_x = 0;
float eye_y = 1;
float eye_z = 20;
float look_x = 0;
float look_y = 0;
float look_z = -1;
float rot_u = 0.0f;
float rot_v = 0;
float rot_w = 0;

int wire = 0;

Camera *c;

void displayFunction(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (wire){
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	//glLoadIdentity();
	//glTranslatef(0.0f, 0.0, -5.0f);
	draw();

    glutSwapBuffers();
}


void keyboardFunc(unsigned char k, int x, int y){
	Vec3f v;
	
	switch (k){
	case 'w':
		v = c->GetLookVector();
		scale_vec(v, 0.1f);
		c->Translate(v);
		break;
	case 'a':
		v = makeCross(c->GetLookVector(), c->GetUpVector());
		scale_vec(v, -0.1f);
		c->Translate(v);
		break;
	case 's':
		v = c->GetLookVector();
		scale_vec(v, -0.1f);
		c->Translate(v);
		break;
	case 'd':
		v = makeCross(c->GetLookVector(), c->GetUpVector());
		//scale_vec(v, 0.1f);
		c->Translate(v);
		break;
	case 'q':
		v = c->GetUpVector();
		//scale_vec(v, -0.1f);
		v = v * -1.0f;
		v = v * 0.1f;
		c->Translate(v);
	case 'e':
		v = c->GetUpVector();
		scale_vec(v, 0.1f);
		c->Translate(v);
	}
	Point3f e = c->GetEyePoint();
	e = c->getWorld2Cam() * e;
	eye_x = e[0];
	eye_y = e[1];
	eye_z = e[2];

}

int main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	window = glutCreateWindow("Procedural Terrain Generation - Zachary Grumbles");

	glutDisplayFunc(displayFunction);
	glutKeyboardFunc(keyboardFunc);
	glutIdleFunc(idle);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	
	
	GLfloat afPropertiesAmbient[] = { 0.50, 0.50, 0.50, 1.00 };
	GLfloat afPropertiesDiffuse[] = { 0.75, 0.75, 0.75, 1.00 };
	GLfloat afPropertiesSpecular[] = { 1.00, 1.00, 1.00, 1.00 };

	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_AMBIENT, afPropertiesAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, afPropertiesDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, afPropertiesSpecular);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);

	glEnable(GL_LIGHT0);

	glMaterialfv(GL_BACK, GL_AMBIENT, afAmbientGreen);
	glMaterialfv(GL_BACK, GL_DIFFUSE, afDiffuseGreen);
	glMaterialfv(GL_FRONT, GL_AMBIENT, afAmbientBlue);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, afDiffuseBlue);
	glMaterialfv(GL_FRONT, GL_SPECULAR, afSpecularWhite);
	glMaterialf(GL_FRONT, GL_SHININESS, 25.0);
	
	c = new Camera();
	c->Orient(Point3f(eye_x, eye_y, eye_z), Vec3f(look_x, look_y, look_z),
		Vec3f(0, 1, 0));

	init();

	glutMainLoop();
	return 0;
}