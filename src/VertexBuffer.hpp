#ifndef VERTEX_BUFFER_HPP
#define VERTEX_BUFFER_HPP

class VertexBuffer {
public:
  VertexBuffer(const void *data, unsigned int size);
  ~VertexBuffer();
  void Bind() const;
  void Unbind() const;
private:
  unsigned int m_RendererID;
};

#endif
