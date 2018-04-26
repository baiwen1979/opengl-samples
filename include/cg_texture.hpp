#ifndef _CG_TEXTURE_HPP_
#define _CG_TEXTURE_HPP_

#include <string>
#include <GL/cgl.h>

using std::string;

namespace cg {

//2D纹理（贴图）类
class Texture {

public:
    // 贴图类型
    enum Type {
        Diffuse,  // 漫反射纹理（贴图）
        Specular, // 高光纹理（贴图）
        Normal,   // 法线纹理（贴图）
        Height    // 高度纹理（贴图）
    };

    Texture(const char* filePath, const Type& textureType);
    // 纹理（贴图）ID
    unsigned int getId() const;
    // 纹理（贴图）类型
    Type getType() const;
    // 纹理（贴图）路径
    const string& getPath() const;
    // 应用纹理到指定纹理单元
    void apply(GLenum textureUnit) const;

    ~Texture();

    // 从指定的完整文件路径中加载指定类型的纹理贴图
    static Texture load(const char* filePath, Type textureType);
    // 从指定目录的指定文件中加载指定类型的纹理贴图
    static Texture load(const string& filename, const string& directory, Type textureType);
private:
    // 构造函数
    Texture(unsigned int id, const Type& textureType);
    Texture(unsigned int id, const Type& textureType, const string& path);
    // 成员变量
    unsigned int _id;
    Type _type;
    string _path;
};

unsigned int loadTexture(const char* filePath);

} //namespace cg

#endif //_CG_TEXTURE_HPP_