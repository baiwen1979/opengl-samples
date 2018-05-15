#include <cg_model.hpp>

namespace cg {

Model::Model(const string& path) {
    load(path);
}

void Model::render(const Shader& shader) const {
    for (unsigned int i = 0; i < _meshes.size(); i++) {
        _meshes[i].render(shader);
    }
}

void Model::render(const Shader* pShader) const {
    render(*pShader);
}

const vector<Mesh>& Model::getMeshes() const {
    return _meshes;
}

const vector<Texture>& Model::getTexturesLoaded() const {
    return _texturesLoaded;
}

void Model::load(const string& path) {
    // 使用 ASSIMP 读取文件
    Assimp::Importer importer;
    cout << "Loading Model from file: " << path << endl;
    unsigned int flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | 
        aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices;
    const aiScene* scene = importer.ReadFile(path, flags);
    // 错误检查
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
        return;
    }
    // 取出完整文件路径中目录部分
    _directory = path.substr(0, path.find_last_of('/'));

    // 递归地处理 ASSIMP 的根节点
    processNode(scene->mRootNode, scene); 
    cout << "Model Loaded Successfully." << endl;
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    // 处理当前节点中的每个mesh
    for(unsigned int i = 0; i < node->mNumMeshes; i++) { 
        // 节点对象仅包含从场景对象中实际对象的索引
        // 场景中包含所有数据，节点只是用来组织这些对象（如节点节的关系）
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        _meshes.push_back(processMesh(mesh, scene));
    }
    // 处理完当前节点的所有的mesh后，然后递归地处理每个子节点
    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    // 用于填充的数据
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    // 遍历mesh中的每个顶点
    for(unsigned int i = 0; i < mesh -> mNumVertices; i++) {
        // 顶点位置
        Vec3f position(mesh -> mVertices[i].x, mesh -> mVertices[i].y, mesh -> mVertices[i].z);
        // 顶点法线
        Vec3f normal(mesh -> mNormals[i].x, mesh -> mNormals[i].y, mesh -> mNormals[i].z);
        // 纹理坐标
        Vec2f txCrds(0.0f);
        if(mesh -> mTextureCoords[0]) // 如果有纹理的话
        {
            txCrds.x = mesh -> mTextureCoords[0][i].x;
            txCrds.y = mesh -> mTextureCoords[0][i].y;
        }
        // 切线
        Vec3f tangent(mesh -> mTangents[i].x, mesh -> mTangents[i].y, mesh -> mTangents[i].z);
        // 二重切线
        Vec3f bitangent(mesh -> mBitangents[i].x, mesh -> mBitangents[i].y, mesh -> mBitangents[i].z);

        Vertex vertex(position, normal, txCrds, tangent, bitangent);
        vertices.push_back(vertex);
    }
    
    // 遍历mesh的每个面片，获取相应的顶点索引
    for(unsigned int i = 0; i < mesh -> mNumFaces; i++) {
        aiFace face = mesh -> mFaces[i];
        // 取出面片中的顶点索引，将其保存到索引列表中
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // 处理材质
    aiMaterial* material = scene -> mMaterials[mesh -> mMaterialIndex];    
    // 这里对着色器中的采样器名称进行了假设。每个漫反射纹理应该命名为"texture_diffuseN"，
    // 其中N为从1到MAX_SAMPLER_NUMBER的序号。同样其他材质的命名约定如下：
    // 漫反射: texture_diffuseN
    // 高光: texture_specularN
    // 法线: texture_normalN

    // 1. 漫反射贴图
    vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, Texture::Diffuse);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. 高光贴图
    vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, Texture::Specular);
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. 法线贴图
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, Texture::Normal);
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. 高度贴图
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, Texture::Height);
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // 返回从提取的网格数据中所创建的网格对象
    return Mesh(vertices, indices, textures);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, Texture::Type texType) {
    vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        // 检查纹理是否已经加载过，如果是，直接进入下次迭代：跳过加载新纹理
        bool skip = false;
        for(unsigned int j = 0; j < _texturesLoaded.size(); j++) {
            if(std::strcmp(_texturesLoaded[j].getPath().data(), str.C_Str()) == 0) {
                textures.push_back(_texturesLoaded[j]);
                skip = true; // 路径相同的纹理已经加载, 继续加载下一个. (优化)
                break;
            }
        }
        if(!skip) {   // 如果纹理没有被加载过，则加载之
            Texture texture = Texture::load(str.C_Str(), _directory, texType);

            textures.push_back(texture);
            _texturesLoaded.push_back(texture);  // 避免重复加载
        }
    }
    return textures;
}


} // namespace cg