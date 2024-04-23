#ifndef VERTEX_BUFFER_LAYOUT_HPP
#define VERTEX_BUFFER_LAYOUT_HPP

#include <vector>

typedef struct {
  unsigned int count, type;
  bool normalized;
} VertexBufferElement;

class VertexBufferLayout {
public:
  VertexBufferLayout();
  void Push(unsigned int count, unsigned int type, bool normalized);
private:
  std::vector<VertexBufferElement> m_Elements;
};

#endif
