// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#ifndef TextureManager_h__
#define TextureManager_h__

#include "Texture.h"
#include <string>
#include <unordered_map>
#include "Atlas.h"
#include <memory>


/// Менеджер текстур.
/// Хранит все игровые текстуры.
/// TODO: Кеширование.
class TextureManager
{
public:
  TextureManager(const glm::uvec2 &size);

  /// Загрузить все текстуры в директории.
  void LoadDirectory(const std::string &directory);

  /// Получить текстуру по имени текстуры.
  Texture *GetTexture(const std::string &name) const;

  /// Получить текстурные координаты по имени текстуры.
  glm::vec4 GetTextureUV(const std::string &name) const;

  /// Создает текстуры в графической памяти.
  void Compile();

  /// Формирует текстурные координаты.
  void GenerateUV();

private:
  glm::uvec2 mSize;

  struct AtlasChunk
  {
    size_t index;       // Номер атласа.
    glm::uvec4 pos;      // Положение в атласе.
    glm::vec4 uv;
  };

  struct AtlasTexture
  {
    AtlasTexture(const glm::uvec2 &size)
      : atlas(size)
    {};
    Atlas atlas;
    std::unique_ptr<Texture> texture;
  };

  std::vector<AtlasTexture> mMultiAtlas;

  std::unordered_map<std::string, AtlasChunk> mTextures;

private:
  /// Загрузить указанную текстуру.
  void LoadTexture(const std::string &name);

  bool LoadToAtlas(size_t atlas, const std::string &name);
};



#endif // TextureManager_h__
