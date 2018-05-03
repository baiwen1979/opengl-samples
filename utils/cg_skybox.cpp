#include <cg_skybox.hpp>
#include <cg_mesh.hpp>

namespace cg {

Skybox::Skybox(
    const string& right, const string& left, 
    const string& top, const string& bottom,
    const string& front, const string& back):
    _cubemap(right, left, top, bottom, front, back) {
}

Skybox::Skybox(const Cubemap& cubemap):_cubemap(cubemap) {
}

void Skybox::render(const Shader& shader) const {
    Vertex vertices[] = {         
        Vertex(Vec3f(-1.0f,  1.0f, -1.0f)),
        Vertex(Vec3f(-1.0f, -1.0f, -1.0f)),
        Vertex(Vec3f( 1.0f, -1.0f, -1.0f)),
        Vertex(Vec3f( 1.0f, -1.0f, -1.0f)),
        Vertex(Vec3f( 1.0f,  1.0f, -1.0f)),
        Vertex(Vec3f(-1.0f,  1.0f, -1.0f)),

        Vertex(Vec3f(-1.0f, -1.0f,  1.0f)),
        Vertex(Vec3f(-1.0f, -1.0f, -1.0f)),
        Vertex(Vec3f(-1.0f,  1.0f, -1.0f)),
        Vertex(Vec3f(-1.0f,  1.0f, -1.0f)),
        Vertex(Vec3f(-1.0f,  1.0f,  1.0f)),
        Vertex(Vec3f(-1.0f, -1.0f,  1.0f)),

        Vertex(Vec3f( 1.0f, -1.0f, -1.0f)),
        Vertex(Vec3f( 1.0f, -1.0f,  1.0f)),
        Vertex(Vec3f( 1.0f,  1.0f,  1.0f)),
        Vertex(Vec3f( 1.0f,  1.0f,  1.0f)),
        Vertex(Vec3f( 1.0f,  1.0f, -1.0f)),
        Vertex(Vec3f( 1.0f, -1.0f, -1.0f)),

        Vertex(Vec3f(-1.0f, -1.0f,  1.0f)),
        Vertex(Vec3f(-1.0f,  1.0f,  1.0f)),
        Vertex(Vec3f( 1.0f,  1.0f,  1.0f)),
        Vertex(Vec3f( 1.0f,  1.0f,  1.0f)),
        Vertex(Vec3f( 1.0f, -1.0f,  1.0f)),
        Vertex(Vec3f(-1.0f, -1.0f,  1.0f)),

        Vertex(Vec3f(-1.0f,  1.0f, -1.0f)),
        Vertex(Vec3f( 1.0f,  1.0f, -1.0f)),
        Vertex(Vec3f( 1.0f,  1.0f,  1.0f)),
        Vertex(Vec3f( 1.0f,  1.0f,  1.0f)),
        Vertex(Vec3f(-1.0f,  1.0f,  1.0f)),
        Vertex(Vec3f(-1.0f,  1.0f, -1.0f)),

        Vertex(Vec3f(-1.0f, -1.0f, -1.0f)),
        Vertex(Vec3f(-1.0f, -1.0f,  1.0f)),
        Vertex(Vec3f( 1.0f, -1.0f, -1.0f)),
        Vertex(Vec3f( 1.0f, -1.0f, -1.0f)),
        Vertex(Vec3f(-1.0f, -1.0f,  1.0f)),
        Vertex(Vec3f( 1.0f, -1.0f,  1.0f))
    };

    Mesh skybox(vector<Vertex>(vertices, vertices + sizeof(vertices) / sizeof(Vertex)));
    glDepthFunc(GL_LEQUAL);
    _cubemap.apply(GL_TEXTURE0);
    skybox.render(shader);
    glDepthFunc(GL_LESS);
}

void Skybox::render(const Shader * pShader) const {
    render(*pShader);
}

} // namespace cg