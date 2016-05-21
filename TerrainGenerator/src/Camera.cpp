#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <GL/glut.h>

Camera::Camera(){
    setup(500, 500, 0.01f, 100.0f, 0.0f, 0.0f, 60.0f, vec3(0, 0, 1));
    _old_time = glutGet(GLUT_ELAPSED_TIME);
    _move_speed = 0.5f;
    _turn_speed = 5.0f;
}

void Camera::setup(int w, int h, float n, float f,
    float width_angle, float height_angle, float fov, vec3 pos){

    _position = pos;
    _aspect_ratio = (float)w / (float)h;
    _near = n;
    _far = f;
    _width_angle = width_angle;
    _height_angle = height_angle;
    _fov = fov;

    _update_attributes();
}

mat4 Camera::getModelview(){
    return lookAt(_position, _position + _look, _up);
    
}

mat4 Camera::getProjection(){
    return perspective(_fov, _aspect_ratio, _near, _far);
}

void Camera::forward(){
    _position += _look * _move_speed;
}

void Camera::back(){
    _position -= _look * _move_speed;
}

void Camera::left(){
    _position -= _right * _move_speed;
}

void Camera::right(){
    _position += _right * _move_speed;
}

void Camera::up(){
    _position += _up * _move_speed;
}

void Camera::down(){
    _position -= _up * _move_speed;
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