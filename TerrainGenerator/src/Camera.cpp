#include "Camera.h"

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
	
	Orient(Point(0, 0, 1), Vector(0, 0, -1), Vector(0, 1, 0));
}

void Camera::Orient(Point& eye, Point& focus, Vector& up) {
	Vector lookVector (focus - eye);
	Orient(eye, lookVector, up);
}


void Camera::Orient(Point& eye, Vector& look, Vector& up) {
	Matrix orient;

	Vector lookVector = normalize(look);
	m_n = -lookVector;
	m_u = normalize(cross(lookVector, up));
	m_v = cross(m_n, m_u);

	orient(0,0) = m_u[0]; orient(0,1) = m_u[1]; orient(0,2) = m_u[2];
	orient(1,0) = m_v[0]; orient(1,1) = m_v[1]; orient(1,2) = m_v[2];
	orient(2,0) = m_n[0]; orient(2,1) = m_n[1]; orient(2,2) = m_n[2];

	m_worldToCamera = orient * inv_trans_mat(Vector(eye[0], eye[1], eye[2]));
	m_cameraToWorld = trans_mat(Vector(eye[0], eye[1], eye[2])) * transpose(orient);
}

Matrix Camera::GetProjectionMatrix()
{
	Matrix scaleM (1.0 / (tan(m_viewAngle * PI / 360.0)*m_farPlane*m_screenWidthRatio), 0, 0, 0,
		0, 1.0 / (tan(m_viewAngle * PI / 360.0)*m_farPlane), 0, 0,
		0, 0, 1.0 / m_farPlane, 0,
		0, 0, 0, 1);
	double c = -m_nearPlane / m_farPlane;
	Matrix unhingeM (1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, -1 / (c + 1), c / (c + 1),
		0, 0, -1, 0);
	Matrix perspective = unhingeM*scaleM;

	return perspective;
}


void Camera::SetViewAngle (double viewAngle) {
	m_viewAngle = viewAngle;
	m_filmPlanDepth = -1/tan((m_viewAngle*RAD)/2.0);
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

Matrix Camera::GetModelViewMatrix() {
    return m_worldToCamera;
}

//Matrix Camera::GetInvModelViewMatrix() {
//	return m_cameraToWorld;
//}

void Camera::RotateV(double angle) {
	Rotate(GetEyePoint(), GetUpVector(), angle);
}

void Camera::RotateU(double angle) {
	Rotate(GetEyePoint(), cross(GetLookVector(), GetUpVector()), angle);
}

void Camera::RotateW(double angle) {
	Rotate(GetEyePoint(), GetLookVector(), angle);
}

void Camera::Translate(const Vector &v) {
   m_cameraToWorld = m_cameraToWorld * trans_mat(v);
   m_worldToCamera = inv_trans_mat(v) * m_worldToCamera; 
}


void Camera::Rotate(Point p, Vector axis, double degrees) {
   double angle = degrees * RAD;
   m_cameraToWorld = rot_mat(p, axis, angle) * m_cameraToWorld;
   m_worldToCamera = m_worldToCamera * inv_rot_mat(p, axis, angle);
}


Point Camera::GetEyePoint() {
   return Point(m_cameraToWorld(0,3), m_cameraToWorld(1,3), m_cameraToWorld(2, 3)); 
}

Vector Camera::GetLookVector() {
	return Vector(-m_cameraToWorld(0,2), -m_cameraToWorld(1,2), -m_cameraToWorld(2, 2));
}

Vector Camera::GetUpVector() {
	return Vector(m_cameraToWorld(0, 1), m_cameraToWorld(1, 1), m_cameraToWorld(2, 1));
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
