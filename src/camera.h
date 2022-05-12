#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "WindowManager.h"
#include "gameObject.h"

class camera {
public:
  camera() {
    window = nullptr;
    pos = glm::vec3(0, 0.85f, 0);
    rot = glm::vec3(0, 0, 0);
    front = glm::vec3(0, 0.85f, -1);
  }

  camera(GLFWwindow *win) {
    window = win;
    pos = glm::vec3(0, -1, 0);
    rot = glm::vec3(0, 0, 0);
    front = glm::vec3(0, -1, -6);
  }

  bool isColliding(std::shared_ptr<gameObject> other);
  void processKeyboard(double ftime);
  void processCursor(float xoffset, float yoffset);
  glm::mat4 process(double ftime);

  bool isFirstMouse() { return firstMouse; }
  glm::vec3 getPos() { return pos; }
  glm::vec3 getFront() { return front; }
  glm::vec3 getUp() { return up; }
  float getLastX() { return lastX; }
  float getLastY() { return lastY; }
  int getIFrames() { return invinFrames; }
  float getDt() { return dt; }
  glm::mat4 getModel() { return cammodel; }
  glm::vec3 getModelDirection() { return modeldirection; }
  int wkey() { return w; }
  int akey() { return a; }
  int skey() { return s; }
  int dkey() { return d; }
  int zkey() { return z; }


  void setDt(float newDelta) { dt = newDelta; }
  void toggleFirstMouse() { firstMouse = false; }
  void setWindow(GLFWwindow *win) { window = win; }
  void setFront(glm::vec3 direction) { front = direction; }
  void setLastX(float x) { lastX = x; }
  void setLastY(float y) { lastY = y; }
  void setw(int newW) { w = newW; }
  void seta(int newA) { a = newA; }
  void sets(int newS) { s = newS; }
  void setd(int newD) { d = newD; }
  void setp(int newP) { p = newP; }
  void setz(int newZ) { z = newZ; }

  void decrementHealth() { playerHealth = playerHealth - 1; }
  void incrementFrames() { invinFrames++; }
  void resetFrames() { invinFrames = 0; }
  void changeZMode() { zMode = !zMode; }

  float getSpeedBoost() { return speedBoost; }

  int getHealth();
  void takeDamage();

private:
  glm::vec3 modeldirection;
  glm::mat4 cammodel;
  bool zMode = false;
  int playerHealth = 5;
  int invinFrames = 150;
  float dt = 0.0f;
  float speedBoost = 1.0f;
  glm::vec3 up = glm::vec3(0, 1, 0);
  glm::vec3 pos, rot, front;

  bool firstMouse = true;
  float yaw = -90.0f; // yaw is initialized to -90.0 degrees since a yaw of 0.0
                      // results in a direction vector pointing to the right so
                      // we initially rotate a bit to the left.
  float pitch = 0.0f;
  float lastX = 1920.0f / 2.0;
  float lastY = 1080.0f / 2.0;

  int w = 0;
  int a = 0;
  int s = 0;
  int d = 0;
  int p = 0;
  int z = 0;
  GLFWwindow *window;
  float rad = 1.0f;

  // This should probably be in a different class. Okay for now
  int score = 0;
};