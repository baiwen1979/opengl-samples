#ifndef _CG_IMG_HPP_
#define _CG_IMG_HPP_

namespace cg {

class Texture {
public:
    int getWidth() const;
    int getHeight() const;
    int getNumOfChannels() const;
    unsigned char* getData() const;
    ~Texture();
    static Texture load(const char* filename);
private:
    Texture();
    int _width, _height, _nChannels;
    unsigned char* _data;
};

} //namespace cg

#endif //_CG_IMG_HPP_