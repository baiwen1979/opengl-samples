#ifndef _CG_OUTLINE_HPP_
#define _CG_OUTLINE_HPP_

#include "cg_shader.hpp"
#include "cg_model.hpp"

namespace cg { 

class Outline {
public:
    Outline(const Model* pModel, const Shader* pOutlineShader);
    void setStyle(float size, const Color3f& color);
    void render(const Shader* pModelShader) const;
    virtual ~Outline();
private:
    const Model * _pModel;
    const Shader * _pShader;
    float _scale;
};

} // namespace cg

#endif //_CG_OUTLINE_HPP_