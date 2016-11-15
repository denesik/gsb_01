#pragma once
#ifndef IRenderResource_h__
#define IRenderResource_h__

#include <string>

class IRenderResource
{
public:
  virtual ~IRenderResource() = default;

  /// Загрузить данные в гпу.
  /// Данные из озу удаляются.
  virtual void Compile() = 0;

  /// Удалить данные из гпу.
  virtual void Release() = 0;

};

#endif // IRenderResource_h__
