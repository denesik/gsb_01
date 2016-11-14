#pragma once
#ifndef UniformBase_h__
#define UniformBase_h__

#include "Shader.h"
#include <vector>
#include "UniformTools.h"

class UniformBase
{
public:
  
  /// Поток графики.
  /// Запрашивает у шейдера все юниформы.
  void AttachShader(Shader &shader);

protected:
  struct Uniform
  {
    int location;
    char *name;
  };

  std::vector<Uniform> mUniforms;

protected:
  void SetUniform(const glm::mat4 &val, int location) const;
  void SetUniform(const glm::mat3 &val, int location) const;
  void SetUniform(const glm::mat2 &val, int location) const;
  void SetUniform(int val, int location) const;
  void SetUniform(unsigned int val, int location) const;
  void SetUniform(const glm::vec4 &val, int location) const;
  void SetUniform(const glm::vec3 &val, int location) const;
  void SetUniform(const glm::vec2 &val, int location) const;
  void SetUniform(const float &val, int location) const;

};

UNIFORM
(
  (UniformBasic)
  (int, atlas)
  (glm::mat4, mat_vp)
)




#endif // UniformBase_h__
