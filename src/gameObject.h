#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#include "Shape.h"

class gameObject {
public:
  virtual bool isColliding(std::shared_ptr<gameObject> other);
  void destroy(double ftime);
  virtual void process(std::vector<std::shared_ptr<gameObject>> others, int index, double ftime);
  std::shared_ptr<Shape> getMesh() { return mesh; }
  glm::vec3 getPos() { return pos; }
  float getRad() { return rad; }
  float getRot() { return rot; }
  bool getDestroying() { return destroying; }
  bool getDestroyed() { return destroyed; }
  bool getIsStatic() { return isStatic; }
  glm::mat4 getMatrix() { return matrix; }
  int getObjectType() { return objectType; }

  void setMesh(std::shared_ptr<Shape> newMesh) { mesh = newMesh; }
  void setDestroying(bool newVal) { destroying = newVal; }
  void setVelocity(glm::vec3 newVel) { vel = newVel; }
  void setRadius(float newRad) { rad = newRad; }
  void setIsStatic(bool newVal) { isStatic = newVal; }
  void setObjectType(int type) { objectType = type; };

  virtual void move(double ftime) = 0;
  
protected:
  std::shared_ptr<Shape> mesh = NULL;
  
  int objectType = NULL;
  bool isStatic = false;
  glm::vec3 pos, vel;
  float rad, rot;
  bool destroying = false;
  bool destroyed = false;
  glm::mat4 matrix = glm::mat4(1);
  virtual glm::mat4 formRotationMatrix(float frametime);
  glm::vec3 midpoint(std::shared_ptr<Shape> shape);
};