#include "gameManager.h"

void gameManager::spawnGameObject(std::shared_ptr<Shape> shape)
{
  std::shared_ptr<gameObject> object = std::make_shared<gameObject>(shape);
  object->setObjectType(0);
  objects.push_back(object);
  count++;
}

// temp kibble code
void gameManager::spawnStaticGameObject(std::shared_ptr<Shape> shape)
{
  std::shared_ptr<gameObject> object = std::make_shared<gameObject>(shape);
  object->setObjectType(1);
  object->setVelocity(glm::vec3(0, 0, 0));
  object->setRadius(0.1f);
  object->setIsStatic(true);
  object->setMesh(shape);
  objects.push_back(object);
  count++;
}

/*
int gameManager::getHealth()
{
    return playerHealth;
}

void gameManager::takeDamage()
{
    playerHealth--;
    if (playerHealth == 0)
        exit(0);
}
*/

void gameManager::process(camera &mycam, double ftime)
{
  //std::vector<int> destroyList;
  for (int i = 0; i < objects.size(); i++)
  {
    if (mycam.isColliding(objects.at(i)) &&  // CHECK COLLISION W/ PLAYER
        !objects.at(i)->getDestroying())      // IF DESTROYING
    {
      spawnStaticGameObject(objects.at(i)->getMesh());
      objects.at(i)->setDestroying(true);
      count--;
      //std::cout << "CATS REMAINING: " << count << std::endl;
      //score++;
      //cout << "OBJECTS DESTROYED: " << score << endl;
    }

    objects.at(i)->process(objects, i,ftime); // CHECK COLLISION W/ GAME OBJECTS
    if (objects.at(i)->getDestroyed()) // DESTROY OBJECT
    {
      //if (std::find(destroyList.begin(), destroyList.end(), i) !=
      //    destroyList.end())
      //  destroyList.push_back(i);
      objects.erase(objects.begin() + i);
      i++;
    }
  }
  
  /*
  // destroy list
  if (destroyList.size() == 0)
    return;
  for (int i = destroyList.size() - 1; i >= 0; i--) {
    objects.erase(objects.begin() + i);
    count--;
  }
  */

  framecount++;
  if (count < 15 && framecount > 150) {
    count++;
    //std::cout << "CATS REMAINING: " << count << std::endl;
    framecount = 0;
    spawnStaticGameObject(curShape);
  }
}