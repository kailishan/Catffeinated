#pragma once

#include "gameObject.h"

class roomObject : public gameObject {
public:
    roomObject(glm::vec3 position, float radius) {
        pos = position;
        rad = radius;
        vel = glm::vec3(0);
        rot = 0.0;
        objectType = 2;
     
    }

    /* a room object doesn't move */
    void move(double ftime) override{};
};