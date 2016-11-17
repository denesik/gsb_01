#pragma once
#ifndef MeshManager_h__
#define MeshManager_h__

#include <glm/glm.hpp>
#include <string>
#include "Model.h"
#include <unordered_map>
#include "TextureManager.h"
#include "ShaderManager.h"


class ModelManager
{
public:
  ModelManager();
  ~ModelManager();

  /// Загрузить все меши в директории.
  void LoadDirectory(const std::string &directory);

  /// Получить меш по имени.
  Model *GetModel(const std::string &name);

  void Compile();

private:
  TextureManager mTextureManager;
  ShaderManager mShaderManager;

  std::unordered_map<std::string, Model> mModels;

private:
  VertexArray *LoadMesh(const std::string &name, const glm::vec4 &uv);
  void LoadModel(const std::string &name);
};



#endif // MeshManager_h__
