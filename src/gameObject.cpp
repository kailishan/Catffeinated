#include "gameObject.h"



gameObject::gameObject(glm::vec3 position, float radius)
{
    pos = position;
    rad = radius;
    vel = glm::vec3(0);
    rot = 0.0;
}

gameObject::gameObject(std::shared_ptr<Shape> shape) {
  mesh = shape;
  // pos = glm::vec3(rand() % 25 - 12, 0, rand() % 25 - 12);
  pos = glm::vec3((rand() % 25) - 12, 0.35, (rand() % 25) - 12);
  // rot = glm::radians((float)(rand() % 361)); // y-axis
  // vel = vec3(0, 0, 0); // random x and y velocity
  float velocities[] = {0.05f, 0.025f, 0.0125f, -0.0125f, -0.025f, -0.05f};
  vel = glm::vec3(velocities[rand() % 6], 0.0f, velocities[rand() % 6]);
  rot = tan(vel.z / vel.x);
  // vel = vec3(static_cast <float> (rand()) / static_cast <float> (1) *
  // 0.00000000075, 0, static_cast <float> (rand()) / static_cast <float> (1) *
  // 0.00000000075); // random x and y velocity
  glm::vec3 posDirection = glm::normalize(pos);
  glm::vec3 velDirection = glm::normalize(vel);
  float angle = acos(glm::dot(posDirection, velDirection));

  rad = .5;
  std::cout << "x: " << pos.x << " z: " << pos.z << std::endl;
}


bool gameObject::isColliding(gameObject other) {
  if (destroying || other.destroying)
    return false;
  float d = glm::distance(pos, other.pos);
  if (d > rad + other.rad)
    return false;
  else if (d <= rad + other.rad && !other.destroying) {
    vel.x = -vel.x;
    vel.z = -vel.z;
    return true;
  } else
    return false;
}

void gameObject::destroy(double ftime) {
  rad -= 0.01;
  vel.x = 0;
  vel.y = 0;
  if (rad <= 0) {
    // destroying = false;
    // cout << "(CAT) x:" << pos.x << " z: " << pos.z << endl;
    destroyed = true;
  }
}

void gameObject::move(double ftime) {
  /*
  if (pos.x > 12.5 && vel.x > 0)
      vel.x = -vel.x;
  if (pos.x < -12.5 && vel.x < 0)
      vel.x = -vel.x;
  if (pos.z > 12.5 && vel.z > 0)
      vel.z = -vel.z;
  if (pos.z < -12.5 && vel.z < 0)
      vel.z = -vel.z;
  */
  rot = atan(vel.z / vel.x);
  if (vel.z < 0)
    rot += glm::radians(180.f);

  glm::mat4 R = glm::rotate(glm::mat4(1), rot, glm::vec3(0.0f, 1.0f, 0.0f));
  R = formRotationMatrix(ftime);
  glm::vec4 dir = glm::vec4(vel, 1);

  if (pos.x + dir.x > 12.5 || pos.x + dir.x < -12.5) {
    pos = pos;
    vel.x = -vel.x;
  } else if (pos.z + dir.z > 12.5 || pos.z + dir.z < -12.5) {
    pos = pos;
    vel.z = -vel.z;
  } else
    pos += glm::vec3(dir.x, dir.y, dir.z);

  glm::mat4 T = glm::translate(glm::mat4(1), pos);
  matrix = T * R;
}

void gameObject::process(std::vector<gameObject> others, int index,
                         double ftime) {
  if (destroyed)
    return;
  if (destroying) {
    destroy(ftime);
    return;
  } else {
    for (int i = 0; i < others.size(); i++) {
      if (i == index)
        continue;
      else if (isColliding(others.at(i)))
        continue;
    }
  }
  if (!isStatic)
    move(ftime);
}

glm::mat4 gameObject::formRotationMatrix(float frametime) {
  glm::vec3 dogMid = midpoint(mesh);
  glm::vec3 curPos = pos;

  glm::vec3 dest = curPos + vel * frametime;

  // vector in direction to look at
  glm::vec3 forward = glm::normalize(dest - curPos);
  glm::vec3 left = glm::normalize(glm::cross(glm::vec3(0, 1, 0), forward));
  glm::vec3 up = glm::cross(forward, left);
  // dog originally points forward, e.g. in +z direction
  // need to rotate dog to align with forward...
  glm::mat4 rot = glm::mat4(
      glm::vec4(left.x, left.y, left.z, 0), glm::vec4(0, 1, 0, 0),
      glm::vec4(forward.x, forward.y, forward.z, 0), glm::vec4(0, 0, 0, 1));

  return rot;
}

glm::vec3 gameObject::midpoint(std::shared_ptr<Shape> shape) {
  glm::vec3 midpoint;
  midpoint.x = (shape->min.x + shape->max.x) / 2;
  midpoint.y = (shape->min.y + shape->max.y) / 2;
  midpoint.z = (shape->min.z + shape->max.z) / 2;

  return midpoint;
}