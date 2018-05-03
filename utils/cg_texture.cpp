#define STB_IMAGE_IMPLEMENTATION
#include "cg_image.hpp"

#include <iostream>
#include <cg_texture.hpp>
#include <GL/cgl.h>

namespace cg {

Texture::Texture(unsigned int id, const Type& texType): 
    _id(id), _type(texType) {}

Texture::Texture(unsigned int id, const Type& texType, const string& path):
    _id(id), _type(texType), _path(path) {}

Texture::Texture(const char* filePath, const Type& texType):
    _type(texType) {   
    _id = loadTexture(filePath);
}

unsigned int Texture::getId() const {
    return _id;
}

Texture::Type Texture::getType() const {
    return _type;
}

const string& Texture::getPath() const {
    return _path;
}

void Texture::apply(GLenum textureUnit) const {
    glActiveTexture(textureUnit); // 激活纹理单元
    glBindTexture(GL_TEXTURE_2D, _id);  // 绑定当前纹理对象
}

Texture::~Texture() {
    // nothing to do;
}

Texture Texture::load(const string& filename, const string& directory, Type texType) {
    Texture tex = load((directory + '/' + filename).c_str(), texType);
    tex._path;
    return tex;
}

Texture Texture::load(const char* filePath, Type texType) {
    unsigned int texId = loadTexture(filePath);
    return Texture(texId, texType);
}

unsigned int loadTexture(const char* filePath) {
    unsigned int texId;
    glGenTextures(1, &texId);
    int width, height, numOfChannels;
    stbi_set_flip_vertically_on_load(true);
    std::cout << "Loading Texture from file: " << filePath << std::endl;
    unsigned char* data = stbi_load(filePath, &width, &height, &numOfChannels, 0);
    if (data) {
        GLenum format;
        if (numOfChannels == 1) {
            format = GL_RED;
        }
        if (numOfChannels == 3) {
            format = GL_RGB;
        }
        if (numOfChannels == 4) {
            format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, texId);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
        std::cout << "Texture Loaded Successfully." << std::endl;
    }
    else {
        std::cout << "Texture Failed to load from file path: " << filePath << std::endl;
        stbi_image_free(data);
    }
    return texId;
}

} // namespace cg

