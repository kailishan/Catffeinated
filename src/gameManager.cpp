#include "gameManager.h"

void gameManager::spawnGameObject(std::shared_ptr<Shape> shape) {
  gameObject object = gameObject(shape);
  objects.push_back(object);
  count++;
}

// temp kibble code
void gameManager::spawnStaticGameObject(std::shared_ptr<Shape> shape) {
    gameObject object = gameObject(shape);
    object.vel = 0;
    object.rad = 0.1;
    object.isStatic = true;
    object.setMesh(object.kibble);
    objects.push_back(object);
    count++;
}

void gameManager::process(camera mycam, double ftime) {
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