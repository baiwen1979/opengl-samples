#include <cg_mesh.hpp>

namespace cg {

Vertex::Vertex(const Vec3f& pos):
    position(pos), normal(0), texCoords(0), tangent(0), bitangent(0) {}

Vertex::Vertex(const Vec3f& pos, const Vec3f& norm):
    position(pos), normal(norm), texCoords(0), tangent(0), bitangent(0) {}

Vertex::Vertex(const Vec3f& pos, const Vec3f& norm, const Vec2f& txCrds): 
    position(pos), normal(norm), texCoords(txCrds), tangent(0), bitangent(0) {}

Vertex::Vertex(const Vec3f& pos, const Vec3f& norm, const Vec2f& txCrds, const Vec3f& tan, const Vec3f& bitan):
    position(pos), normal(norm), texCoords(txCrds), tangent(tan), bitangent(bitan) {}

Mesh::Mesh(
    const vector<Vertex>& vertices, 
    const vector<unsigned int>& indices, 
    const vector<Texture>& textures):
    _vertices(vertices),
    _indices(indices),
    _textures(textures) {
   // 初始化网格
   init(); 
}

void Mesh::init() {
    // 生成顶点数组对象VAO
    glGenVertexArrays(1, &_VAO);
    // 生成顶点缓存对象VBO
    glGenBuffers(1, &_VBO);
    // 生成索引（元素）缓存对象EBO
    glGenBuffers(1, &_EBO);

    // 绑定顶点数组对象VAO
    glBindVertexArray(_VAO);
    // 绑定顶点缓存对象VBO
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    // 为当前绑定的顶点缓存对象分配内存空间，并存储顶点数据
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);  

    // 绑定元素（索引）缓存对象EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    // 为当前绑定的元素（索引）缓存对象分配内容空间，并存储元素（索引）数据
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);

    // 启用并指定顶点位置属性
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // 启用并指定顶点法线属性
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // 启用并指定顶点纹理坐标属性
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    // 解除顶点数组绑定
    glBindVertexArray(0);
}

void Mesh::render(const Shader& shader) const {

    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr   = 1;
    unsigned int heightNr   = 1;

    for(unsigned int i = 0; i < _textures.size(); i++) {
        // 激活相应的纹理单元
        glActiveTexture(GL_TEXTURE0 + i);
        // 计算纹理编号
        string number, name;
        Texture::Type texType = _textures[i].getType();
        
        switch (texType) {
            case Texture::Diffuse:
                name = "texture_diffuse";
                number = std::to_string(diffuseNr++);
                break;
            case Texture::Specular:
                name = "texture_specular";
                number = std::to_string(specularNr++);
                break;
            case Texture::Normal:
                name = "texture_normal";
                number = std::to_string(normalNr++);
                break;
            case Texture::Height:
                name = "texture_height";
                number = std::to_string(heightNr++);
        }

        shader.setFloat(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, _textures[i].getId());
    }
    glActiveTexture(GL_TEXTURE0);

    // 绘制网格
    glBindVertexArray(_VAO);
    glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void calcNormals(const GLuint indices[], GLuint indexCount, Vertex vertices[], GLuint vertexCount)
{
    for (GLuint i = 0 ; i < indexCount ; i += 3) {
        GLuint index0 = indices[i];
        GLuint index1 = indices[i + 1];
        GLuint index2 = indices[i + 2];

        Vec3f v1 = vertices[index1].position - vertices[index0].position;
        Vec3f v2 = vertices[index2].position - vertices[index0].position;
        Vec3f normal = v1.cross(v2);

        normal.normalize();

        vertices[index0].normal += normal;
        vertices[index1].normal += normal;
        vertices[index2].normal += normal;
    }

    for (GLuint i = 0 ; i < vertexCount ; i++) {
        vertices[i].normal.normalize();
    }
}

} // namespace