#include "Entity.h"

#include <glm/gtc/matrix_transform.hpp>


Entity::Entity()
{

  mPos = { 0, 0, 5 };
}


Entity::~Entity()
{
}

void Entity::Rotate(const glm::vec3 &degrees)
{
  mDir += degrees;
}

void Entity::Move(const glm::vec3 &dist)
{
  mPos += glm::vec3(dist.x, dist.y, dist.z) * mQuat;
}

void Entity::Update()
{
  const auto &pitch = glm::angleAxis(mDir.x, glm::vec3(1, 0, 0));
  const auto &yaw = glm::angleAxis(mDir.y, glm::vec3(0, 1, 0));
  const auto &roll = glm::angleAxis(mDir.z, glm::vec3(0, 0, -1));
  mDir = {};

  mQuat = pitch * mQuat * yaw;
  mQuat = glm::normalize(mQuat);

  mModel = glm::translate(glm::mat4_cast(mQuat), mPos);
}
