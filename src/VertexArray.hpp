#ifndef VERTEX_ARRAY_HPP
#define VERTEX_ARRAY_HPP

#include "VertexBuffer.hpp"

class VertexArray {
public:
  VertexArray();
  ~VertexArray();
  void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
private:

};

#endif
