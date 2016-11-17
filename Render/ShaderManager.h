#pragma once
#ifndef ShaderManager_h__
#define ShaderManager_h__

#include <unordered_map>
#include "Shader.h"


class ShaderManager
{
public:
  ShaderManager();
  ~ShaderManager();

  /// Загрузить все меши в директории.
  void LoadDirectory(const std::string &directory);

  /// Получить меш по имени.
  Shader *GetShader(const std::string &name);

  void Compile();

private:
  std::unordered_map<std::string, Shader> mShaders;

private:
  void LoadShader(const std::string &name);
};



#endif // ShaderManager_h__
