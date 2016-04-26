#include <GL/freeglut.h>
#include <vector>
#include <gmtl/Point.h>
#include <GL/glui.h>
#include <string>
#include "Tri.h"
#include "Camera.h"

namespace Renderer{
	int w_width;
	int w_height;
	int window;
	float view_angle;

	std::vector<Tri> triangles = std::vector<Tri>();

	std::string custom_func = "-y";

    Camera *c = new Camera();

    enum RENDER_MODE{
        WIRE, FILL
    };

    void draw(){
		int num_tri = triangles.size();
		
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < num_tri; i++){

			for (int j = 0; j < 3; j++){
				glVertex3f(triangles[i][j][0], triangles[i][j][1], triangles[i][j][2]);
			}
			glNormal3f(triangles[i].norm[0], triangles[i].norm[1], triangles[i].norm[2]);

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

		GLUI* g = GLUI_Master.create_glui("Control Panel");
		GLUI_Panel *controls = g->add_panel("Movement");
		(new GLUI_StaticText(controls, "W - Forward"));
		(new GLUI_StaticText(controls, "A - Left"));
		(new GLUI_StaticText(controls, "S - Back"));
		(new GLUI_StaticText(controls, "D - Right"));
		g->add_column_to_panel(controls, true);
		(new GLUI_StaticText(controls, "Q - Up"));
		(new GLUI_StaticText(controls, "E - Down"));
		
		GLUI_EditText *func = g->add_edittext("Custom Density Function d=", custom_func);
		func->set_w(300);
		func->set_h(20);
		g->add_button("Update");
		g->set_main_gfx_window(window);
		GLUI_Master.set_glutIdleFunc(idle);

        c->SetScreenSize(window_w, window_h);

		return true;
	}

	void start(){
		glutMainLoop();
	}

    void setDisplayFunction(void(*dFunc)()){
        if (dFunc != NULL){
            glutDisplayFunc(dFunc);
        }
    }

	void setTriangles(std::vector<Tri> t){
		triangles.clear();
		triangles.insert(triangles.end(), t.begin(), t.end());
	}

    void updateCamOrient(Point3f eye, Vec3f look, Vec3f up){
        c->Orient(eye, look, up);
    }

    void updateCamRot(float rotU, float rotV, float rotW){
        c->RotateV(rotV);
        c->RotateU(rotU);
        c->RotateW(rotW);
    }

    void updateViewAngle(float angle){
        c->SetViewAngle(angle);
    }

    void applyModelView(){
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(c->GetModelView().getData());
    }

    void applyProjection(){
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(c->GetProjection().getData());
    }

    void setRenderMode(RENDER_MODE m){
        switch (m){
        case WIRE:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case FILL:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        }
    }

    Vec3f getCamUpVector(){
        return c->GetUpVector();
    }

    Vec3f getCamRightVector(){
        return c->GetRightVector();
    }
}