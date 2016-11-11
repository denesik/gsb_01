#pragma once
#ifndef VertexArray_h__
#define VertexArray_h__

#include <vector>
#include "Vertex.h"
#include <stdint.h>
#include <memory>

class VertexBuffer
{
public:
  VertexBuffer(size_t size);
  ~VertexBuffer();

  void SetAttibute(const Attribute &attribute, int location);

  float *Data() const;

  size_t Size() const;

private:
  float *mData;
  const size_t mSize;
  struct Location
  {
    Attribute attribute;
    int location;
  };

  std::vector<Location> mLocations;

  unsigned int mVbo = 0;

private:
  friend class VertexArray;
  void Create();
  void Release();
  void Compile();
};


class IndexBuffer
{
public:
  IndexBuffer(size_t size);
  ~IndexBuffer();

  int32_t *Data() const;

  size_t Size() const;

private:
  int32_t *mData;
  const size_t mSize;

  unsigned int mVbi = 0;

private:
  friend class VertexArray;
  void Create();
  void Release();
  void Compile();
  void Draw();
};

class VertexArray
{
public:
  VertexArray();
  ~VertexArray();

  VertexBuffer &CreateBuffer(size_t size);

  IndexBuffer &CreateIndexBuffer(size_t size);

  void Compile();

  void Draw();

  void Release();

private:
  unsigned int mVao = 0;

  std::vector<VertexBuffer> mVertexBuffers;
  std::unique_ptr<IndexBuffer> mIndexBuffer;

};



#endif // VertexArray_h__
