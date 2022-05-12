#include "camera.h"

float distance(float x1, float y1, float z1, float x2, float y2, float z2) {
  float d = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z2 - z1, 2) * 1.0);
  return d;
}

bool camera::isColliding(gameObject other) {
  float d = distance(-pos.x, -pos.y, -pos.z, other.getPos().x, other.getPos().y,
                     other.getPos().z);
  // ?????????????????????????????????????????????
  d = glm::distance(pos, other.getPos());
  if (d > rad + other.getRad()) {
    return false;
  } else if (d <= rad + other.getRad() && !other.getDestroying() && other.getObjectType() == 1) {
    score++;
    if ((score > 0) && (score % 10 == 0)) {
      speedBoost = 3;
      std::cout << "speed boost activated" << std::endl;
    }
    std::cout << "Kibble Collected: " << score << std::endl;
    // cout << "(CAM) " << "x: " << pos.x << " z: " << pos.z << endl;
    other.setDestroying(true);
    return true;
  } else if (d <= rad + other.getRad() &&  other.getObjectType() == 2) {
      return true;
  } else if (d <= rad + other.getRad() && !other.getDestroying() && other.getObjectType() == 0)
  {
    takeDamage();
    return false;
  } else
    return false;
}

/* change camera position wrt keyboard input */
void camera::processKeyboard(double ftime, std::vector<gameObject> *objects) {
  incrementFrames();
  float cameraSpeed = 5.0f * ftime * speedBoost;
  glm::vec3 nextPos = pos;
  glm::vec3 prevPos = pos;

  if (speedBoost > 1) {
    speedBoost -= 0.1;
    std::cout << "speed boost = " << speedBoost << std::endl;
  }
  else
    speedBoost = 1;


  if (w)
    nextPos += cameraSpeed * front;
  if (s)
    nextPos -= cameraSpeed * front;
  if (a)
    nextPos -= glm::normalize(glm::cross(front, up)) * cameraSpeed;
  if (d)
    nextPos += glm::normalize(glm::cross(front, up)) * cameraSpeed;
  if (z && dt > 1.0)
  {
      dt = 0.0;
      changeZMode();
      if (zMode)
          std::cout << "Zoe Mode: ON" << std::endl;
      else
          std::cout << "Zoe Mode: OFF" << std::endl;
  }

  for (int i = 0; i < objects->size(); i++)
  {
      pos = nextPos;
      if (isColliding((*objects)[i])) {
          pos = prevPos;
          return;
      }
      else
          pos = prevPos;
  }


  if (nextPos.x > 12.5 || nextPos.x < -12.5 || nextPos.z > 12.5 ||
      nextPos.z < -12.5)
      pos = pos;
  else
      pos = nextPos;

  pos.y = 1.0f;
}


int camera::getHealth()
{
    return playerHealth;
}

void camera::takeDamage()
{
  if (dt > 3.0f && !zMode) {
    dt = 0.0f;
    decrementHealth();
    std::cout << "Health: " << playerHealth << std::endl;
  }
  /*
    if (invinFrames > 150)
    {
        resetFrames();
        decrementHealth();
        std::cout << "Health: " << playerHealth << std::endl;
        //std::cout << "OUCH" << std::endl;
    }
    */
    if (playerHealth == 0)
        exit(0);
}

void camera::processCursor(float xoffset, float yoffset) {
  float sensitivity = 0.25f;

  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  // previously 89.0f
  if (pitch > 20.0f)
    pitch = 20.0f;
  if (pitch < -20.0f)
    pitch = -20.0f;

  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

  modeldirection = direction;

  front = glm::normalize(direction);
}

// currently unused
glm::mat4 camera::process(double ftime) {
  if (p == 1) {
    // cout << "(CAM) " << "x: " << pos.x << " z: " << pos.z << endl;
    p = 0;
  }
  float speed = 0;
  double xpos, ypos;

  if (w == 1 || a == 1) {
    speed = 6 * ftime;
  } else if (s == 1 || d == 1) {
    speed = -6 * ftime;
  }
  float yangle = 0;

  glfwGetCursorPos(window, &xpos, &ypos);
  xpos = xpos * 0.005;
  ypos = ypos * 0.005;

  glm::mat4 R = glm::rotate(glm::mat4(1), (float)xpos, glm::vec3(0, 1, 0));
  glm::mat4 R2 = glm::rotate(glm::mat4(1), (float)ypos, glm::vec3(1, 0, 0));

  glm::vec4 dir = glm::vec4(0, 0, 0, 1);

  if (w == 1 || s == 1)
    dir = glm::vec4(0, 0, speed, 1);
  else if (a == 1 || d == 1)
    dir = glm::vec4(speed, 0, 0, 1);

  dir = dir * R;

  if (pos.x + dir.x > 12.5 || pos.x + dir.x < -12.5 || pos.z + dir.z > 12.5 ||
      pos.z + dir.z < -12.5)
    pos = pos;
  else
    pos += glm::vec3(dir.x, dir.y, dir.z);

  // cout << "x: " << pos.x << " z: " << pos.z << endl;
  glm::mat4 T = glm::translate(glm::mat4(1), pos);

  cammodel = R2;
  return R2 * R * T;
}