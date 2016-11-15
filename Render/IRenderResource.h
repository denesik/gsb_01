#pragma once
#ifndef IRenderResource_h__
#define IRenderResource_h__

#include <string>

class IRenderResource
{
public:
  virtual ~IRenderResource() = default;

  /// ��������� ������ � ���.
  /// ������ �� ��� ���������.
  virtual void Compile() = 0;

  /// ������� ������ �� ���.
  virtual void Release() = 0;

};

#endif // IRenderResource_h__
