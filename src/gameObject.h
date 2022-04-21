#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#include "Shape.h"

class gameObject {
public:
  gameObject(std::shared_ptr<Shape> shape);
  bool isColliding(gameObject other);
  void destroy(double ftime);
  void move(double ftime);
  void process(std::vector<gameObject> others, int index, double ftime);
  std::shared_ptr<Shape> getMesh() { return mesh; }
  glm::vec3 getPos() { return pos; }
  float getRad() { return rad; }
  float getRot() { return rot; }
  bool getDestroying() { return destroying; }
  bool getDestroyed() { return destroyed; }
  glm::mat4 getMatrix() { return matrix; }

  void setMesh(std::shared_ptr<Shape> newMesh) { mesh = newMesh; }
  void setDestroying(bool newVal) { destroying = newVal; }

  //temp
  std::shared_ptr<Shape> kibble;
  //

private:
  std::shared_ptr<Shape> mesh;

  bool isStatic = false;
  glm::vec3 pos, vel;
  float rad, rot;
  bool destroying = false;
  bool destroyed = false;
  glm::mat4 matrix = glm::mat4(1);
  glm::mat4 formRotationMatrix(float frametime);
  glm::vec3 midpoint(std::shared_ptr<Shape> shape);
};