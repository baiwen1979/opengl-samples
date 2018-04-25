#ifndef _CG_MODEL_HPP_
#define _CG_MODEL_HPP_

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "cg_mesh.hpp"

namespace cg {

// 模型类
class Model {

public:
    /* 构造函数：从指定的文件中创建模型对象 */
    Model(const string& path);

    /* 渲染模型：使用指定的着色器渲染模型 */
    void render(const Shader& shader) const;

private:
    /* 模型数据 */
    vector<Texture> _texturesLoaded;
    vector<Mesh> _meshes;
    string _directory;

    /* 成员函数 */
    // 从指定的文件中加载模型
    void load(const string& path);
    // 处理模型
    void processNode(aiNode *node, const aiScene *scene);
    // 处理网格对象
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    // 加载材质纹理
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, Texture::Type texType);
};


} //namespace cg

#endif //_CG_MODEL_HPP_