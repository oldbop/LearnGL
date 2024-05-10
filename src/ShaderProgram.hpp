#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

#include <string>
#include <vector>

class ShaderProgram {
public:
  void CompileShader(unsigned int type, const std::string &path);
  void CreateProgram();
  unsigned int GetID() const { return m_ID; }
  void SetBool(const std::string &name, bool value) const;
  void SetFloat(const std::string &name, float value) const;
  void SetInt(const std::string &name, int value) const;
  void Use();
private:
  unsigned int m_ID;
  std::vector<unsigned int> m_ShaderIDs;
};

#endif
