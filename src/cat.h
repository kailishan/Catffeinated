#pragma once

#include "gameObject.h"

class cat : public gameObject {
public:
  cat(std::shared_ptr<Shape> shape);

  void move(double ftime) override;
};