#include "camera.h"

float distance(float x1, float y1,
               float z1, float x2,
               float y2, float z2)
{
    float d = sqrt(pow(x2 - x1, 2) +
                   pow(y2 - y1, 2) +
                   pow(z2 - z1, 2) * 1.0);
    return d;
}


bool camera::isColliding(gameObject other)
{
    float d = distance(-pos.x, -pos.y, -pos.z, other.getPos().x, other.getPos().y, other.getPos().z);
    if (d > rad + other.getRad())
        return false;
    else if (d <= rad + other.getRad() && !other.getDestroying())
    {
        score++;
        std::cout << "CATS BOOPED: " << score << std::endl;
        //cout << "(CAM) " << "x: " << pos.x << " z: " << pos.z << endl;
        other.setDestroying(true);
        return true;
    }
    else
        return false;
}

glm::mat4 camera::process(double ftime)
{
    if (p == 1)
    {
        //cout << "(CAM) " << "x: " << pos.x << " z: " << pos.z << endl;
        p = 0;
    }
    float speed = 0;
    double xpos, ypos;

    if (w == 1 || a == 1)
    {
        speed = 6 * ftime;
    }
    else if (s == 1 || d == 1)
    {
        speed = -6 * ftime;
    }
    float yangle = 0;

    glfwGetCursorPos(window, &xpos, &ypos);
    xpos = xpos * 0.005;
    ypos = ypos * 0.005;

    glm::mat4 R = glm::rotate(glm::mat4(1), (float)xpos, glm::vec3(0, 1, 0));
    glm::mat4 R2 = glm::rotate(glm::mat4(1), (float)ypos, glm::vec3(1, 0, 0));

    glm::vec4 dir = glm::vec4(0, 0, 0, 1);

    if (w == 1 || s == 1)
        dir = glm::vec4(0, 0, speed, 1);
    else if (a == 1 || d == 1)
        dir = glm::vec4(speed, 0, 0, 1);

    dir = dir * R;

    if (pos.x + dir.x > 12.5 || pos.x + dir.x < -12.5 || pos.z + dir.z > 12.5 || pos.z + dir.z < -12.5)
        pos = pos;
    else
        pos += glm::vec3(dir.x, dir.y, dir.z);

    //cout << "x: " << pos.x << " z: " << pos.z << endl;
    glm::mat4 T = glm::translate(glm::mat4(1), pos);

    return R2 * R * T;
}