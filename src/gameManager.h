#pragma once

#include "camera.h"
#include "gameObject.h"
#include "GLFW/glfw3.h"
#include "Shape.h"

class gameManager {
public:
  gameManager(std::shared_ptr<Shape> shape) {
    curShape = shape;
    srand(glfwGetTime());
    while (count <= 14) {
      spawnGameObject(curShape);
    }
  }

  void spawnGameObject(std::shared_ptr<Shape> shape);
  void process(camera mycam, double ftime);

  std::vector<gameObject> getObjects() { return objects; }

  private:
  int maxObj = 15;
  int count = 0;
  int bound = 12.5; // min/max x/y
  int score = 0;
  int framecount = 0;
  std::shared_ptr<Shape> curShape;
  std::vector<gameObject> objects;
};