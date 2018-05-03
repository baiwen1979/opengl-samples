#include <cg_outline.hpp>

namespace cg {

Outline::Outline(const Model* pModel, const Shader* pOutlineShader): 
    _pModel(pModel), _pShader(pOutlineShader), _scale(1.1) {
    _pShader -> use();
    _pShader -> setColor3f("outlineColor", Color3f(1.0f, 0.5f, 0.0f));
}

void Outline::setStyle(float size, const Color3f& color) {
    size = clamp<float>(size, 0.01f, 0.5f);
    _scale = 1 + size;
    _pShader -> use();
    _pShader -> setColor3f("outlineColor", color);
}

void Outline::render(const Shader* pModelShader) const {
    // (1). 第一遍，以正常模式绘制立方体，并写入蒙版缓存
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);

    pModelShader -> use();
    _pModel -> render(pModelShader);
    // (2). 第二遍，以轮廓着色器绘制稍微放大的立方体，并禁止模版缓存写入。
    // 因为蒙版缓存现在填充的都是1，正好对应于原来的立方体。 这次绘制时，蒙版缓存中为1的部分
    // 会被忽略，只绘制大小不同的部分，这就使得这部分看起来像轮廓
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF); //忽略不为1的部分
    glStencilMask(0x00); // 禁止更新蒙版缓存
    glDisable(GL_DEPTH_TEST); // 禁止更新深度缓存
    _pShader -> use();
    _pModel -> render(_pShader);
    // 允许更新蒙版缓存
    glStencilMask(0xFF);
    // 允许深度测试
    glEnable(GL_DEPTH_TEST);
}

Outline::~Outline() {
    _pModel = NULL;
    _pShader = NULL;
}


} // namespace cg