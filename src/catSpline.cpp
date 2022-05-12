#include "catSpline.h"

catSpline::catSpline(std::shared_ptr<Shape> shape) {
  mesh = shape;
  pos = glm::vec3(1, 0.35, 1);
  float velocities[] = {0.05f, 0.025f, 0.0125f, -0.0125f, -0.025f, -0.05f};
  vel = glm::vec3(velocities[rand() % 6], 0.0f, velocities[rand() % 6]);
  rot = tan(vel.z / vel.x);
  glm::vec3 posDirection = glm::normalize(pos);
  glm::vec3 velDirection = glm::normalize(vel);
  float angle = acos(glm::dot(posDirection, velDirection));

  rad = .5;
  std::cout << "x: " << pos.x << " z: " << pos.z << std::endl;

  splinepath[0] = Spline(pos, glm::vec3(pos.x + 5, pos.y, pos.z + 5),
                         glm::vec3(pos.x - 5, pos.y, pos.z - 5),
                         pos, 8);
  splinepath[1] = Spline(pos,
                         glm::vec3(pos.x + 5, pos.y, pos.z - 5),
                         glm::vec3(pos.x - 1, pos.y, pos.z + 1), pos, 8);
}

void catSpline::move(double ftime) {
    if (!splinepath[0].isDone()) {
      splinepath[0].update(ftime);
      pos = splinepath[0].getPosition();
    } else {
      splinepath[1].update(ftime);
      pos = splinepath[1].getPosition();
    }

    if (splinepath[1].isDone()) {
      splinepath[0] = Spline(pos, glm::vec3(pos.x + 5, pos.y, pos.z + 5),
                             glm::vec3(pos.x - 5, pos.y, pos.z - 5), pos, 8);
      splinepath[1] = Spline(pos, glm::vec3(pos.x + 5, pos.y, pos.z - 5),
                             glm::vec3(pos.x - 1, pos.y, pos.z + 1), pos, 8);
    }


    glm::mat4 T = glm::translate(glm::mat4(1), pos);
    matrix = T;
};
