#ifndef _CG_ERROR_H_
#define _CG_ERROR_H_

#include "cg_types.h"

namespace cg {

/**
 * 检查并输出OpenGL内部错误
 * @return {GLenum} 错误编码
 */
GLenum errorCheck ();

}
#endif
