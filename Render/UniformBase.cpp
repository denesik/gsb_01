#include "UniformBase.h"

#include <GL/glew.h>


void UniformBase::AttachShader(Shader &shader)
{
  for (auto &uniform : mUniforms)
  {
    uniform.location = shader.GetUniformLocation(uniform.name);
  }
}

void UniformBase::SetUniform(const glm::mat4 &val, int location) const
{
  glUniformMatrix4fv(location, 1, GL_FALSE, &val[0][0]);
}

void UniformBase::SetUniform(const glm::mat3 &val, int location) const
{
  glUniformMatrix3fv(location, 1, GL_FALSE, &val[0][0]);
}

void UniformBase::SetUniform(const glm::mat2 &val, int location) const
{
  glUniformMatrix2fv(location, 1, GL_FALSE, &val[0][0]);
}

void UniformBase::SetUniform(int val, int location) const
{
  glUniform1i(location, val);
}

void UniformBase::SetUniform(unsigned int val, int location) const
{
  glUniform1ui(location, val);
}

void UniformBase::SetUniform(const glm::vec4 &val, int location) const
{
  glUniform4fv(location, 1, &val[0]);
}

void UniformBase::SetUniform(const glm::vec3 &val, int location) const
{
  glUniform3fv(location, 1, &val[0]);
}

void UniformBase::SetUniform(const glm::vec2 &val, int location) const
{
  glUniform2fv(location, 1, &val[0]);
}

void UniformBase::SetUniform(const float &val, int location) const
{
  glUniform1f(location, val);
}