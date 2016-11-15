#include "MeshManager.h"
#include "..\tools\tiny_obj_loader.h"
#include "..\tools\Log.h"
#include <boost\filesystem.hpp>


MeshManager::MeshManager()
{
}


MeshManager::~MeshManager()
{
}

void MeshManager::LoadMesh(const std::string &name)
{
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> mats;
  std::string err;

  bool ok = tinyobj::LoadObj(shapes, mats, err, name.c_str());
  if (!ok)
  {
    LOG(error) << err;
  }

  size_t vertex_size = (
    shapes[0].mesh.positions.size() +
    shapes[0].mesh.texcoords.size() +
    shapes[0].mesh.normals.size()) * sizeof(float);

  size_t index_size = shapes[0].mesh.indices.size() * sizeof(unsigned int);

  auto &vao = mMeshes[name];
  {
    auto &buf = vao.CreateBuffer(vertex_size);
    auto *data = static_cast<float *>(buf.Data());
    size_t offset = 0;
    for (size_t i = 0; i < shapes[0].mesh.positions.size() / 3; ++i)
    {
      data[offset] = shapes[0].mesh.positions[i * 3 + 0]; ++offset;
      data[offset] = shapes[0].mesh.positions[i * 3 + 1]; ++offset;
      data[offset] = shapes[0].mesh.positions[i * 3 + 2]; ++offset;
      data[offset] = shapes[0].mesh.texcoords[i * 2 + 0]; ++offset;
      data[offset] = shapes[0].mesh.texcoords[i * 2 + 1]; ++offset;
      data[offset] = shapes[0].mesh.normals[i * 3 + 0]; ++offset;
      data[offset] = shapes[0].mesh.normals[i * 3 + 1]; ++offset;
      data[offset] = shapes[0].mesh.normals[i * 3 + 2]; ++offset;
    }

    for (auto &attibute : VertexVTN::Get())
    {
      buf.SetAttibute(attibute);
    }
  }
  {
    auto &buf = vao.CreateIndexBuffer(index_size);
    auto *data = static_cast<unsigned int *>(buf.Data());
    for (size_t i = 0; i < shapes[0].mesh.indices.size(); ++i)
    {
      data[i] = shapes[0].mesh.indices[i];
    }
  }
}

void MeshManager::LoadDirectory(const std::string &dir)
{
  boost::filesystem::path targetDir(dir);
  boost::filesystem::recursive_directory_iterator iter(targetDir);

  int loaded = 0;
  for (const boost::filesystem::path &file : iter)
  {
    if (boost::filesystem::is_regular_file(file) && boost::filesystem::extension(file) == ".obj")
    {
      LoadMesh(file.string());
    }
  }
}

VertexArray * MeshManager::GetMesh(const std::string &name)
{
  auto itMesh = mMeshes.find(name);
  if (itMesh == mMeshes.end())
  {
    return nullptr;
  }

  return &(itMesh->second);
}

void MeshManager::Compile()
{
  for (auto &mesh : mMeshes)
  {
    mesh.second.Compile();
  }
}
