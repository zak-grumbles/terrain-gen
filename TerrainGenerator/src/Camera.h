/*
    Camera class declaration.

    Author: Zachary Grumbles
*/

#include <glm/glm.hpp>

using namespace glm;

class Camera{
public:

    //constructor
    Camera();

    /*Sets up the camera's position and other attributes*/
    void setup(int w, int h, float n, float f,
        float width_angle, float height_angle, float fov,
        vec3 pos);

    /*Matrix accessors*/
    mat4 getModelview();
    mat4 getProjection();

    /*Movement functions*/
    void forward();
    void back();
    void left();
    void right();
    void up();
    void down();
    void rotate(int x, int y);

    /*Keeps the time updated. Necessary for clean movement*/
    void updateTime();

    /*Updates screen size*/
    void setDimensions(int x, int y);

private:
    /*Camera position and vectors.*/
    vec3 _position, _look, _up, _right;

    vec2 _last_click;

    /*Transformation matrices*/
    mat4 _modelview, _projection;

    /*Camera attributes*/
    float _width_angle, _height_angle, _fov,
        _move_speed, _turn_speed, _aspect_ratio,
        _near, _far, _old_time;
    int _width, _height;

    /*Gets the change in time*/
    float _getDeltaTime();

    /*Updates the camera's private attributes*/
    void _update_attributes();
};