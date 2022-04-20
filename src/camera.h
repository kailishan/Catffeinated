#pragma once

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "gameObject.h"
#include "WindowManager.h"

class camera
{
    public:
    camera()
    {
        window = nullptr;
        pos = glm::vec3(0, -1, 0);
        rot = glm::vec3(0, 0, 0);
    }

    camera(GLFWwindow* win)
    {
        window = win;
        pos = glm::vec3(0, -1, 0);
        rot = glm::vec3(0, 0, 0);
    }

    bool isColliding(gameObject other);
    glm::mat4 process(double ftime);

    bool isFirstMouse() { return firstMouse; }
    glm::vec3 getPos() { return pos; }
    int wkey() { return w; }
    int akey() { return a; }
    int skey() { return s; }
    int dkey() { return d; }

    void toggleFirstMouse() { firstMouse = true; }
    void setWindow(GLFWwindow* win) { window = win; }
    void setFront(glm::vec3 direction) { front = direction; }
    void setw(int newW) { w = newW; }
    void seta(int newA) { a = newA; }
    void sets(int newS) { s = newS; }
    void setd(int newD) { d = newD; }
    void setp(int newP) { p = newP; }

    private:
    glm::vec3 lookAtPoint = glm::vec3(0, -1, -6);
    glm::vec3 offset = glm::vec3(0, 0, 0);
    glm::vec3 eye = glm::vec3(0, -1, 0);
    glm::vec3 up = glm::vec3(0, 1, 0);
    glm::vec3 pos, rot, front;
    bool firstMouse = false;
    float dt = 0.0f;
    int w = 0;
    int a = 0;
    int s = 0;
    int d = 0;
    int p = 0;
    GLFWwindow* window;
    float rad = 0.8f;
    int score = 0;

};