/*
	Zachary Grumbles
*/

#include <GL/glut.h>
#include <GL/glui.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "TerrainGenerator.h"
#include "Camera.h"

//glui & camera variables
int monitor_width;
int monitor_height;
int window_width = 640;
int window_height = 480;
float v_angle = 60.0f;
int window;

float eye_x = 254.4;
float eye_y = 436.6;
float eye_z = 258.9;
float look_x = 0;
float look_y = 0;
float look_z = -1;
float rot_u = -90.0f;
float rot_v = -90.001f;
float rot_w = 0.0;

int wire = 1;

/*Terrain variables*/
float cube_size = 2.0f;

float seed_0 = 0;
float seed_1 = 5;
float seed_2 = 0;
float seed_3 = 5;

Camera *c = new Camera();
GLUI* glui;

TerrainGenerator *tg;

//represents available presets
enum preset{
	NONE = 0,
	DEFAULT,
	MINECRAFTISH,
	SPIRES
};

//current preset
int current = MINECRAFTISH;

/*
	Display function. Render terrain and water.
*/
void displayFunction(){
    c->updateTime();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//apply modelview projection matrix
	glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(c->getProjection()));

	glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(c->getModelview()));
	
	//set wireframe/filled render mode
	if (wire){
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	//render terrain
	tg->draw();

	//render "water"
	glColor3f(0.376f, 0.690f, 0.902f);
	glTranslatef(0.0, -0.6f, 0.0f);
	glScalef(1024.0f, 0.0f, 1024.0f);
	glutSolidCube(1.0f);

	//swap dem buffers
    glutSwapBuffers();
}

/*
	Reshape function.
*/
void reshape(int x, int y){
	float aspect;
	aspect = (float)x / (float)y;
    c->setDimensions(x, y);
	glutPostRedisplay();
}

void clickAndDrag(int x, int y){
    c->rotate(x, y);
}

/*
    Keyboard function
*/
void keyboard(unsigned char k, int x, int y){
    switch (k){
    case 'w':
        c->forward();
        break;
    case 'a':
        c->left();
        break;
    case 's':
        c->back();
        break;
    case 'd':
        c->right();
        break;
    case 'q':
        c->down();
        break;
    case 'e':
        c->up();
        break;
    }
}

/*
	Updates variables based on preset & other input and recalculates terrain
*/
void updateLandscape(int id){

	//if preset selected, set variables accordingly
	switch (current){
	case DEFAULT:
		eye_x = 254.4;
		eye_y = 436.6;
		eye_z = 258.9;
		look_x = 0;
		look_y = 0;
		look_z = -1;
		rot_u = -90.0f;
		rot_v = -90.001f;
		rot_w = 0.0;
		cube_size = 1024.0f / 512.0f;
		seed_0 = 0;
		seed_1 = 5;
		seed_2 = 0;
		seed_3 = 5;
		break;
	case MINECRAFTISH:
		eye_x = 40.2598;
		eye_y = 15.7042;
		eye_z = 25.975;
		look_x = 0;
		look_y = 0;
		look_z = -1;
		rot_u = -11.8;
		rot_v = 150.366;
		rot_w = 0.0;
		cube_size = 100.0f / 512.0f;
		seed_0 = 0;
		seed_1 = 10;
		seed_2 = 0;
		seed_3 = 10;
		break;
	case SPIRES:
		eye_x = 0.0;
		eye_y = 19.2;
		eye_z = 0.0;
		look_x = 1.0;
		look_y = 0;
		look_z = 1.0;
		rot_u = -14.5;
		rot_v = 0.0;
		rot_w = 0.0;
		cube_size = 100.0f / 512.0f;
		seed_0 = 0;
		seed_1 = 50;
		seed_2 = 0;
		seed_3 = 50;
	case NONE:
		break;
	}

	//sync live variables, set terrain generator's variables, and
	//re run algorithm.
	glui->sync_live();
	tg->setSeed(seed_0, seed_1, seed_2, seed_3);
	tg->setCellSize(cube_size);
	tg->init();
}

/*
	Idle function
*/
void idle(){
	glutPostRedisplay();
}

/*
	Main. Set up window and objects.
*/
int main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(750, 750);
	window = glutCreateWindow("Procedural Terrain Generation - Zachary Grumbles");
    

	glutDisplayFunc(displayFunction);
	glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMotionFunc(clickAndDrag);
    glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

	static float one[] = { 1, 1, 1, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, one);

    c->setup(window_width, window_height, 0.01f, 100.0f,
        0.0f, 0.0f, 60.0f, glm::vec3(10, 5, 0));

	glui = GLUI_Master.create_glui("Controls");

	GLUI_Panel *terrain = glui->add_panel("Terrain");
	(new GLUI_Checkbox(terrain, "Wireframe:", &wire));
	(new GLUI_Spinner(terrain, "Cell Size:", &cube_size));
	(new GLUI_Spinner(terrain, "Sample from X=", &seed_0));
	(new GLUI_Spinner(terrain, "to X=", &seed_1));
	(new GLUI_Spinner(terrain, "Sample from Y=", &seed_2));
	(new GLUI_Spinner(terrain, "to Y=", &seed_3));
	glui->add_column_to_panel(terrain);
	(new GLUI_StaticText(terrain, "Presets"));
	GLUI_RadioGroup* presets = new GLUI_RadioGroup(terrain, &current);
	(new GLUI_RadioButton(presets, "None"));
	(new GLUI_RadioButton(presets, "Default"));
	(new GLUI_RadioButton(presets, "Minecraft(ish)"));
	(new GLUI_RadioButton(presets, "Spires"));
	glui->add_button_to_panel(terrain, "Update Terrain", 0, updateLandscape);

	glui->add_column_to_panel(terrain, true);

	glui->set_main_gfx_window(window);
	GLUI_Master.set_glutIdleFunc(idle);

	tg = new TerrainGenerator(512, cube_size);
	tg->setCellSize(cube_size);
	tg->setSeed(seed_0, seed_1, seed_2, seed_3);
	tg->init();

	glutMainLoop();
	return 0;
}