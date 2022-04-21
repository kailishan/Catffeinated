/*
CPE/CSC 471 Lab base code Wood/Dunn/Eckhardt
*/
#define _USE_MATH_DEFINES

#include <glad/glad.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "GLSL.h"
#include "MatrixStack.h"
#include "Program.h"
#include "camera.h"
#include "gameManager.h"
#include "gameObject.h"
#include "math.h"
#include "stb_image.h"

#include "Shape.h"
#include "WindowManager.h"
// value_ptr for glm
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;
using namespace glm;

camera mycam;

double get_last_elapsed_time() {
  static double lasttime = glfwGetTime();
  double actualtime = glfwGetTime();
  double difference = actualtime - lasttime;
  lasttime = actualtime;
  return difference;
}

class Application : public EventCallbacks {

public:
  WindowManager *windowManager = nullptr;
  std::shared_ptr<gameManager> myManager;

  // shapes to draw
  shared_ptr<Shape> cat;


  // Our shader program
  std::shared_ptr<Program> prog, progL, heightshader;

  // Contains vertex information for OpenGL
  GLuint VertexArrayID;

  // Data necessary to give our box to OpenGL
  GLuint MeshPosID, MeshTexID, IndexBufferIDBox;

  // texture data
  GLuint Texture;
  GLuint Texture2, HeightTex;

  void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                   int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
      mycam.setw(1);
    }
    if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
      mycam.setw(0);
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
      mycam.sets(1);
    }
    if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
      mycam.sets(0);
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
      mycam.seta(1);
    }
    if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
      mycam.seta(0);
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
      mycam.setd(1);
    }
    if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
      mycam.setd(0);
    }
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
      mycam.setp(1);
    }
  }

  // callback for the mouse when clicked move the triangle when helper functions
  // written
  void mouseCallback(GLFWwindow *window, int button, int action, int mods) {
    double posX, posY;
    float newPt[2];
    if (action == GLFW_PRESS) {
      glfwGetCursorPos(window, &posX, &posY);
      std::cout << "Pos X " << posX << " Pos Y " << posY << std::endl;

      // change this to be the points converted to WORLD
      // THIS IS BROKEN< YOU GET TO FIX IT - yay!
      newPt[0] = 0;
      newPt[1] = 0;

      std::cout << "converted:" << newPt[0] << " " << newPt[1] << std::endl;
      glBindBuffer(GL_ARRAY_BUFFER, MeshPosID);
      // update the vertex array with the updated points
      glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * 6, sizeof(float) * 2,
                      newPt);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
  }

  void cursorCallback(GLFWwindow *window, double xpos, double ypos) {
    if (mycam.isFirstMouse()) {
      mycam.setLastX(xpos);
      mycam.setLastY(ypos);
      mycam.toggleFirstMouse();
    }

    float xoffset = xpos - mycam.getLastX();
    float yoffset = mycam.getLastY() - ypos;
    mycam.setLastX(xpos);
    mycam.setLastY(ypos);

    mycam.processCursor(xoffset, yoffset);
  }

  // if the window is resized, capture the new size and reset the viewport
  void resizeCallback(GLFWwindow *window, int in_width, int in_height) {
    // get the window size - may be different then pixels for retina
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
  }
