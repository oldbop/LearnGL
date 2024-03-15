#ifndef INDEX_BUFFER_HPP
#define INDEX_BUFFER_HPP

class IndexBuffer {
public:
  IndexBuffer(const unsigned int *data, unsigned int count);
  ~IndexBuffer();
  void Bind() const;
  void Unbind() const;
  unsigned int GetCount() const { return m_Count; }
private:
  unsigned int m_RendererID;
  unsigned int m_Count;
};

#endif
