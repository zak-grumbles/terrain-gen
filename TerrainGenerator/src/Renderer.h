#include <GL/freeglut.h>
#include <vector>
#include <gmtl/Point.h>
#include <gmtl/Generate.h>
#include <string>
#include "Tri.h"
#include "Camera.h"
#include "utilities.h"
#include "Mesh.h"

namespace Renderer{
	int w_width;
	int w_height;
	int window;
	float view_angle;

    Camera *c = new Camera();
	Mesh *m = NULL;

    enum RENDER_MODE{
        WIRE, FILL
    };

	void setMesh(Mesh* new_m){
		m = new_m;
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
		glutReshapeFunc(reshape_function);
		glutIdleFunc(idle);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);
		glPolygonOffset(1, 1);

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

	void moveForward(){
		float dist = 0.1f;
		Vec3f l = c->GetLookVector();
		scale_vec(l, dist);

		c->Translate(l);
	}

	void moveBackward(){
		float dist = 0.1f;
		Vec3f l = c->GetLookVector();
		scale_vec(l, -dist);

		c->Translate(l);
	}

	void moveRight(){
		float dist = 0.1f;
		Vec3f r = makeCross(c->GetLookVector(), c->GetUpVector());
		scale_vec(r, dist);
		c->Translate(r);
	}

	void moveLeft(){
		float dist = 0.1f;
		Vec3f l = makeCross(c->GetLookVector(), c->GetUpVector());
		scale_vec(l, -dist);
		c->Translate(l);
	}

	void moveUp(){
		float dist = 0.1f;
		Vec3f u = c->GetUpVector();
		scale_vec(u, dist);
		c->Translate(u);
	}

	void moveDown(){
		float dist = 0.1f;
		Vec3f d = c->GetUpVector();
		scale_vec(d, -dist);
		c->Translate(d);
	}


}