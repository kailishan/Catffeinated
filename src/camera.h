#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "WindowManager.h"
#include "gameObject.h"
#include "audioManager.h"

class camera {
public:

  bool newKey = true;
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
  void processKeyboard(double ftime, std::vector<std::shared_ptr<gameObject>> objects);
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

  void setAudio(audioManager myAudio) { audio = myAudio; }

  void decrementHealth() { playerHealth = playerHealth - 1; }
  void incrementFrames() { invinFrames++; }
  void resetFrames() { invinFrames = 0; }
  void changeZMode() { zMode = !zMode; }

  float getSpeedBoost() { return speedBoost; }
  float getSscale() { return sScale; }
  int getHealth();
  void takeDamage();

  /*int initEngine(int id);
  void uninitEngine();
  void playMeow();
  void playRoost();
  void playCollect();
  void playPowerup();*/

  bool getDisplayDamage() { return displayDamage; }

private:
  glm::vec3 modeldirection;
  glm::mat4 cammodel;
  bool zMode = false;
  int playerHealth = 5;
  int invinFrames = 150;
  float dt = 0.0f;
  float baseSpeed = 1.0f;
  float speedBoost = 0.0f;
  int speedTimer = 0;
  float sScale = 0.5f; // strech & squash scale
  int sDir = 1;
  glm::vec3 up = glm::vec3(0, 1, 0);
  glm::vec3 pos, rot, front;

  bool firstMouse = true;
  float yaw = -90.0f; // yaw is initialized to -90.0 degrees since a yaw of 0.0
                      // results in a direction vector pointing to the right so
                      // we initially rotate a bit to the left.
  float pitch = 0.0f;
  float lastX = 0;
  float lastY = 0;

  int w = 0;
  int a = 0;
  int s = 0;
  int d = 0;
  int p = 0;
  int z = 0;
  GLFWwindow *window;
  float rad = 0.7f;

  // This should probably be in a different class. Okay for now
  int score = 0;

  // audio engine
  //ma_engine engine;
  audioManager audio;

  bool isDamaged = false;
  bool displayDamage = false; // true = red, false = texture
  int damageTimer = 0;
};