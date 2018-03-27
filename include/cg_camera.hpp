#ifndef _CG_CAMERA_HPP_
#define _CG_CAMERA_HPP_
#include "cg_types.h"
#include "cg_math3d.h"

namespace cg {

// 摄像机
class Camera
{
public:
    Camera();
    Camera(const Vec3f& pos, const Vec3f& target, const Vec3f& up);

    bool onKeyboard(int key);

    void setPos(const Vec3f& pos);
    void setTarget(const Vec3f& target);
    void setUp(const Vec3f& up);

    const Vec3f& getPos() const;
    const Vec3f& getTarget() const;
    const Vec3f& getUp() const;

    static const float StepSize;

private:
    Vec3f _pos;
    Vec3f _target;
    Vec3f _up;
};


}

#endif