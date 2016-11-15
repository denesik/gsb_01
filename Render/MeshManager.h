#pragma once
#ifndef MeshManager_h__
#define MeshManager_h__

#include <string>
#include "VertexArray.h"
#include <unordered_map>


class MeshManager
{
public:
  MeshManager();
  ~MeshManager();

  /// Загрузить все меши в директории.
  void LoadDirectory(const std::string &directory);

  /// Получить меш по имени.
  VertexArray *GetMesh(const std::string &name);

  void Compile();

private:
  std::unordered_map<std::string, VertexArray> mMeshes;

private:
  void LoadMesh(const std::string &name);

};



#endif // MeshManager_h__
