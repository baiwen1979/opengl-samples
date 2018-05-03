#ifndef _CG_SKYBOX_HPP_
#define _CG_SKYBOX_HPP_

#include <string>
#include "cg_cubemap.hpp"
#include "cg_shader.hpp"
namespace cg {

class Skybox {
public:
    Skybox(
        const string& right, const string& left, 
        const string& top, const string& bottom,
        const string& front, const string& back
    );
    Skybox (const Cubemap& cubemap);

    void render(const Shader& shader) const;
    void render(const Shader* pShader) const;

private:
    Cubemap _cubemap;
};

} //namespace cg

#endif