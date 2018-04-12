#include <cg_camera.hpp>
#include <cgm/cg_math.h>

namespace cg {

const float Camera::STEP_SCALE = 1.0f;
const int Camera::MARGIN = 10;
const float Camera::EDGE_STEP = 0.1f;

Camera::Camera(int w, int h): 
    _pos(Vec3f(0.0f)), 
    _target(Vec3f(0.0f, 0.0f, 1.0f)), 
    _up(Vec3f(0.0f, 1.0f, 0.0f)),
    _winWidth(w), _winHeight(h),
    _onTopEdge(false), 
    _onBottomEdge(false), 
    _onLeftEdge(false), 
    _onRightEdge(false),
    _mousePos(Vec2i(w / 2, h / 2)) {
    init();
}

Camera::Camera(int w, int h, const Vec3f& pos, const Vec3f& target, const Vec3f& up):
    _pos(pos), 
    _target(target), 
    _up(up), 
    _winWidth(w), _winHeight(h),
    _onTopEdge(false), 
    _onBottomEdge(false), 
    _onLeftEdge(false), 
    _onRightEdge(false),
    _mousePos(Vec2i(w / 2, h / 2)) {
    _target.normalize();
    _up.normalize();
    init();
}

bool Camera::onKeyboard(int key) {
    bool ret = false;

    switch (key) {

        case GLUT_KEY_UP:
        {
            _pos += _target * STEP_SCALE;
            ret = true;
        }
        break;

        case GLUT_KEY_DOWN:
        {
            _pos -= _target * STEP_SCALE;
            ret = true;
        }
        break;

        case GLUT_KEY_LEFT:
        {
            Vec3f left = _target.cross(_up);
            left.normalize();
            left *= STEP_SCALE;
            _pos += left;
            ret = true;
        }
        break;

        case GLUT_KEY_RIGHT:
        {
            Vec3f right = _up.cross(_target);
            right.normalize();
            right *= STEP_SCALE;
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

void Camera::init() {
    Vec3f hTarget(_target.x, 0.0f, _target.z);
    hTarget.normalize();
    // 计算水平倾角
    if (hTarget.z >= 0.0f) {
        if (hTarget.x >= 0.0f) {
            _angleH = 360.0f - toDegree(asin(hTarget.z));
        }
        else {
            _angleH = 180.0f + toDegree(asin(hTarget.z));
        }
    }
    else {
        if (hTarget.x >= 0.0f) {
            _angleH = toDegree(asin(-hTarget.z));
        }
        else {
            _angleH = 180.0f - toDegree(asin(-hTarget.z));
        }

    }
    // 计算垂直倾角
    _angleV = -toDegree(asin(_target.y));
    // 移动鼠标到指定位置
    glutWarpPointer(_mousePos.x, _mousePos.y);
}

void Camera::onMouse(int x, int y) {
    // 鼠标移动距离
    const int dx = x - _mousePos.x;
    const int dy = y - _mousePos.y;
    // 当前鼠标位置
    _mousePos.x = x;
    _mousePos.y = y;
    // 根据鼠标移动距离计算水平和垂直倾角
    _angleH += (float)dx / 20.0f;
    _angleV += (float)dy / 20.0f;

    if (dx == 0) {
        if (x <= MARGIN) {
            _onLeftEdge = true;
        }
        else if (x >= (_winWidth - MARGIN)) {
            _onRightEdge = true;
        }
    }
    else {
        _onLeftEdge = false;
        _onRightEdge = false;
    }

    if (dy == 0) {
        if (y <= MARGIN) {
            _onTopEdge = true;
        }
        else if (y >= (_winHeight - MARGIN)) {
            _onBottomEdge = true;
        }
    }
    else {
        _onTopEdge = false;
        _onBottomEdge = false;
    }

    update();

}

void Camera::onRender()
{
    bool shouldUpdate = false;

    if (_onLeftEdge) {
        _angleH -= EDGE_STEP;
        shouldUpdate = true;
    }
    else if (_onRightEdge) {
        _angleH += EDGE_STEP;
        shouldUpdate = true;
    }

    if (_onTopEdge) {
        if (_angleV > -90.0f) {
            _angleV -= EDGE_STEP;
            shouldUpdate = true;
        }
    }
    else if (_onBottomEdge) {
        if (_angleV < 90.0f) {
            _angleV += EDGE_STEP;
            shouldUpdate = true;
        }
    }

    if (shouldUpdate) {
        update();
    }
}

void Camera::update() {
    // 垂直旋转轴
    const Vec3f vAxis(0.0f, 1.0f, 0.0f);

    // 围绕垂直轴旋转观察向量为指定的水平角
    Vec3f view(1.0f, 0.0f, 0.0f);

    view.rotate(_angleH, vAxis);
    view.normalize();

    // 围绕水平轴旋转观察向量为指定的垂直角
    Vec3f hAxis = vAxis.cross(view);
    hAxis.normalize();
    view.rotate(_angleV, hAxis);
    view.normalize();

    // 设定摄像机目标为观察向量
    _target = view;
    _target.normalize();
    
    // 重新计算上方向量
    _up = _target.cross(hAxis);
    _up.normalize();
}


} // namespace cg