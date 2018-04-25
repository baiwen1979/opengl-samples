#include <cg_camera.hpp>

namespace cg {

const float Camera::YAW = -90.0f;
const float Camera::PITCH = 0.0f;
const float Camera::SPEED = 2.5f;
const float Camera::SENSITIVITY = 0.05f;
const float Camera::ZOOM = 45.0f;
const float Camera::ASPECT = 4.0f / 3.0f;

Camera::Camera(float aspect, const Vec3f& position, const Vec3f& up, float yaw, float pitch):
    _aspect(aspect),
    _position(position), 
    _front(Vec3f(0.0f, 0.0f, -1.0f)), 
    _worldUp(up), 
    _yaw(yaw), _pitch(pitch), 
    _movementSpeed(SPEED),
    _moveSensitivity(SENSITIVITY),
    _zoom(ZOOM) {
    // 更新摄像机向量
    updateCameraVectors();
}

Camera::Camera(float aspect, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch):
    _aspect(aspect),
    _position(Vec3f(posX, posY, posZ)),
    _front(Vec3f(0.0f, 0.0f, -1.0f)),
    _worldUp(Vec3f(upX, upY, upZ)),
    _yaw(yaw), _pitch(pitch),
    _movementSpeed(SPEED),
    _moveSensitivity(SENSITIVITY),
    _zoom(ZOOM) {
    // 更新摄像机向量
    updateCameraVectors();
}

const Vec3f& Camera::getPosition() const {
    return _position;
}

float Camera::getZoom() const {
    return _zoom;
}

float Camera::getAspect() const {
    return _aspect;
}

Mat4f Camera::getViewMatrix() const {
    return Mat4f::lookAt(_position, _position + _front, _up);
}

Mat4f Camera::getPerspectiveMatrix() const {
    return Mat4f::perspective(_zoom, _aspect, 1.0f, 100.0f);
}

void Camera::processKeyboardInput(MovementDirection direction, float deltaTime) {
    float velocity = _movementSpeed * deltaTime;

    if (direction == FORWARD) {
        _position += _front * velocity;
    }
    if (direction == BACKWARD) {
        _position -= _front * velocity;
    }
    if (direction == LEFT) {
        _position += _right * velocity;
    }
    if (direction == RIGHT) {
        _position -= _right * velocity;
    }
}

void Camera::processMouseMovement(float xOffset, float yOffset, bool constrainPitch) {
    xOffset *= _moveSensitivity;
    yOffset *= _moveSensitivity;

    _yaw   += xOffset;
    _pitch += yOffset;

    // 确保仰角在（-89.0，89.0）之间，否则会上下颠倒
    if (constrainPitch)
    {
        if (_pitch > 89.0f) {
            _pitch = 89.0f;
        }
        if (_pitch < -89.0f) {
            _pitch = -89.0f;
        }
    }
    // 使用更新后的欧拉角更新前、右和上向量
    updateCameraVectors();    
}

void Camera::processMouseScroll(float yOffset) {
    if (_zoom >= 1.0f && _zoom <= 45.0f) {
        _zoom -= yOffset;
    }
    if (_zoom <= 1.0f) {
        _zoom = 1.0f;
    }
    if (_zoom >= 45.0f) {
        _zoom = 45.0f;
    } 
}

void Camera::updateCameraVectors() {
    // 重新计算前向量
    Vec3f front;
    front.x = cos(toRadian(_yaw)) * cos(toRadian(_pitch));
    front.y = sin(toRadian(_pitch));
    front.z = sin(toRadian(_yaw)) * cos(toRadian(_pitch));
    _front = Vec3f::normalize(front);
    // 并重新计算右和上向量
    _right = Vec3f::normalize(Vec3f::cross(_front, _worldUp)); // 计算并规范化右向量
    _up    = Vec3f::normalize(Vec3f::cross(_right, _front));   // 计算摄像机的本地上向量
}

} // namespace cg