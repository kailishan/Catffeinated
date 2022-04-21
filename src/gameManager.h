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

  void spawnGameObject(std::shared_ptr<Shape> shape) {
    gameObject object = gameObject(shape);
    objects.push_back(object);
    count++;
  }

  std::vector<gameObject> getObjects() { return objects; }


  void process(camera mycam, double ftime) {
    std::vector<int> destroyList;
    for (int i = 0; i < objects.size(); i++) {
      if (mycam.isColliding(objects.at(i)) &&
          !objects.at(i).getDestroying()) // CHECK COLLISION W/ PLAYER
      {
        objects.at(i).setDestroying(true);
        count--;
        std::cout << "CATS REMAINING: " << count << std::endl;
        // score++;
        // cout << "OBJECTS DESTROYED: " << score << endl;
      }
      objects.at(i).process(objects, i,
                            ftime);     // CHECK COLLISION W/ GAME OBJECTS
      if (objects.at(i).getDestroyed()) // DESTROY OBJECT
      {
        if (std::find(destroyList.begin(), destroyList.end(), i) !=
            destroyList.end())
          destroyList.push_back(i);
      }
    }

    // destroy list
    if (destroyList.size() == 0)
      return;
    for (int i = destroyList.size() - 1; i >= 0; i--) {
      objects.erase(objects.begin() + i);
      count--;
    }

    framecount++;
    if (count < 10 && framecount > 300) {
      count++;
      std::cout << "CATS REMAINING: " << count << std::endl;
      framecount = 0;
      spawnGameObject(curShape);
    }
  }

  private:
  int maxObj = 15;
  int count = 0;
  int bound = 12.5; // min/max x/y
  int score = 0;
  int framecount = 0;
  std::shared_ptr<Shape> curShape;
  std::vector<gameObject> objects;
};