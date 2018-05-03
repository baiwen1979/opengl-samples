#ifndef _CG_CUBEMAP_HPP_
#define _CG_CUBEMAP_HPP_

#include <vector>
#include <GL/cgl.h>

using std::vector;
using std::string;

namespace cg {

class Cubemap {

public:
    Cubemap(
        const string& right, const string& left, 
        const string& top, const string& bottom, 
        const string& front, const string& back
    );
    
    void apply(GLenum textureUnit) const;


private:
    unsigned int _texId;
};

// 加载立方体贴图，需指定立方体6个面对应的纹理文件
// 需按照以下顺序指定贴图文件:
// +X (右)
// -X (左)
// +Y (上)
// -Y (下)
// +Z (前) 
// -Z (后)
// -------------------------------------------------------
unsigned int loadCubemap(const vector<string>& faces);

} // namespace cg

#endif