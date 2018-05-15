#ifndef _CG_MESH_HPP_
#define _CG_MESH_HPP_

#include <string>
#include <sstream>
#include <vector>
#include <cgm/cg_math.h>
#include "cg_texture.hpp"
#include "cg_shader.hpp"

namespace cg {

// 顶点
struct Vertex {
    Vertex(const Vec3f& pos);
    Vertex(const Vec3f& pos, const Vec3f& norm);
    Vertex(const Vec3f& pos, const Vec3f& norm, const Vec2f& txCrds);
    Vertex(const Vec3f& pos, const Vec3f& norm, const Vec2f& txCrds, const Vec3f& tan, const Vec3f& bitan);

    Vec3f position;  // 位置
    Vec3f normal;    // 法向量
    Vec2f texCoords; // 纹理坐标
    Vec3f tangent;   // 切向量
    Vec3f bitangent; // 二重切向量
};

// 3D网格模型类
class Mesh {
public:

    /*** 成员函数  ***/
    // 构造函数
    Mesh(const vector<Vertex>& vertices, const vector<unsigned int>& indices, const vector<Texture>& textures);
    Mesh(const vector<Vertex>& vertices, const vector<Texture>& textures);
    Mesh(const vector<Vertex>& vertices);
    // 使用指定的着色器进行渲染（绘制）
    void render(const Shader& shader) const;
    void render(const Shader* pShader) const;

    unsigned int getVAO() const;
    const vector<unsigned int>& getIndices() const;

private:
    /*** 网格数据 ***/
    vector<Vertex> _vertices;      // 网格顶点集合
    vector<unsigned int> _indices; // 三角形索引集合
    vector<Texture> _textures;     // 纹理集合
    /*** 渲染数据 ***/
    unsigned int _VAO, _VBO, _EBO;
    /*** 初始化 ***/
    void init();
};

// 计算顶点法线
void calcNormals(const GLuint indices[], GLuint indexCount, Vertex vertices[], GLuint vertexCount);


} // namespace

#endif