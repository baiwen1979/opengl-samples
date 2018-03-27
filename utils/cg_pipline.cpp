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

void Pipeline::setCamera(const Camera& camera) {
    _camera = camera;
}

const Mat4f& Pipeline::getWPTransform() {
    getWorldTransform();
    Mat4f mp = Mat4f::persProjMat(
        _persProjParams.aspectRatio,
        _persProjParams.fieldOfView,
        _persProjParams.zNear,
        _persProjParams.zFar
    );
    _transform = mp * _transform;
    return _transform;
}

const Mat4f& Pipeline::getWorldTransform() {
    Mat4f mt = Mat4f::translationMat(_pos.x, _pos.y, _pos.z);
    Mat4f mr = Mat4f::rotationMat(_rot.x, _rot.y, _rot.z);
    Mat4f ms = Mat4f::scalingMat(_scale.x, _scale.y, _scale.z);
    _transform = mt * mr * ms;
    return _transform;
}

const Mat4f& Pipeline::getWVPTransform() {
    getWorldTransform();
    Vec3f pos = _camera.getPos() * -1.0;
    Mat4f cmt = Mat4f::translationMat(pos.x, pos.y, pos.z);
    Mat4f cmr = Mat4f::cameraMat(_camera.getTarget(), _camera.getUp());
    Mat4f mp = Mat4f::persProjMat(
        _persProjParams.aspectRatio,
        _persProjParams.fieldOfView,
        _persProjParams.zNear,
        _persProjParams.zFar
    );
    _transform = mp * cmr * cmt * _transform;
    return _transform;
}

}