#ifndef _CG_CAMERA_HPP_
#define _CG_CAMERA_HPP_

#include "cgm/cg_math.h"

namespace cg {

// 摄像机类
class Camera {
public:
    // 移动方向
    enum MovementDirection {
        FORWARD,  // 前
        BACKWARD, // 后
        LEFT,     // 左
        RIGHT     // 右
    };

    static const float YAW;         // 偏航(水平）角
    static const float PITCH;       // 仰（垂直）角
    static const float SPEED;       // 速度
    static const float SENSITIVITY; // 敏感度
    static const float ZOOM;        // 缩放度
    static const float ASPECT;      // 宽高比

    // 构造函数：向量参数
    Camera(
        float aspect = ASPECT,
        const Vec3f& position = Vec3f(0.0f),
        const Vec3f& up = Vec3f(0.0f, 1.0f, 0.0f), 
        float yaw = YAW, float pitch = PITCH
    );

    // 构造函数：标量参数
    Camera (
        float aspect,
        float posX, float posY, float poxZ,
        float upX, float upY, float upZ,
        float yaw, float pitch 
    );

    // 获取摄像机位置
    const Vec3f& getPosition() const;
    // 获取摄像机前向量
    const Vec3f& getFront() const;
    // 获取变焦度数
    float getZoom() const;
    // 获取宽高比
    float getAspect() const;
    // 获取观察变换矩阵
    Mat4f getViewMatrix() const;
    // 获取透视投影矩阵
    Mat4f getPerspectiveMatrix() const;

    // 处理键盘输入
    void processKeyboardInput(MovementDirection direction, float deltaTime);
    // 处理鼠标移动
    void processMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
    // 处理鼠标滚动
    void processMouseScroll(float yOffset);

private:
    // 基本属性
    Vec3f _position; // 位置
    Vec3f _front;    // 前向量（局部坐标系）
    Vec3f _up;       // 上向量
    Vec3f _right;    // 右向量
    Vec3f _worldUp;  // 上向量（世界坐标系）
    // 欧拉角
    float _yaw;      // 偏航角
    float _pitch;    // 仰角
    // 摄像机选项
    float _movementSpeed;    // 移动速度
    float _moveSensitivity;  // 移动灵敏度
    float _zoom;             // 变焦度数
    float _aspect;           // 宽高比

    // 根据摄像机（更新后）的欧拉角计算前向量
    void updateCameraVectors();

};

} // namespace

#endif //_CG_CAMERA_HPP_