#include "ModelManager.h"
#include "..\tools\tiny_obj_loader.h"
#include "..\tools\Log.h"
#include <boost\filesystem.hpp>
#include <fstream>
#include "..\rapidjson\document.h"

ModelManager::ModelManager()
{
}


ModelManager::~ModelManager()
{
}

VertexArray *ModelManager::LoadMesh(const std::string &name, const glm::vec4 &uv)
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

  auto vao = new VertexArray;
  {
    auto &buf = vao->CreateBuffer(vertex_size);
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
    auto &buf = vao->CreateIndexBuffer(index_size);
    auto *data = static_cast<unsigned int *>(buf.Data());
    for (size_t i = 0; i < shapes[0].mesh.indices.size(); ++i)
    {
      data[i] = shapes[0].mesh.indices[i];
    }
  }

  return vao;
}

void ModelManager::LoadModel(const std::string &name)
{
  std::string data;
  std::ifstream file(name, std::ios::in | std::ios::ate);
  if (file.is_open())
  {
    size_t size = static_cast<size_t>(file.tellg());
    file.seekg(0, std::ios_base::beg);
    data.resize(size);
    file.read(&data[0], size);
    file.close();
  }

  rapidjson::Document document;
  document.Parse<0>(data.c_str());

  auto &model = mModels[name];

  if (document.IsObject())
  {
    glm::vec4 uv;
    if (document.HasMember("texture"))
    {
      auto texture = document["texture"].GetString();
      uv = mTextureManager.GetTextureUV(texture);
      model.texture = mTextureManager.GetTexture(texture);
    }
    if (document.HasMember("mesh"))
    {
      auto mesh = document["mesh"].GetString();
      model.vertexArray = LoadMesh(mesh, uv);
      model.vertexArray->Compile();
    }
  }
}

void ModelManager::LoadDirectory(const std::string &dir)
{
  mTextureManager.LoadDirectory("data\\textures\\");
  mTextureManager.Compile();

  boost::filesystem::path targetDir(dir);
  boost::filesystem::recursive_directory_iterator iter(targetDir);

  int loaded = 0;
  for (const boost::filesystem::path &file : iter)
  {
    if (boost::filesystem::is_regular_file(file) && boost::filesystem::extension(file) == ".mdl")
    {
      LoadModel(file.string());
    }
  }
}

Model * ModelManager::GetModel(const std::string &name)
{
  auto itMesh = mModels.find(name);
  if (itMesh == mModels.end())
  {
    return nullptr;
  }

  return &(itMesh->second);
}

void ModelManager::Compile()
{
//   for (auto &mesh : mModels)
//   {
//     mesh.second.Compile();
//   }
}