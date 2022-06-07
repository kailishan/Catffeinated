#include "catSpline.h"

catSpline::catSpline(std::shared_ptr<Shape> shape, std::vector<std::shared_ptr<gameObject>> data) {
  collisionData = data;
  mesh = shape;
  pos = glm::vec3(1, 0.35, 1);
  float velocities[] = {0.05f, 0.025f, 0.0125f, -0.0125f, -0.025f, -0.05f};
  vel = glm::vec3(velocities[rand() % 6], 0.0f, velocities[rand() % 6]);
  rot = tan(vel.z / vel.x);
  glm::vec3 posDirection = glm::normalize(pos);
  glm::vec3 velDirection = glm::normalize(vel);
  float angle = acos(glm::dot(posDirection, velDirection));

  rad = .6;
  std::cout << "x: " << pos.x << " z: " << pos.z << std::endl;

  splinepath[0] = Spline(pos, glm::vec3(-3, 0.35, -3),
                         glm::vec3(3, 0.35, -3), pos, 5);
  splinepath[1] = Spline(glm::vec3(2, 0.5, 5), glm::vec3(3, 0.5, 5),
                         glm::vec3(-0.25, 0.5, 5), glm::vec3(0, 0.5, 5), 5);
}

void catSpline::move(double ftime) {
    glm::vec3 oldPos = pos;

    if (!splinepath[0].isDone()) {
      splinepath[0].update(ftime);
      vel = splinepath[0].getPosition() - pos;
      //pos = splinepath[0].getPosition();
    }

    glm::vec4 dir = glm::vec4(vel, 1);


    if (splinepath[0].isDone()) {
      splinepath[0] =
          Spline(pos, glm::vec3(-3, 0.35, -3), glm::vec3(3, 0.35, -3), pos, 5);
      splinepath[1] = Spline(glm::vec3(2, 0.5, 5), glm::vec3(3, 0.5, 5),
                             glm::vec3(-0.25, 0.5, 5), glm::vec3(0, 0.5, 5), 5);
    }

    glm::vec3 temp = pos;

    pos += glm::vec3(dir.x, dir.y, dir.z);

    for (int i = 0; collisionData.size() > i; i++)
    {
        if (isColliding(collisionData[i])) {
            pos = temp;
            break;
        }
    }


    glm::mat4 R = formRotationMatrix(ftime, pos);

    glm::mat4 T = glm::translate(glm::mat4(1), pos);
    matrix = T * R;
};

glm::mat4 catSpline::formRotationMatrix(float frametime, glm::vec3 oldPos) {
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
