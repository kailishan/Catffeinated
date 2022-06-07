#pragma once

#include "gameObject.h"

class cat : public gameObject {
public:
  cat(std::shared_ptr<Shape> shape, std::vector<std::shared_ptr<gameObject>> data);

  void move(double ftime) override;

private:
	std::vector<std::shared_ptr<gameObject>> collisionData;
};