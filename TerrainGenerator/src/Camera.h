/*
	Most of these methods are taken from a Camera implementation given
    to me by my graphics professor. I have changed the class to use the
    GMTL library for vectors, points, and matrices and added additional
    movement functions.

	Authors: Dr. Gregory Michael Poor, Not a Dr. Zachary Grumbles
*/


#ifndef CAMERA_H
#define CAMERA_H

#include <gmtl/Point.h>
#include <gmtl/Vec.h>
#include <gmtl/Matrix.h>

#define DEFAULT_FOCUS_LENGTH 1
#define NEAR_PLANE 0.001
#define FAR_PLANE 512
#define VIEW_ANGLE 60.0
#define DEFAULT_SPEED 0.2f

using namespace gmtl;

class Camera {
	public:
		Camera();
		~Camera();
		void Reset();
		void Orient(Point3f& eye, Point3f& focus, Vec3f& up);
		void Orient(Point3f& eye, Vec3f& look, Vec3f& up);
		void SetViewAngle (double viewAngle);
		void SetNearPlane (double nearPlane);
		void SetFarPlane (double farPlane);
		void SetScreenSize (int screenWidth, int screenHeight);
		void SetLookDistance (double lookDistance);

		Matrix44f GetModelViewMatrix();
        Matrix44f GetInvModelViewMatrix();
        Matrix44f GetProjectionMatrix();
        
		void RotateV(double angle);
		void RotateU(double angle);
		void RotateW(double angle);

		void Translate(const Vec3f &v);
		void Rotate(Point3f p, Vec3f axis, double degree);

		Point3f GetEyePoint();
		Vec3f GetLookVector();
		Vec3f GetUpVector();
		double GetViewAngle();
		double GetNearPlane();
		double GetFarPlane();
		int GetScreenWidth();
		int GetScreenHeight();

		double GetFilmPlanDepth();
		double GetScreenWidthRatio();

        /*
            Movement functions
        */
        void Forward();
        void Back();
        void Left();
        void Right();
        void Up();
        void Down();

	private:
		Matrix44f m_worldToCamera; //World to camera Matrix44f
		Matrix44f m_cameraToWorld; //camera to world Matrix44f
		Vec3f m_n, m_u, m_v;  //n u v of the camera

		double m_viewAngle, m_filmPlanDepth;
		double m_nearPlane, m_farPlane;
		int m_screenWidth, m_screenHeight;
		double m_screenWidthRatio; // m_screenHeightRatio;
};
#endif

