#include "VertexArray.h"

#include <gl/glew.h>
#include "OpenGLCall.h"

VertexArray::VertexArray()
{
}


VertexArray::~VertexArray()
{
}

VertexBuffer & VertexArray::CreateBuffer(size_t size)
{
  mVertexBuffers.emplace_back(size);
  return mVertexBuffers.back();
}

IndexBuffer & VertexArray::CreateIndexBuffer(size_t size)
{
  mIndexBuffer = std::make_unique<IndexBuffer>(size);
  return *mIndexBuffer;
}

void VertexArray::Compile()
{
  GL_CALL(glGenVertexArrays(1, &mVao)); // 1
  GL_CALL(glBindVertexArray(mVao));
  for (auto &buf : mVertexBuffers)
    buf.Create();
  if (mIndexBuffer)
    mIndexBuffer->Create();
  for (auto &buf : mVertexBuffers)
    buf.Compile();
  if (mIndexBuffer)
    mIndexBuffer->Compile();
}

void VertexArray::Draw()
{
  if (mIndexBuffer)
  {
    GL_CALL(glBindVertexArray(mVao));
    mIndexBuffer->Draw();
  }
}

void VertexArray::Release()
{
  GL_CALL(glDeleteVertexArrays(1, &mVao));
  for (auto &buf : mVertexBuffers)
  {
    buf.Release();
  }
  if (mIndexBuffer)
  {
    mIndexBuffer->Release();
  }
}

VertexBuffer::VertexBuffer(size_t size)
  : mSize(size)
{
  mData = new float[mSize];
}

VertexBuffer::~VertexBuffer()
{
  if (mData)
  {
    delete[] mData;
    mData = nullptr;
  }
}

void VertexBuffer::SetAttibute(const Attribute &attribute)
{
  mAttributs.push_back(attribute);
}

void VertexBuffer::Create()
{
  GL_CALL(glGenBuffers(1, &mVbo));
}

void VertexBuffer::Release()
{
  GL_CALL(glDeleteBuffers(1, &mVbo));
}

void VertexBuffer::Compile()
{
  if (mData)
  {
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);

    size_t mVertexSize = 0;
    for (const auto &attribute : mAttributs)
    {
      mVertexSize += attribute.size;
    }

    for (const auto &attribute : mAttributs)
    {
      glVertexAttribPointer(attribute.location, attribute.size / sizeof(float),
        GL_FLOAT, GL_FALSE, mVertexSize, (char *)NULL + attribute.offset);
      glEnableVertexAttribArray(attribute.location);
    }

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, mVbo));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, mSize * sizeof(float), mData, GL_STATIC_DRAW));

    //delete[] mData;
    //mData = nullptr;
  }
}

void IndexBuffer::Draw()
{
  GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVbi));
  GL_CALL(glDrawElements(GL_TRIANGLES, mSize, GL_UNSIGNED_INT, NULL));
}

float *VertexBuffer::Data() const
{
  return mData;
}

size_t VertexBuffer::Size() const
{
  return mSize;
}

IndexBuffer::IndexBuffer(size_t size)
  : mSize(size)
{
  mData = new int32_t[mSize];
}

IndexBuffer::~IndexBuffer()
{
  if (mData)
  {
    delete[] mData;
    mData = nullptr;
  }
}

int32_t * IndexBuffer::Data() const
{
  return mData;
}

size_t IndexBuffer::Size() const
{
  return mSize;
}

void IndexBuffer::Create()
{
  GL_CALL(glGenBuffers(1, &mVbi));
  GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVbi));
}

void IndexBuffer::Release()
{
  GL_CALL(glDeleteBuffers(1, &mVbi));
}

void IndexBuffer::Compile()
{
  if (mData)
  {
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVbi));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, mSize * sizeof(uint32_t), mData, GL_STATIC_DRAW));
    delete[] mData;
    mData = nullptr;
  }
}
