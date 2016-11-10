// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#ifndef RenderList_h__
#define RenderList_h__

#include <glm/glm.hpp>
#include <memory>
#include <vector>


class Render
{
public:
  Render();
  ~Render();

  static void Initialize();

private:
  glm::ivec2 mVersion;

};

#endif // RenderList_h__
