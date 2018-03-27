#include <cg_camera.hpp>

namespace cg {

const float Camera::StepSize = 0.5f;

Camera::Camera(): 
    _pos(Vec3f(0.0f)), 
    _target(Vec3f(0.0f, 0.0f, 1.0f)),
    _up(Vec3f(0.0f, 0.0f, 1.0f)) {
    // nothing to do
}

Camera::Camera(const Vec3f& pos, const Vec3f& target, const Vec3f& up):
    _pos(pos), _target(target), _up(up) {
    // nothing to do
}

bool Camera::onKeyboard(int key) {
    bool ret = false;

    switch (key) {

        case GLUT_KEY_UP:
        {
            _pos += _target * StepSize;
            ret = true;
        }
        break;

        case GLUT_KEY_DOWN:
        {
            _pos -= _target * StepSize;
            ret = true;
        }
        break;

        case GLUT_KEY_LEFT:
        {
            Vec3f left = _target.crossProduct(_up);
            left.normalize();
            left *= StepSize;
            _pos += left;
            ret = true;
        }
        break;

        case GLUT_KEY_RIGHT:
        {
            Vec3f right = _up.crossProduct(_target);
            right.normalize();
            right *= StepSize;
            _pos += right;
            ret = true;
        }
        break;
    }

    return ret;
}

void Camera::setPos(const Vec3f& pos) {
    _pos = pos;
}

void Camera::setTarget(const Vec3f& target) {
    _target = target;
}

void Camera::setUp(const Vec3f& up) {
    _up = up;
}

const Vec3f& Camera::getPos() const {
    return _pos;
}

const Vec3f& Camera::getTarget() const {
    return _target;
}

const Vec3f& Camera::getUp() const {
    return _up;
}


} // namespace cg