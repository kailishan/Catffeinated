#ifndef KIBBLE_H
#define KIBBLE_H

#include "gameObject.h"
#include "particleSys.h"
#include "Program.h"

class kibble : public gameObject {
public:
  kibble(std::shared_ptr<Shape> shape, std::vector<std::shared_ptr<gameObject>> data);

  std::shared_ptr<particleSys> getParticleSystem() { return particleSystem; }

  void move(double ftime) override;
  std::shared_ptr<particleSys> particleSystem;
private:
  std::vector<std::shared_ptr<gameObject>> collisionData;
};

#endif