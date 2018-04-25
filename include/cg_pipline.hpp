#ifndef _CG_PIPLINE_HPP_
#define _CG_PIPLINE_HPP_

#include "cgm/cg_math.h"
#include "cg_cam2.hpp"

namespace cg {

// 透视投影参数
struct PersProjParams {
    float aspectRatio; // 宽高比
    float fieldOfView; // 视场角
    float zNear;       // 近平面
    float zFar;        // 远平面
};

class Pipeline {

public:
    // 创建管线对象
    Pipeline();
    // 缩放
    void scale(float s);
    void scale(float scaleX, float scaleY, float scaleZ);
    void scale(const Vec3f& scale);
    // 定位
    void posit(float x, float y, float z);
    void posit(const Vec3f& pos);
    // 旋转
    void rotate(float rotX, float rotY, float rotZ);
    void rotate(const Vec3f& rot);

    // 设置透视投影
    void setPersProjParams(const PersProjParams& p);
    // 设置摄像机
    void setCamera(const CameraQuat& camera);

    // 获取世界坐标变换矩阵
    const Mat4f& getWorldTransform();
    // 获取世界坐标透视投影矩阵
    const Mat4f& getWPTransform();
    // 获取世界视图坐标投影矩阵
    const Mat4f& getWVPTransform();

private:
    Vec3f _scale; // 缩放
    Vec3f _pos;   // 位置
    Vec3f _rot;   // 旋转

    struct {
        Vec3f pos;     // 位置
        Vec3f target;  // 目标
        Vec3f up;      // 上方向
    } _camera; // 摄像机

    Mat4f _transform; // 变换矩阵
    PersProjParams _persProjParams; // 透视投影设置
};

} //namespace cg

#endif //_CG_PIPLINE_HPP_