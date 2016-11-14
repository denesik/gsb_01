// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#ifndef Vertex_h__
#define Vertex_h__

#include <glm/glm.hpp>
#include <vector>
#include "VertexTools.h"

enum AttributeLocations
{
  Location_vertex = 1,
  Location_texture = 2,
  Location_normal = 3,
};

/// Конфигурация атрибута.
struct Attribute
{
  int location;
  size_t size;
  size_t offset;
  const char *name;
};


/// Сформирует структуру вершины и статическую информацию 
/// об атрибуте для каждой компоненты вершины.
VERTEX
(
  (VertexVTN)
  (glm::vec3, vertex)
  (glm::vec2, texture)
  (glm::vec3, normal)
)

VERTEX
(
  (VertexVT)
  (glm::vec3, vertex)
  (glm::vec2, texture)
)

#endif // Vertex_h__

