/*
This code was not written by me. It was written by my graphics professor
and given to us for our assignments. Comments were written by me.

Author: Dr. Gregory Michael Poor
*/

#include "Camera.h"
#include "TGMathUtils.h"
#include <gmtl/Generate.h>


Camera::Camera() {
	Reset();
}

Camera::~Camera() {
}

void Camera::Reset() {
   // TODO: set up a default  position,  uvw  coordinate,  near-far  planes,  viewangle, and aspect ratio.
	SetViewAngle(VIEW_ANGLE);
	SetNearPlane(NEAR_PLANE);
	SetFarPlane(FAR_PLANE);
	
	Orient(Point3f(0, 0, 1), Vec3f(0, 0, -1), Vec3f(0, 1, 0));
}

void Camera::Orient(Point3f& eye, Point3f& focus, Vec3f& up) {
	Vec3f lookVec3f (focus - eye);
	Orient(eye, lookVec3f, up);
}


void Camera::Orient(Point3f& eye, Vec3f& look, Vec3f& up) {
	Matrix44f orient;

	Vec3f lookVec3f = makeNormal(look);
	m_n = -lookVec3f;
	m_u = makeNormal(makeCross(lookVec3f, up));
	m_v = makeCross(m_n, m_u);

	orient(0,0) = m_u[0]; orient(0,1) = m_u[1]; orient(0,2) = m_u[2];
	orient(1,0) = m_v[0]; orient(1,1) = m_v[1]; orient(1,2) = m_v[2];
	orient(2,0) = m_n[0]; orient(2,1) = m_n[1]; orient(2,2) = m_n[2];

	m_worldToCamera = orient * make_inv_trans_mat(Vec3f(eye[0], eye[1], eye[2]));
	m_cameraToWorld = make_trans_mat(Vec3f(eye[0], eye[1], eye[2])) * transpose(orient);
}

Matrix44f Camera::GetProjectionMatrix()
{
    Matrix44f scaleM;
    scaleM[0][0] = (1.0f / (tan(m_viewAngle * Math::PI / 360.0f)*
        m_farPlane*m_screenWidthRatio));
    scaleM[0][1] = 0.0f; scaleM[0][2] = 0.0f; scaleM[0][3] = 0.0f;

    scaleM[1][0] = 0.0f;
    scaleM[1][1] = 1.0f / (tan(m_viewAngle * Math::PI / 360.0f)*m_farPlane);
    scaleM[1][2] = 0.0f; scaleM[1][3] = 0.0f;

    scaleM[2][0] = 0.0f; scaleM[2][1] = 0.0f;
    scaleM[2][2] = 1.0f / m_farPlane;
    scaleM[2][3] = 0.0f;

	double c = -m_nearPlane / m_farPlane;
    Matrix44f unhingeM;
    unhingeM[2][2] = -1 / (c + 1);
    unhingeM[2][3] = c / (c + 1);
    unhingeM[3][2] = -1.0f;
    unhingeM[3][3] = 0.0f;
	Matrix44f perspective = unhingeM*scaleM;

	return perspective;
}


void Camera::SetViewAngle (double viewAngle) {
	m_viewAngle = viewAngle;
	m_filmPlanDepth = -1/tan((Math::deg2Rad(m_viewAngle))/2.0);
}

void Camera::SetNearPlane (double nearPlane) {
	m_nearPlane = nearPlane;
}

void Camera::SetFarPlane (double farPlane) {
	m_farPlane = farPlane;
}

void Camera::SetScreenSize (int screenWidth, int screenHeight) {
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	m_screenWidthRatio = (double)m_screenWidth/(double)m_screenHeight;
	//m_screenHeightRatio = (double)m_screenHeight/(double)m_screenHeight;
}

Matrix44f Camera::GetModelViewMatrix() {
    return m_worldToCamera;
}

//Matrix44f Camera::GetInvModelViewMatrix44f() {
//	return m_cameraToWorld;
//}

void Camera::RotateV(double angle) {
	Rotate(GetEyePoint3f(), GetUpVec3f(), angle);
}

void Camera::RotateU(double angle) {
	Rotate(GetEyePoint3f(), makeCross(GetLookVec3f(), GetUpVec3f()), angle);
}

void Camera::RotateW(double angle) {
	Rotate(GetEyePoint3f(), GetLookVec3f(), angle);
}

void Camera::Translate(const Vec3f &v) {
   m_cameraToWorld = m_cameraToWorld * make_trans_mat(v);
   m_worldToCamera = make_inv_trans_mat(v) * m_worldToCamera; 
}


void Camera::Rotate(Point3f p, Vec3f axis, double degrees) {
   double angle = Math::deg2Rad(degrees);
   m_cameraToWorld = make_rot_mat(p, axis, angle) * m_cameraToWorld;
   m_worldToCamera = m_worldToCamera * make_inv_rot_mat(p, axis, angle);
}


Point3f Camera::GetEyePoint3f() {
   return Point3f(m_cameraToWorld(0,3), m_cameraToWorld(1,3), m_cameraToWorld(2, 3)); 
}

Vec3f Camera::GetLookVec3f() {
	return Vec3f(-m_cameraToWorld(0,2), -m_cameraToWorld(1,2), -m_cameraToWorld(2, 2));
}

Vec3f Camera::GetUpVec3f() {
	return Vec3f(m_cameraToWorld(0, 1), m_cameraToWorld(1, 1), m_cameraToWorld(2, 1));
}

double Camera::GetViewAngle() {
	return m_viewAngle;
}

double Camera::GetNearPlane() {
	return m_nearPlane;
}

double Camera::GetFarPlane() {
	return m_farPlane;
}

int Camera::GetScreenWidth() {
	return m_screenWidth;
}

int Camera::GetScreenHeight() {
	return m_screenHeight;
}

double Camera::GetFilmPlanDepth() {
   return m_filmPlanDepth;
}

double Camera::GetScreenWidthRatio() {
   return m_screenWidthRatio;
}
