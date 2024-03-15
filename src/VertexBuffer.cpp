#include "VertexBuffer.hpp"

#ifdef SYS_GL_HEADERS
#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glext.h>

#else
#include <glad/gl.h>

#endif

VertexBuffer::VertexBuffer(const void *data, unsigned int size) {
  glGenBuffers(1, &m_RendererID);
  glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
  glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Bind() const {
  glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() const {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
