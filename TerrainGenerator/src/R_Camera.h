#ifndef R_CAMERA_H
#define R_CAMERA_H

#include <GL/glut.h>
#include "Camera.h"

namespace Renderer{
	Camera *c = new Camera();

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
}

#endif