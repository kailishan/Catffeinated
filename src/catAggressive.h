#pragma once

#include "camera.h"
#include "gameObject.h"

class catAggressive : public gameObject {
public:
  catAggressive(std::shared_ptr<Shape> shape, camera *p, std::vector<std::shared_ptr<gameObject>> data);
  void move(double ftime) override;
  bool isColliding(std::shared_ptr<gameObject> other) override;

private:
  camera *player;
  glm::mat4 formRotationMatrix(float frametime) override;
  std::vector<std::shared_ptr<gameObject>> collisionData;
};