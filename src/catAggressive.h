#pragma once

#include "camera.h"
#include "gameObject.h"

class catAggressive : public gameObject {
public:
  catAggressive(std::shared_ptr<Shape> shape, camera *p);
  void move(double ftime) override;
  bool isColliding(std::shared_ptr<gameObject> other) override;

private:
  camera *player;
  glm::mat4 formRotationMatrix(float frametime) override;
};