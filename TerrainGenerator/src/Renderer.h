#include <GL/freeglut.h>
#include <vector>
#include <gmtl/Point.h>
#include <GL/glui.h>
#include <string>

namespace Renderer{
	int w_width;
	int w_height;
	int window;
	float view_angle;

	std::vector<gmtl::Point3f> vertices;

	std::string custom_func = "d = -y";

    void draw(){
        glBegin(GL_TRIANGLES);
        int num_vert = vertices.size();
        for (int i = 0; i < num_vert; i++){
			
			glVertex3f(vertices[i][0], vertices[i][1], vertices[i][2]);
			
        }
		glEnd();
    }

	void display_function(){
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		gluPerspective(view_angle, (float)w_width / (float)w_height, 0.1, 30);


        draw();
		glutSwapBuffers();
	}

	void reshape_function(int x, int y){
		glutReshapeWindow(w_width, w_height);
	}

	void idle(){
		if (glutGetWindow() != window)
			glutSetWindow(window);

		glutPostRedisplay();
	}

	bool init(int window_w, int window_h, float v_angle, int argc = 0, char* argv[] = NULL){
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

		if (window_w > 0){
			w_width = window_w;
		}
		else{
			return false;
		}
		if (window_h > 0){
			w_height = window_w;
		}
		else{
			return false;
		}
		if (v_angle > 0){
			view_angle = v_angle;
		}
		else{
			return false;
		}

		glutInitWindowPosition(100, 100);
		glutInitWindowSize(w_width, w_height);
		window = glutCreateWindow("Procedural Terrain Generation - Zachary Grumbles");
		glutDisplayFunc(display_function);
		glutReshapeFunc(reshape_function);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);
		glPolygonOffset(1, 1);

		vertices = std::vector<gmtl::Point3f>();

		GLUI* g = GLUI_Master.create_glui("Control Panel");
		GLUI_Panel *controls = g->add_panel("Movement");
		(new GLUI_StaticText(controls, "W - Forward"));
		(new GLUI_StaticText(controls, "A - Left"));
		(new GLUI_StaticText(controls, "S - Back"));
		(new GLUI_StaticText(controls, "D - Right"));
		g->add_column_to_panel(controls, true);
		(new GLUI_StaticText(controls, "Q - Up"));
		(new GLUI_StaticText(controls, "E - Down"));
		
		GLUI_EditText *func = g->add_edittext("Custom Density Function", custom_func);
		func->set_w(300);
		func->set_h(20);
		g->add_button("Update");
		g->set_main_gfx_window(window);
		GLUI_Master.set_glutIdleFunc(idle);

		return true;
	}

	void start(){
		glutMainLoop();
	}

	void setVertices(std::vector<gmtl::Point3f> v){
		vertices.clear();
		vertices = v;
	}

    void setDisplayFunction(void(*dFunc)()){
        if (dFunc != NULL){
            glutDisplayFunc(dFunc);
        }
    }


}