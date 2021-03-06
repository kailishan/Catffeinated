#include "cat.h"

cat::cat(std::shared_ptr<Shape> shape, std::vector<std::shared_ptr<gameObject>> data) {
  collisionData = data;
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


void cat::move(double ftime) {
  rot = atan(vel.z / vel.x);
  if (vel.z < 0)
    rot += glm::radians(180.f);

  glm::mat4 R = glm::rotate(glm::mat4(1), rot, glm::vec3(0.0f, 1.0f, 0.0f));
  R = formRotationMatrix(ftime);
  glm::vec4 dir = glm::vec4(vel, 1);

  glm::vec3 temp = pos;

  

  if (pos.x + dir.x > 12 || pos.x + dir.x < -12) {
    pos = pos;
    vel.x = -vel.x;
  } else if (pos.z + dir.z > 12 || pos.z + dir.z < -12) {
    pos = pos;
    vel.z = -vel.z;
  } else
    pos += glm::vec3(dir.x, dir.y, dir.z);

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