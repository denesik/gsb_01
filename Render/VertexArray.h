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
  /// Выделяет память размером size байт.
  VertexBuffer(size_t size);
  ~VertexBuffer();

  void SetAttibute(const Attribute &attribute);

  void *Data() const;

  size_t Size() const;

private:
  void *mData;
  const size_t mSize;

  std::vector<Attribute> mAttributs;

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

  void *Data() const;

  size_t Size() const;

private:
  void *mData;
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
