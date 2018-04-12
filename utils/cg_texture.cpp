#define STB_IMAGE_IMPLEMENTATION
#include "cg_image.hpp"

#include <cg_texture.hpp>

namespace cg {

Texture::Texture(): _width(0), _height(0), _data(NULL) {}

unsigned char* Texture::getData() const {
    return _data;
}

int Texture::getHeight() const {
    return _height;
}

int Texture::getWidth() const {
    return _width;
}

Texture::~Texture() {
    stbi_image_free(_data);
}

Texture Texture::load(const char* filename) {
    Texture tex;
    int w, h, nc;
    stbi_set_flip_vertically_on_load(true);
    tex._data = stbi_load(filename, &w, &h, &nc, 0);
    tex._width = w;
    tex._height = h;
    tex._nChannels = nc;
    return tex;
}


}

