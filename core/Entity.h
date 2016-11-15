#pragma once
#ifndef Entity_h__
#define Entity_h__

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>


class Entity
{
public:
  Entity();
  ~Entity();

  void Rotate(const glm::vec3 &degrees);

  void Move(const glm::vec3 &dist);

  void Update();

  const glm::vec3 &GetPos() const 
  {
    return mPos;
  }

  const glm::quat &GetQuat() const 
  {
    return mQuat;
  }

private:
  glm::quat mQuat;
  glm::vec3 mDir;
  glm::vec3 mPos;
  glm::mat4 mModel;

};



#endif // Entity_h__