#define MESHSIZE 25
  void init_mesh() {
    // generate the VAO
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // generate vertex buffer to hand off to OGL
    glGenBuffers(1, &MeshPosID);
    glBindBuffer(GL_ARRAY_BUFFER, MeshPosID);
    vec3 vertices[MESHSIZE * MESHSIZE * 4];
    for (int x = 0; x < MESHSIZE; x++)
      for (int z = 0; z < MESHSIZE; z++) {
        vertices[x * 4 + z * MESHSIZE * 4 + 0] =
            vec3(0.0, 0.0, 0.0) + vec3(x, 0, z);
        vertices[x * 4 + z * MESHSIZE * 4 + 1] =
            vec3(1.0, 0.0, 0.0) + vec3(x, 0, z);
        vertices[x * 4 + z * MESHSIZE * 4 + 2] =
            vec3(1.0, 0.0, 1.0) + vec3(x, 0, z);
        vertices[x * 4 + z * MESHSIZE * 4 + 3] =
            vec3(0.0, 0.0, 1.0) + vec3(x, 0, z);
      }
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * MESHSIZE * MESHSIZE * 4,
                 vertices, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    // tex coords
    float t = 1. / 100;
    vec2 tex[MESHSIZE * MESHSIZE * 4];
    for (int x = 0; x < MESHSIZE; x++)
      for (int y = 0; y < MESHSIZE; y++) {
        tex[x * 4 + y * MESHSIZE * 4 + 0] = vec2(0.0, 0.0) + vec2(x, y) * t;
        tex[x * 4 + y * MESHSIZE * 4 + 1] = vec2(t, 0.0) + vec2(x, y) * t;
        tex[x * 4 + y * MESHSIZE * 4 + 2] = vec2(t, t) + vec2(x, y) * t;
        tex[x * 4 + y * MESHSIZE * 4 + 3] = vec2(0.0, t) + vec2(x, y) * t;
      }
    glGenBuffers(1, &MeshTexID);
    // set the current state to focus on our vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, MeshTexID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * MESHSIZE * MESHSIZE * 4, tex,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glGenBuffers(1, &IndexBufferIDBox);
    // set the current state to focus on our vertex buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferIDBox);
    GLushort elements[MESHSIZE * MESHSIZE * 6];
    int ind = 0;
    for (int i = 0; i < MESHSIZE * MESHSIZE * 6; i += 6, ind += 4) {
      elements[i + 0] = ind + 0;
      elements[i + 1] = ind + 1;
      elements[i + 2] = ind + 2;
      elements[i + 3] = ind + 0;
      elements[i + 4] = ind + 2;
      elements[i + 5] = ind + 3;
    }
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(GLushort) * MESHSIZE * MESHSIZE * 6, elements,
                 GL_STATIC_DRAW);
    glBindVertexArray(0);
  }
  /*Note that any gl calls must always happen after a GL state is initialized */
  void initGeom() {
    // initialize the net mesh
    init_mesh();

    string resourceDirectory = "../resources";
    // Initialize cat.
    cat = make_shared<Shape>();
    // shape->loadMesh(resourceDirectory + "/t800.obj");
    cat->loadMesh(resourceDirectory + "/Cat_Low.obj");
    cat->resize();
    cat->init();

    // have to initialize after mesh or shape is nullptr
    myManager = make_shared<gameManager>(cat);

    int width, height, channels;
    char filepath[1000];

    // texture 1
    string str = resourceDirectory + "/grass.jpg";
    strcpy(filepath, str.c_str());
    unsigned char *data = stbi_load(filepath, &width, &height, &channels, 4);
    glGenTextures(1, &Texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    // texture 2
    str = resourceDirectory + "/sky.jpg";
    strcpy(filepath, str.c_str());
    data = stbi_load(filepath, &width, &height, &channels, 4);
    glGenTextures(1, &Texture2);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, Texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // texture 3
    str = resourceDirectory + "/height.png";
    strcpy(filepath, str.c_str());
    data = stbi_load(filepath, &width, &height, &channels, 4);
    glGenTextures(1, &HeightTex);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, HeightTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    //[TWOTEXTURES]
    // set the 2 textures to the correct samplers in the fragment shader:
    GLuint Tex1Location = glGetUniformLocation(
        prog->pid, "tex"); // tex, tex2... sampler in the fragment shader
    GLuint Tex2Location = glGetUniformLocation(prog->pid, "tex2");
    // Then bind the uniform samplers to texture units:
    glUseProgram(prog->pid);
    glUniform1i(Tex1Location, 0);
    glUniform1i(Tex2Location, 1);

    Tex1Location = glGetUniformLocation(
        heightshader->pid,
        "tex"); // tex, tex2... sampler in the fragment shader
    Tex2Location = glGetUniformLocation(heightshader->pid, "tex2");
    // Then bind the uniform samplers to texture units:
    glUseProgram(heightshader->pid);
    glUniform1i(Tex1Location, 0);
    glUniform1i(Tex2Location, 1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }

  // General OGL initialization - set OGL state here
  void init(const std::string &resourceDirectory) {

    GLSL::checkVersion();

    // Set background color.
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    // Enable z-buffer test.
    glEnable(GL_DEPTH_TEST);

    // Initialize the GLSL program.
    prog = std::make_shared<Program>();
    prog->setVerbose(true);
    prog->setShaderNames(resourceDirectory + "/shader_vertex.glsl",
                         resourceDirectory + "/shader_fragment.glsl");
    if (!prog->init()) {
      std::cerr << "One or more shaders failed to compile... exiting!"
                << std::endl;
      exit(1);
    }
    prog->addUniform("P");
    prog->addUniform("V");
    prog->addUniform("M");
    prog->addUniform("campos");
    prog->addAttribute("vertPos");
    prog->addAttribute("vertNor");
    prog->addAttribute("vertTex");

    progL = std::make_shared<Program>();
    progL->setVerbose(true);
    progL->setShaderNames(resourceDirectory + "/shader_vertex_light.glsl",
                          resourceDirectory + "/shader_fragment_light.glsl");
    if (!progL->init()) {
      std::cerr << "One or more shaders failed to compile... exiting!"
                << std::endl;
      exit(1);
    }
    progL->addUniform("P");
    progL->addUniform("V");
    progL->addUniform("M");
    progL->addUniform("campos");
    progL->addUniform("objColor");
    progL->addAttribute("vertPos");
    progL->addAttribute("vertColor");
    progL->addAttribute("vertNor");
    progL->addAttribute("vertTex");

    // Initialize the GLSL program.
    heightshader = std::make_shared<Program>();
    heightshader->setVerbose(true);
    heightshader->setShaderNames(resourceDirectory + "/height_vertex.glsl",
                                 resourceDirectory + "/height_frag.glsl");
    if (!heightshader->init()) {
      std::cerr << "One or more shaders failed to compile... exiting!"
                << std::endl;
      exit(1);
    }
    heightshader->addUniform("P");
    heightshader->addUniform("V");
    heightshader->addUniform("M");
    heightshader->addUniform("camoff");
    heightshader->addUniform("campos");
    heightshader->addAttribute("vertPos");
    heightshader->addAttribute("vertTex");
  }

  /****DRAW
  This is the most important function in your program - this is where you
  will actually issue the commands to draw any geometry you have set up to
  draw
  ********/
  void render() {
    double frametime = get_last_elapsed_time();
    myManager->process(mycam, frametime);

    // Get current frame buffer size.
    int width, height;
    glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
    float aspect = width / (float)height;
    glViewport(0, 0, width, height);

    // Clear framebuffer.
    glClearColor(0.8f, 0.8f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Create the matrix stacks - please leave these alone for now

    glm::mat4 V, M, P; // View, Model and Perspective matrix
    V = glm::mat4(1);
    M = glm::mat4(1);
    // Apply orthographic projection....
    P = glm::ortho(-1 * aspect, 1 * aspect, -1.0f, 1.0f, -2.0f, 100.0f);
    if (width < height) {
      P = glm::ortho(-1.0f, 1.0f, -1.0f / aspect, 1.0f / aspect, -2.0f, 100.0f);
    }
    // ...but we overwrite it (optional) with a perspective projection.
    P = glm::perspective(
        (float)(3.14159 / 4.), (float)((float)width / (float)height), 0.1f,
        1000.0f); // so much type casting... GLM metods are quite funny ones

    // animation with the model matrix:
    static float w = 0.0;
    w += 1.0 * frametime; // rotation angle
    float trans = 0;      // sin(t) * 2;
    glm::mat4 RotateY =
        glm::rotate(glm::mat4(1.0f), w, glm::vec3(0.0f, 1.0f, 0.0f));
    float angle = -3.1415926 / 2.0;
    glm::mat4 RotateX =
        glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 TransZ =
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3 + trans));

    glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 0.3f, 0.3f));
    glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    // M =  TransZ * RotateY * RotateX * S;
    M = S * T;

    // Draw the box using GLSL.
    progL->bind();

    // TODO set to glm lookAt
    V = glm::lookAt(mycam.getPos(), mycam.getPos() + mycam.getFront(),
                    mycam.getUp());
    mycam.processKeyboard(frametime);
    // V = mycam.process(frametime);
    // send the matrices to the shaders
    glUniformMatrix4fv(progL->getUniform("P"), 1, GL_FALSE, &P[0][0]);
    glUniformMatrix4fv(progL->getUniform("V"), 1, GL_FALSE, &V[0][0]);
    glUniformMatrix4fv(progL->getUniform("M"), 1, GL_FALSE, &M[0][0]);
    glUniform3fv(progL->getUniform("campos"), 1, &mycam.getPos()[0]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture);

    glm::vec4 pink = glm::vec4(1.0, 0.357, 0.796, 1);
    glUniform4fv(progL->getUniform("objColor"), 1, &pink[0]);

    for (int i = 0; i < myManager->getObjects().size(); i++) {
      gameObject currObj = myManager->getObjects().at(i);
      vec3 currPos = currObj.getPos();
      S = glm::scale(glm::mat4(1.0f), glm::vec3(currObj.getRad()));
      T = glm::translate(glm::mat4(1.0f), currPos);
      glm::mat4 R =
          glm::rotate(glm::mat4(1), currObj.getRot(), glm::vec3(0, 1, 0));
      M = myManager->getObjects().at(i).getMatrix() * S;
      glUniformMatrix4fv(progL->getUniform("M"), 1, GL_FALSE, &M[0][0]);
      // draw object's mesh; this helps generalize
      currObj.getMesh()->draw(progL, false);
    }
    // shape->draw(prog,FALSE);

    heightshader->bind();
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glm::mat4 TransY = glm::translate(glm::mat4(1.0f), glm::vec3(-50.0f,
    // -3.0f, -50)); M = TransY;
    M = glm::translate(glm::mat4(1.0f), glm::vec3(-12.5f, 0.0f, -12.5f));
    glUniformMatrix4fv(heightshader->getUniform("M"), 1, GL_FALSE, &M[0][0]);
    glUniformMatrix4fv(heightshader->getUniform("P"), 1, GL_FALSE, &P[0][0]);
    glUniformMatrix4fv(heightshader->getUniform("V"), 1, GL_FALSE, &V[0][0]);

    vec3 offset = mycam.getPos();
    offset.y = 0;
    offset.x = 0; // (int)offset.x;
    offset.z = 0; // (int)offset.z;
    glUniform3fv(heightshader->getUniform("camoff"), 1, &offset[0]);
    glUniform3fv(heightshader->getUniform("campos"), 1, &mycam.getPos()[0]);
    glBindVertexArray(VertexArrayID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferIDBox);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, HeightTex);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, Texture);
    glDrawElements(GL_TRIANGLES, MESHSIZE * MESHSIZE * 6, GL_UNSIGNED_SHORT,
                   (void *)0);

    heightshader->unbind();
  }
};
//******************************************************************************************
int main(int argc, char **argv) {
  std::string resourceDir =
      "../resources"; // Where the resources are loaded from
  if (argc >= 2) {
    resourceDir = argv[1];
  }

  Application *application = new Application();

  /* your main will always include a similar set up to establish your window
          and GL context, etc. */
  WindowManager *windowManager = new WindowManager();
  windowManager->init(1920, 1080);
  windowManager->setEventCallbacks(application);

  // allows cursor to remain within window while also hiding it
  glfwSetInputMode(windowManager->getHandle(), GLFW_CURSOR,
                   GLFW_CURSOR_DISABLED);

  application->windowManager = windowManager;
  mycam.setWindow(windowManager->getHandle());

  /* This is the code that will likely change program to program as you
          may need to initialize or set up different data and state */
  // Initialize scene.
  application->init(resourceDir);
  application->initGeom();

  // Loop until the user closes the window.
  while (!glfwWindowShouldClose(windowManager->getHandle())) {
    // Render scene.
    application->render();

    // Swap front and back buffers.
    glfwSwapBuffers(windowManager->getHandle());
    // Poll for and process events.
    glfwPollEvents();
  }

  // Quit program.
  windowManager->shutdown();
  return 0;
}
