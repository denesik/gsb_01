#include "ShaderManager.h"

#include <boost\filesystem.hpp>


ShaderManager::ShaderManager()
{
}


ShaderManager::~ShaderManager()
{
}

void ShaderManager::LoadDirectory(const std::string &directory)
{
  boost::filesystem::path targetDir(directory);
  boost::filesystem::recursive_directory_iterator iter(targetDir);

  for (const boost::filesystem::path &file : iter)
  {
    if (boost::filesystem::is_regular_file(file) && boost::filesystem::extension(file) == ".glsl")
    {
      LoadShader(file.string());
    }
  }
}

Shader * ShaderManager::GetShader(const std::string &name)
{
  auto it = mShaders.find(name);
  if (it == mShaders.end())
  {
    return nullptr;
  }

  return &(it->second);
}

void ShaderManager::Compile()
{
  for (auto &shader : mShaders)
  {
    shader.second.Compile();
  }
}

void ShaderManager::LoadShader(const std::string &name)
{
  mShaders[name].LoadFromFile(name);
}
