#include <gmtl/Point.h>
#include <gmtl/Vec.h>
#include <gmtl/VecOps.h>
#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>

#define DEFAULT_FOCAL_LENGTH 1
#define NEAR_PLANE 0.001
#define FAR_PLANE 100
#define VIEW_ANGLE 60.0

using namespace gmtl;

class Camera{
public:
	Camera();
	~Camera();

	void Reset();
	void Orient(Point3f&, Point3f&, Vec3f&);
	void Orient(Point3f&, Vec3f&, Vec3f&);

	void SetViewAngle(float);
	void SetNear(float);
	void SetFar(float);
	void SetScreenSize(int, int);
	
	Matrix44f GetModelView();
	Matrix44f GetProjection();
	Matrix44f GetScaleMat();

	void RotateV(float);
	void RotateU(float);
	void RotateW(float);

	void Rotate(Point3f, Vec3f, float);

	Point3f GetEyePoint();
	Vec3f GetLookVector();
	float GetViewAngle();
	float GetNear();
	float GetFar();

	int GetScreenWidth();
	int GetScreenHeight();

	float GetRatio();
	float GetFilmPlaneDepth();

	Vec3f GetUpVector();
	Vec3f GetRightVector();
	Vec3f GetLeftVector();

private:
	Matrix44f model_view;
	Matrix44f inv_model_view;
	Matrix44f cam_2_world;
	
	Vec3f u, v, w;

	float view_angle, near_p, far_p, film_plane_depth;
	int width, height;
	float ratio;
};