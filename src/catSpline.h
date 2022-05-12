#pragma once

#include "gameObject.h"
#include "Spline.h"

class catSpline : public gameObject {
public:
  catSpline(std::shared_ptr<Shape> shape);
  void move(double ftime) override;

private:
  Spline splinepath[2];
};