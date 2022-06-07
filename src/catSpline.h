#pragma once

#include "gameObject.h"
#include "Spline.h"

class catSpline : public gameObject {
public:
  catSpline(std::shared_ptr<Shape> shape, std::vector<std::shared_ptr<gameObject>> data);
  void move(double ftime) override;

private:
  Spline splinepath[2];
  glm::mat4 formRotationMatrix(float frametime, glm::vec3 oldPos);
  std::vector<std::shared_ptr<gameObject>> collisionData;
};