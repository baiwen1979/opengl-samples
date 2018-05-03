#include <cg_cubemap.hpp>
#include "cg_image.hpp"
namespace cg {

Cubemap::Cubemap (
    const string& right, const string& left, const string& top, 
    const string& bottom, const string& front, const string& back) {
    vector<string> faces;
    faces.push_back(right);
    faces.push_back(left);
    faces.push_back(top);
    faces.push_back(bottom);
    faces.push_back(front);
    faces.push_back(back);

    _texId = loadCubemap(faces);
}

void Cubemap::apply(GLenum textureUnit) const {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _texId);
}

unsigned int loadCubemap(const vector<string>& faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    int width, height, nrChannels;
    int numOfLoaded = 0;
    stbi_set_flip_vertically_on_load(false);
    for (unsigned int i = 0; i < faces.size(); i++) {
        std::cout << "Loading Cubemap Texture from file: " << faces[i] << std::endl;
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
            numOfLoaded ++;
        }
        else {
            std::cout << "Cubemap Texture Failed to Load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    if (numOfLoaded == faces.size()) {
        std::cout << "Cubemap Texture Loaded Successfully." << std::endl;
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

} // namespace cg