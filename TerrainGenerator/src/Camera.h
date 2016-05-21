#include <glm/glm.hpp>

using namespace glm;

class Camera{
public:
    Camera();

    void setup(int w, int h, float n, float f,
        float width_angle, float height_angle, float fov,
        vec3 pos);

    mat4 getModelview();
    mat4 getProjection();

    void forward();
    void back();
    void left();
    void right();
    void up();
    void down();
    

private:
    vec3 _position, _look, _up, _right;

    mat4 _modelview, _projection;

    float _width_angle, _height_angle, _fov,
        _move_speed, _turn_speed, _aspect_ratio,
        _near, _far, _old_time;

    float _getDeltaTime();
    void _update_attributes();
};