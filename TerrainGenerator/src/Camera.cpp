/*
    Class definition for camera class.

    Author: Zachary Grumbles
*/

#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <GL/glut.h>

/*
    Constructor.

    Sets the camera at 0, 0, 1 looking down the z axis.
    Sets the movement speeds.
*/
Camera::Camera(){
    setup(500, 500, 0.01f, 100.0f, 0.0f, 0.0f, 60.0f, vec3(0, 0, 1));
    _old_time = glutGet(GLUT_ELAPSED_TIME);
    _move_speed = 0.2f;
    _turn_speed = 0.00001f;
    _last_click = vec2(0.0, 0.0);
}

/*
    Sets up the camera at a given position with the given attributes.
*/
void Camera::setup(int w, int h, float n, float f,
    float width_angle, float height_angle, float fov, vec3 pos){

    _position = pos;
    _aspect_ratio = (float)w / (float)h;
    _near = n;
    _far = f;
    _width_angle = width_angle;
    _height_angle = height_angle;
    _fov = fov;
    _width = w;
    _height = h;

    _update_attributes();
}

mat4 Camera::getModelview(){
    return lookAt(_position, _position + _look, _up);
    
}

mat4 Camera::getProjection(){
    return perspective(_fov, _aspect_ratio, _near, _far);
}

void Camera::forward(){
    _position += _look * _getDeltaTime() * _move_speed;
}

void Camera::back(){
    _position -= _look * _getDeltaTime() * _move_speed;
}

void Camera::left(){
    _position -= _right * _getDeltaTime() * _move_speed;
}

void Camera::right(){
    _position += _right * _getDeltaTime() * _move_speed;
}

void Camera::up(){
    _position += _up * _getDeltaTime() * _move_speed;
}

void Camera::down(){
    _position -= _up * _getDeltaTime() * _move_speed;
}

void Camera::rotate(int x, int y){
    float deltaTime = _getDeltaTime();

    _width_angle += _turn_speed * deltaTime * float(_width / 2.0f - x);
    _height_angle += _turn_speed * deltaTime * float(_height / 2.0f - y);

    _update_attributes();
}

void Camera::updateTime(){
    _old_time = glutGet(GLUT_ELAPSED_TIME);
}

void Camera::setDimensions(int x, int y){
    _aspect_ratio = (float)x / (float)y;
    _update_attributes();
}

float Camera::_getDeltaTime(){
    float new_time = glutGet(GLUT_ELAPSED_TIME);
    float result = new_time - _old_time;
    _old_time = new_time;

    return result;
}

void Camera::_update_attributes(){
    _look = vec3
        (
        cos(_height_angle) * sin(_width_angle),
        sin(_height_angle),
        cos(_height_angle) * cos(_width_angle)
        );

    _right = vec3
        (
        sin(_width_angle - 3.14f / 2.0f),
        0.0f,
        cos(_width_angle - 3.14f / 2.0f)
        );

    _up = cross(_right, _look);
}