#pragma once
#ifndef Model_h__
#define Model_h__

#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"


class Model
{
public:
  Shader *shader;
  Texture *texture;
  VertexArray *vertexArray;
};


#endif // Model_h__
