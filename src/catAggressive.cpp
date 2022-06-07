#include "catAggressive.h"

catAggressive::catAggressive(std::shared_ptr<Shape> shape, camera *p, std::vector<std::shared_ptr<gameObject>> data) {
  collisionData = data;
  mesh = shape;
  player = p;
  // pos = glm::vec3(rand() % 25 - 12, 0, rand() % 25 - 12);
  pos = glm::vec3((rand() % 25) - 12, 0.35, (rand() % 25) - 12);
  // rot = glm::radians((float)(rand() % 361)); // y-axis
  // vel = vec3(0, 0, 0); // random x and y velocity
  float velocities[] = {0.05f, 0.025f, 0.0125f, -0.0125f, -0.025f, -0.05f};
  // vel = glm::vec3(velocities[rand() % 6], 0.0f, velocities[rand() % 6]);
  vel = glm::vec3(0.025f, 0, 0.025f);
  rot = tan(vel.z / vel.x);
  // vel = vec3(static_cast <float> (rand()) / static_cast <float> (1) *
  // 0.00000000075, 0, static_cast <float> (rand()) / static_cast <float> (1) *
  // 0.00000000075); // random x and y velocity
  glm::vec3 posDirection = glm::normalize(pos);
  glm::vec3 velDirection = glm::normalize(vel);
  float angle = acos(glm::dot(posDirection, velDirection));

  rad = .6;
  std::cout << "x: " << pos.x << " z: " << pos.z << std::endl;
}

bool catAggressive::isColliding(std::shared_ptr<gameObject> other) {
  if (destroying || other->getDestroying())
    return false;
  float d = glm::distance(pos, other->getPos());
  if (d > rad + other->getRad())
    return false;
  else if (d <= rad + other->getRad() && !other->getDestroying()) {
    return true;
  } else
    return false;
}


void catAggressive::move(double ftime) {
  glm::vec3 dir = glm::normalize(glm::vec3(player->getPos() - pos));

  glm::mat4 R = formRotationMatrix(ftime);

  glm::vec3 temp = pos;
  pos += dir * vel;

  for (int i = 0; collisionData.size() > i; i++)
  {
      if (isColliding(collisionData[i])) {
          pos = temp;
          break;
      }
  }

  glm::mat4 T = glm::translate(glm::mat4(1), pos);
  matrix = T * R;
}

glm::mat4 catAggressive::formRotationMatrix(float frametime) {
  glm::vec3 dogMid = midpoint(mesh);
  glm::vec3 curPos = pos;

  glm::vec3 dest = player->getPos();
  dest.y = curPos.y;

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