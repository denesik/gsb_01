// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "TextureManager.h"

#include <iostream>
#include <boost\filesystem.hpp>
#include "ErrorImage.h"
#include "..\tools\Log.h"

TextureManager::TextureManager()
{
  mMultiAtlas.resize(1);
  Bitmap bitmap(glm::uvec2(error_image.width, error_image.height), 
    reinterpret_cast<const unsigned char *>(error_image.pixel_data));
  auto pos = mMultiAtlas[0].atlas.Add(error_image.name, bitmap);
  mTextures[error_image.name] = { 0, pos };
}

void TextureManager::LoadTexture(const std::string &name)
{
  for (unsigned int i = 0; i < mMultiAtlas.size(); ++i)
  {
    if (LoadToAtlas(i, name))
    {
      // Текстура загружена. Выходим.
      return;
    }
  }
  // Мы пытались, но не смоги...
  // Попробуем создать еще один атлас.
  mMultiAtlas.resize(mMultiAtlas.size() + 1);
  if (!LoadToAtlas(mMultiAtlas.size() - 1, name))
  {
    // Ошибка.
    LOG(warning) << "Текстура не загружена в атлас. ";
  }
}

void TextureManager::LoadDirectory(const std::string &dir)
{
  boost::filesystem::path targetDir(dir);
  boost::filesystem::recursive_directory_iterator iter(targetDir);

  int loaded = 0;
  for (const boost::filesystem::path &file : iter) 
  {
    if (boost::filesystem::is_regular_file(file) && boost::filesystem::extension(file) == ".png")
    {
      LoadTexture(file.string());
    }
  }
}


//TODO:лучше использовать сразу текстурные координаты, вместо отступа и размера в пикселях
Texture *TextureManager::GetTexture(const std::string &name) const
{
  auto itTexture = mTextures.find(name);
  if (itTexture == mTextures.end())
  {
    // Текстура не найдена.
    const auto &texture = mTextures.find(error_image.name)->second;
    return mMultiAtlas[texture.index].texture.get();
  }

  return mMultiAtlas[(*itTexture).second.index].texture.get();
}

glm::vec4 TextureManager::GetTextureUV(const std::string &name) const
{
  auto itTexture = mTextures.find(name);
  if (itTexture == mTextures.end())
  {
    // Текстура не найдена.
    const auto &texture = mTextures.find(error_image.name)->second;
    return texture.uv;
  }

  return (*itTexture).second.uv;
}


void TextureManager::Compile()
{
  LOG(trace) << "texture atlases recompiling";
  for (unsigned int i = 0; i < mMultiAtlas.size(); ++i)
  {
    mMultiAtlas[i].texture = std::make_unique<Texture>(mMultiAtlas[i].atlas.GetAtlas(), false);
    mMultiAtlas[i].texture->GenMipmap();
    mMultiAtlas[i].atlas.GetAtlas().Save("atlas.png");
  }

  for (auto &texture : mTextures)
  {
    const auto &pos = texture.second.pos;

    glm::vec2 coord[] =
    {
      { pos.x,         pos.y },
      { pos.x + pos.z, pos.y + pos.w },
    };
    glm::vec2 scale(1.0f / (static_cast<glm::vec2>(mMultiAtlas[texture.second.index].texture->GetSize())));
    coord[0] *= scale;
    coord[1] *= scale;

    texture.second.uv = { coord[0].x, coord[0].y, coord[1].x, coord[1].y };
  }

  LOG(trace) << "done";
}

bool TextureManager::LoadToAtlas(size_t atlas, const std::string &name)
{
  Bitmap bitmap;
  try
  {
    bitmap = Bitmap(name);
  }
  catch (char *)
  {
    return false;
  }
  auto pos = mMultiAtlas[atlas].atlas.Add(name, bitmap);
  mTextures[name] = { atlas, pos };

  return true;
}
