#include "Camera.h"
#include <gmtl/Generate.h>
#include <cmath>
#include "utilities.h"

Camera::Camera(){
	Reset();
}

Camera::~Camera(){

}

void Camera::Reset(){
	Orient(Point3f(0, 0, DEFAULT_FOCAL_LENGTH), Point3f(0, 0, 0), Vec3f(0, 1, 0));
	SetViewAngle(VIEW_ANGLE);
	SetNear(NEAR_PLANE);
	SetFar(FAR_PLANE);
	ratio = 1.0f;
}

void Camera::Orient(Point3f& eye, Point3f& focus, Vec3f& up){
	Vec3f look = focus - eye;
	Orient(eye, look, up);
}

void Camera::Orient(Point3f& eye, Vec3f& look, Vec3f& up){
	Matrix44f orient;

	normalize(look);
	w = -look;
	cross(u, look, up);
	normalize(u);
	cross(v, w, u);

	orient[0][0] = u[0]; orient[0][1] = u[1]; orient[0][2] = u[2];
	orient[1][0] = v[0]; orient[1][1] = v[1]; orient[1][2] = v[2];
	orient[2][0] = w[0]; orient[2][1] = w[1]; orient[2][2] = w[2];
	model_view = orient * inv_trans_mat(eye[0], eye[1], eye[2]);
	inv_model_view = trans_mat(eye[0], eye[1], eye[2]) * makeTranspose(orient);
}

Matrix44f Camera::GetProjection(){
	float c = -near_p / far_p;

	float unhinge_vals[] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, (-1 / (c + 1)), (c / (c+1)),
		0, 0, -1, 0
	};

	Matrix44f unhinge = init_mat_44f(unhinge_vals);
	Matrix44f perspective = unhinge * GetScaleMat();

	return perspective;
}

Matrix44f Camera::GetScaleMat(){
	float values[] = {
		(1.0 / (tan(view_angle * Math::PI/360.0)) * far_p * ratio),
		0, 0, 0,

		0, (1.0 / (tan(view_angle * Math::PI / 360.0) * far_p)),
		0, 0,

		0, 0, (1.0 / far_p), 0,

		0, 0, 0, 1
	};

	return init_mat_44f(values);
}

void Camera::SetViewAngle(float v_a){
	view_angle = v_a;
	film_plane_depth = -1 / tan(Math::deg2Rad(view_angle) / 2.0);
}

void Camera::SetNear(float n){
	near_p = n;
}

void Camera::SetFar(float f){
	far_p = f;
}

void Camera::SetScreenSize(int w, int h){
	width = w;
	height = h;
	ratio = (float)w / (float)h;
}

Matrix44f Camera::GetModelView(){
	return model_view;
}

void Camera::RotateV(float angle){
	Rotate(GetEyePoint(), GetUpVector(), angle);
}

void Camera::RotateU(float angle){
	Rotate(GetEyePoint(), makeCross(GetLookVector(), GetUpVector()), angle);
}

void Camera::RotateW(float angle){
	Rotate(GetEyePoint(), GetLookVector(), angle);
}

void Camera::Rotate(Point3f p, Vec3f axis, float deg){
	float angle = Math::deg2Rad(deg);

	inv_model_view = rot_mat(p, axis, angle) * inv_model_view;
	model_view = model_view * inv_rot_mat(p, axis, angle);
}

Point3f Camera::GetEyePoint(){
	return Point3f(inv_model_view(0, 2), inv_model_view(1, 3), inv_model_view(2, 3));
}

Vec3f Camera::GetLookVector(){
	return Vec3f(-inv_model_view(0, 2), -inv_model_view(1, 2), -inv_model_view(2, 2));
}

Vec3f Camera::GetUpVector(){
	return Vec3f(inv_model_view(0, 1), inv_model_view(1, 1), inv_model_view(2, 1));
}

float Camera::GetViewAngle(){
	return view_angle;
}

float Camera::GetNear(){
	return near_p;
}

float Camera::GetFar(){
	return far_p;
}

int Camera::GetScreenWidth(){
	return width;
}

int Camera::GetScreenHeight(){
	return height;
}

float Camera::GetFilmPlaneDepth(){
	return film_plane_depth;
}

float Camera::GetRatio(){
	return ratio;
}

Vec3f Camera::GetRightVector(){
	Vec3f v1 = GetLookVector();
	Vec3f v2 = GetUpVector();
	Vec3f r = Vec3f();
	cross(r, v1, v2);
	return r;
}

Vec3f Camera::GetLeftVector(){
	Vec3f v1 = GetLookVector();
	Vec3f v2 = GetUpVector();
	scale_vec(v2, -1.0f);

	Vec3f l = Vec3f();
	cross(l, v1, v2);

	return l;
}

void Camera::Translate(Vec3f v){
	inv_model_view = inv_model_view * trans_mat(v[0], v[1], v[2]);
	model_view = inv_trans_mat(v[0], v[1], v[2]) * model_view;
}