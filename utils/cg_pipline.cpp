#include <cg_pipline.hpp>

namespace cg {

Pipeline::Pipeline(): 
    _scale(Vec3f(1.0f, 1.0f, 1.0f)), 
    _pos(Vec3f(0.0f, 0.0f, 0.0f)),
    _rot(Vec3f(0.0f, 0.0f, 0.0f)) {
}

void Pipeline::scale(float scaleX, float scaleY, float scaleZ) {
    _scale.x = scaleX;
    _scale.y = scaleY;
    _scale.z = scaleZ;
}

void Pipeline::scale(float s) {
    scale(s, s, s);
}

void Pipeline::scale(const Vec3f& sv) {
    scale(sv.x, sv.y, sv.z);
}

void Pipeline::posit(float x, float y, float z) {
    _pos.x = x;
    _pos.y = y;
    _pos.z = z;
}

void Pipeline::posit(const Vec3f& pos) {
    posit(pos.x, pos.y, pos.z);
}

void Pipeline::rotate(float rx, float ry, float rz) {
    _rot.x = rx;
    _rot.y = ry;
    _rot.z = rz;
}

void Pipeline::rotate(const Vec3f& rot) {
    rotate(rot.x, rot.y, rot.z);
}

void Pipeline::setPersProjParams(const PersProjParams& p) {
    _persProjParams = p;
}

void Pipeline::setCamera(const CameraQuat& camera) {
    _camera.pos = camera.getPos();
    _camera.target = camera.getTarget();
    _camera.up = camera.getUp();
}

const Mat4f& Pipeline::getWPTransform() {
    getWorldTransform();
    Mat4f mp = Mat4f::perspective(
        _persProjParams.fieldOfView,
        _persProjParams.aspectRatio,
        _persProjParams.zNear,
        _persProjParams.zFar
    );
    _transform = mp * _transform;
    return _transform;
}

const Mat4f& Pipeline::getWorldTransform() {
    Mat4f m;
    m = Mat4f::scale(m, _scale.x, _scale.y, _scale.z);   
    m = Mat4f::rotateX(m, _rot.x);
    m = Mat4f::rotateY(m, _rot.y);
    m = Mat4f::rotateZ(m, _rot.z);
    m = Mat4f::translate(m, _pos.x, _pos.y, _pos.z);
    _transform = m;
    return _transform;
}

const Mat4f& Pipeline::getWVPTransform() {
    getWorldTransform();
    Vec3f pos = _camera.pos * -1;
    Mat4f cmt = Mat4f::translate(Mat4f(), pos.x, pos.y, pos.z);
    Mat4f cmr = Mat4f::lookAt(_camera.pos, _camera.target, _camera.up);
    Mat4f mp = Mat4f::perspective(
        _persProjParams.fieldOfView,
        _persProjParams.aspectRatio,   
        _persProjParams.zNear,
        _persProjParams.zFar
    );
    _transform = mp * cmr * cmt * _transform;
    return _transform;
}

}