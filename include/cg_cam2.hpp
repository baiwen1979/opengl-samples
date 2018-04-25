#ifndef _CG_CAM2_HPP_
#define _CG_CAM2_HPP_
#include "cg_types.h"
#include "cgm/cg_math.h"

namespace cg {

// 四元数旋转摄像机
class CameraQuat
{
public:
    CameraQuat(int winWidth, int winHeight);
    CameraQuat(int winWidth, int winHeight, const Vec3f& pos, const Vec3f& target, const Vec3f& up);
    // 按键事件处理函数
    bool onKeyboard(int key);
    // 鼠标事件处理函数
    void onMouse(int x, int y);
    // 渲染函数
    void onRender();
    // 设置位置、目标和上方向
    void setPos(const Vec3f& pos);
    void setTarget(const Vec3f& target);
    void setUp(const Vec3f& up);
    // 获取位置、目标和上方向
    const Vec3f& getPos() const;
    const Vec3f& getTarget() const;
    const Vec3f& getUp() const;

    static const float STEP_SCALE;
    static const int MARGIN;
    static const float EDGE_STEP;

private:

    void init();    // 初始化
    void update();  // 更新

    Vec3f _pos;     // 位置
    Vec3f _target;  // 目标
    Vec3f _up;      // 上方向

    int _winWidth;  // 窗口宽度
    int _winHeight; // 窗口高度

    float _angleH;  //水平角
    float _angleV;  //垂直角

    bool _onTopEdge;    //是否在上边界
    bool _onBottomEdge; //是否在下边界
    bool _onLeftEdge;   //是否在左边界
    bool _onRightEdge;  //是否在右边界

    Vec2i _mousePos; //鼠标位置

};


}

#endif // _CG_CAM2_HPP_