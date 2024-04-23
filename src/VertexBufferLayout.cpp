#include "VertexBufferLayout.hpp"

#ifdef SYS_GL_HEADERS
#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glext.h>

#else
#include <glad/gl.h>

#endif

void VertexBufferLayout::Push(unsigned int count, unsigned int type, bool normalized) {

  m_Elements.push_back({ count, type, normalized });

}
