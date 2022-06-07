/*
CPE/CSC 471 Lab base code Wood/Dunn/Eckhardt
*/
#define _USE_MATH_DEFINES

#include <glad/glad.h>
#include <iostream>
#include "GLSL.h"
#include "MatrixStack.h"
#include "Program.h"
#include "audioManager.h"
#include "camera.h"
#include "gameManager.h"
#include "gameObject.h"
#include "roomObject.h"
#include "math.h"
#include "stb_image.h"

#include "Shape.h"
#include "WindowManager.h"
#include "GLTextureWriter.h"
// value_ptr for glm
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
using namespace std;
using namespace glm;

camera mycam;

vector<shared_ptr<gameObject>> objects;
vector<shared_ptr<vec3>> lights;
//glm::vec3 lights[7];
double tail_dt = 0.0f;


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

  // Shapes to draw
  shared_ptr<Shape> cat, sphere, room, table, heart;

  // Our shader program
  std::shared_ptr<Program> prog, progL, heightshader, TVstatic, texProg;

  // Contains vertex information for OpenGL
  GLuint VertexArrayID;

  // Data necessary to give our triangle to OpenGL
  GLuint VertexBufferID;

  // Geometry for texture render
  GLuint quad_VertexArrayID;
  GLuint quad_vertexbuffer;

  // Reference to texture FBO
  GLuint gBuffer;
  GLuint gPosition, gNormal, gColorSpec;

  // Deferred shading data
  bool FirstTime = true;
  bool DEFER = false;
  int gMat = 0;
  vec3 g_light = vec3(2, 6, 6);

  // Data necessary to give our box to OpenGL
  GLuint MeshPosID, MeshTexID, IndexBufferIDBox;

  // Data for TVstatic
  GLuint FBOtex, fb, depth_fb;
  GLuint VertexArrayIDScreen, VertexBufferIDScreen, VertexBufferTexScreen;

  // texture data
  GLuint Texture;
  GLuint Texture2, HeightTex;
  GLuint CatTex1, CatTex2, CatTex3;

  // Camera data
  vec3 g_eye = vec3(0, 1, 0);
  vec3 g_lookAt = vec3(0, 1, -4);


  void initRoomGeo()
  {
    shared_ptr<gameObject> ro1 =
        make_shared<roomObject>(vec3(5.75, 1, .75), .8);
    shared_ptr<gameObject> ro2 =
        make_shared<roomObject>(vec3(5.75, 1, 3.25), .8);
    shared_ptr<gameObject> ro3 =
        make_shared<roomObject>(vec3(5.75, 1, 5.75), .8);
    shared_ptr<gameObject> ro4 =
        make_shared<roomObject>(vec3(5.75, 1, 8.25), .8);
    shared_ptr<gameObject> ro5 =
        make_shared<roomObject>(vec3(5.75, 1, 10.75), .8);
    objects.push_back(ro1);
    objects.push_back(ro2);
    objects.push_back(ro3);
    objects.push_back(ro4);
    objects.push_back(ro5);

    shared_ptr<gameObject> ro6 =
        make_shared<roomObject>(vec3(10.75, 1, -1.75), .8);
    shared_ptr<gameObject> ro7 =
        make_shared<roomObject>(vec3(10.75, 1, .75), .8);
    shared_ptr<gameObject> ro8 =
        make_shared<roomObject>(vec3(10.75, 1, 3.25), .8);
    shared_ptr<gameObject> ro9 =
        make_shared<roomObject>(vec3(10.75, 1, 5.25), .8);
    shared_ptr<gameObject> ro10 =
        make_shared<roomObject>(vec3(10.75, 1, 8.25), .8);
    shared_ptr<gameObject> ro11 =
        make_shared<roomObject>(vec3(10.75, 1, 10.75), .8);
    objects.push_back(ro6);
    objects.push_back(ro7);
    objects.push_back(ro8);
    objects.push_back(ro9);
    objects.push_back(ro10);
    objects.push_back(ro11);

    int x = -3.5;
    int z = 5.75;
    shared_ptr<gameObject> ro12 =
        make_shared<roomObject>(vec3(x = x - 2, 1, z), .8);
    shared_ptr<gameObject> ro13 =
        make_shared<roomObject>(vec3(x = x - 2, 1, z), .8);
    shared_ptr<gameObject> ro14 =
        make_shared<roomObject>(vec3(x = x - 2, 1, z), .8);
    shared_ptr<gameObject> ro15 =
        make_shared<roomObject>(vec3(x = x - 2, 1, z), .8);
    shared_ptr<gameObject> ro16 =
        make_shared<roomObject>(vec3(x, 1, z = z + 2), .8);
    objects.push_back(ro12);
    objects.push_back(ro13);
    objects.push_back(ro14);
    objects.push_back(ro15);
    objects.push_back(ro16);

    x = -8.9;
    z = 12.5;
    shared_ptr<gameObject> ro17 =
        make_shared<roomObject>(vec3(x, 1, z), .8);
    shared_ptr<gameObject> ro18 = 
        make_shared<roomObject>(vec3(x + .75, 1, z), .8);
    objects.push_back(ro17);
    objects.push_back(ro18);

    //cout << "HIT" << endl;
    //cout << objects.size() << endl;

    //vec3 l1 = vec3(10, 5, 10);
    shared_ptr<vec3> l1 = make_shared<vec3>(vec3(9.0, 7.0, 9.0));
    lights.push_back(l1);
    shared_ptr<vec3> l2 = make_shared<vec3>(vec3(9.0, 7.0, 9.0));
    lights.push_back(l2);
    shared_ptr<vec3> l3 = make_shared<vec3>(vec3(9.0, 7.0, 9.0));
    lights.push_back(l3);
    shared_ptr<vec3> l4 = make_shared<vec3>(vec3(9.0, 7.0, 9.0));
    lights.push_back(l4);
    shared_ptr<vec3> l5 = make_shared<vec3>(vec3(9.0, 7.0, 9.0));
    lights.push_back(l5);
    shared_ptr<vec3> l6 = make_shared<vec3>(vec3(9.0, 7.0, 9.0));
    lights.push_back(l6);
    shared_ptr<vec3> l7 = make_shared<vec3>(vec3(9.0, 7.0, 9.0));
    lights.push_back(l7);
  }

  void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                   int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, GL_TRUE);
    }

    // MOVEMENT
    if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) {
        cout << "x: " << mycam.getPos().x << " z: " << mycam.getPos().z << endl;
    }


    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
      mycam.setw(1);
    }
    if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
      mycam.setw(0);
      mycam.newKey = true;
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
      mycam.sets(1);
    }
    if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
      mycam.sets(0);
      mycam.newKey = true;
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
      mycam.seta(1);
    }
    if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
      mycam.seta(0);
      mycam.newKey = true;
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
      mycam.setd(1);
    }
    if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
      mycam.setd(0);
      mycam.newKey = true;
    }

    // POLYGON MODE
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
      mycam.setp(1);
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (key == GLFW_KEY_P && action == GLFW_RELEASE) {
        mycam.setp(0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // ZOE MODE
    if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
        mycam.setz(1);
    }
    if (key == GLFW_KEY_Z && action == GLFW_RELEASE) {
        mycam.setz(0);
    }

    // CAMERA POSITION XZ
    if (key == GLFW_KEY_L && action == GLFW_PRESS) {
        ;
    }
    if (key == GLFW_KEY_L && action == GLFW_RELEASE) {
        cout << mycam.getPos().x << " " << mycam.getPos().z << endl;
    }

    // DEFERRED SHADING
    if (key == GLFW_KEY_O && action == GLFW_PRESS)
    {
      DEFER = !DEFER;
    }
  }

  // callback for the mouse when clicked
  void mouseCallback(GLFWwindow *window, int button, int action, int mods) {
    double posX, posY;
    float newPt[2];
    if (action == GLFW_PRESS) {
      glfwGetCursorPos(window, &posX, &posY);
      std::cout << "Pos X " << posX << " Pos Y " << posY << std::endl;
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

  /* Note that any gl calls must always happen after a GL state is initialized */
  void initGeom() {

    //screen plane
    glGenVertexArrays(1, &VertexArrayIDScreen);
    glBindVertexArray(VertexArrayIDScreen);
    //generate vertex buffer to hand off to OGL
    glGenBuffers(1, &VertexBufferIDScreen);
    //set the current state to focus on our vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferIDScreen);
    vec3 vertices[6];
    vertices[0] = vec3(-1, -1, 0);
    vertices[1] = vec3(1, -1, 0);
    vertices[2] = vec3(1, 1, 0);
    vertices[3] = vec3(-1, -1, 0);
    vertices[4] = vec3(1, 1, 0);
    vertices[5] = vec3(-1, 1, 0);
    //actually memcopy the data - only do this once
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(vec3), vertices, GL_STATIC_DRAW);
    //we need to set up the vertex array
    glEnableVertexAttribArray(0);
    //key function to get up how many elements to pull out at a time (3)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    //generate vertex buffer to hand off to OGL
    glGenBuffers(1, &VertexBufferTexScreen);
    //set the current state to focus on our vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferTexScreen);
    vec2 texscreen[6];
    texscreen[0] = vec2(0, 0);
    texscreen[1] = vec2(1, 0);
    texscreen[2] = vec2(1, 1);
    texscreen[3] = vec2(0, 0);
    texscreen[4] = vec2(1, 1);
    texscreen[5] = vec2(0, 1);
    //actually memcopy the data - only do this once
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(vec2), texscreen, GL_STATIC_DRAW);
    //we need to set up the vertex array
    glEnableVertexAttribArray(1);
    //key function to get up how many elements to pull out at a time (3)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindVertexArray(0);



    string resourceDirectory = "../resources";
    
    // Initialize geometry

    cat = make_shared<Shape>();
    cat->loadMesh(resourceDirectory + "/catsplit.obj");
    cat->resize();
    cat->init();

    sphere = make_shared<Shape>();
    sphere->loadMesh(resourceDirectory + "/sphere.obj");
    sphere->resize();
    sphere->init();

    room = make_shared<Shape>();
    string mtlPath = resourceDirectory + "/acroom/";
    room->loadMesh(resourceDirectory + "/acroom/acroom.obj", &mtlPath);
    room->resize();
    room->init();

    table = make_shared<Shape>();
    table->loadMesh(resourceDirectory + "/Table_Carre.obj");
    table->resize();
    table->init();

    heart = make_shared<Shape>();
    heart->loadMesh(resourceDirectory + "/heart.obj");
    heart->resize();
    heart->init();

    // Initialize texture

    int width, height, channels;
    char filepath[1000];

    string str;
    unsigned char *data;

    // texture 1
    str = resourceDirectory + "/tiger.jpg";
    strcpy(filepath, str.c_str());
    data = stbi_load(filepath, &width, &height, &channels, 4);
    glGenTextures(1, &CatTex1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, CatTex1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // texture 2
    str = resourceDirectory + "/calico.jpg";
    strcpy(filepath, str.c_str());
    data = stbi_load(filepath, &width, &height, &channels, 4);
    glGenTextures(1, &CatTex2);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, CatTex2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // texture 3
    str = resourceDirectory + "/rainbow.png";
    strcpy(filepath, str.c_str());
    data = stbi_load(filepath, &width, &height, &channels, 4);
    glGenTextures(1, &CatTex3);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, CatTex3);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //[TWOTEXTURES]
    // set the 2 textures to the correct samplers in the fragment shader:
    GLuint Tex1Location = glGetUniformLocation(
        prog->pid, "tex"); // tex, tex2... sampler in the fragment shader
    // Then bind the uniform samplers to texture units:
    glUseProgram(prog->pid);
    glUniform1i(Tex1Location, 0);

    Tex1Location = glGetUniformLocation(
        heightshader->pid,
        "tex"); // tex, tex2... sampler in the fragment shader
    GLuint Tex2Location = glGetUniformLocation(heightshader->pid, "tex2");
    // Then bind the uniform samplers to texture units:
    glUseProgram(heightshader->pid);
    glUniform1i(Tex1Location, 0);
    glUniform1i(Tex2Location, 1);

    // tex, tex2... sampler in the fragment shader
    /*Tex1Location = glGetUniformLocation(heightshader->pid, "tex");
    glUseProgram(progL->pid);
    glUniform1i(Tex1Location, 0);*/

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Tex1Location = glGetUniformLocation(TVstatic->pid, "tex");//tex, tex2... sampler in the fragment shader
    glUseProgram(TVstatic->pid);
    glUniform1i(Tex1Location, 0);

    glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
    //RGBA8 2D texture, 24 bit depth texture, 256x256
    glGenTextures(1, &FBOtex);
    glBindTexture(GL_TEXTURE_2D, FBOtex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //NULL means reserve texture memory, but texels are undefined
    //**** Tell OpenGL to reserve level 0
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
    //You must reserve memory for other mipmaps levels as well either by making a series of calls to
    //glTexImage2D or use glGenerateMipmapEXT(GL_TEXTURE_2D).
    //Here, we'll use :
    glGenerateMipmap(GL_TEXTURE_2D);
    //make a frame buffer
    //-------------------------
    glGenFramebuffers(1, &fb);
    glBindFramebuffer(GL_FRAMEBUFFER, fb);
    //Attach 2D texture to this FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBOtex, 0);
    //-------------------------
    glGenRenderbuffers(1, &depth_fb);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_fb);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
    //-------------------------
    //Attach depth buffer to FBO
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_fb);
    //-------------------------
    //Does the GPU support current FBO configuration?
    GLenum status;
    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch (status)
    {
    case GL_FRAMEBUFFER_COMPLETE:
        cout << "status framebuffer: good";
        break;
    default:
        cout << "status framebuffer: bad!!!!!!!!!!!!!!!!!!!!!!!!!";
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  // General OGL initialization - set OGL state here
  void init(const std::string &resourceDirectory) {

    GLSL::checkVersion();

    // Set background color.
    glClearColor(0.098f, 0.098f, 0.439f, 1.0f);
    // Enable z-buffer test.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    // Initialize the GLSL program.
    prog = std::make_shared<Program>();
    prog->setVerbose(true);
    prog->setShaderNames(resourceDirectory + "/toon_vertex.glsl",
                         resourceDirectory + "/toon_fragment.glsl");
    /*prog->setShaderNames(resourceDirectory + "/simple_vert.glsl",
                         resourceDirectory + "/gbuf_frag.glsl");*/
    if (!prog->init()) {
      std::cerr << "One or more shaders failed to compile... exiting!"
                << std::endl;
      exit(1);
    }
    prog->addUniform("P");
    prog->addUniform("V");
    prog->addUniform("M");
    prog->addUniform("objColor");
    prog->addUniform("campos");
    prog->addUniform("lightRad");
    prog->addUniform("lightDir");
    prog->addUniform("lightPos1");
    prog->addUniform("lightPos2");
    prog->addUniform("lightPos3");
    prog->addUniform("lightPos4");
    prog->addUniform("lightPos5");
    prog->addUniform("lightPos6");
    prog->addUniform("lightPos7");
    prog->addUniform("lightPos8");
    prog->addAttribute("vertPos");
    prog->addAttribute("vertNor");
    prog->addAttribute("vertTex");

    /* DEFERRED SHADING */
    texProg = make_shared<Program>();
    texProg->setVerbose(true);
    texProg->setShaderNames(
        resourceDirectory + "/pass_vert.glsl",
        resourceDirectory + "/tex_frag.glsl");
    if (!texProg->init()) {
      std::cerr << "One or more shaders failed to compile... exiting!"
                << std::endl;
      exit(1);
    }
    //texProg->addUniform("texBuf");
    texProg->addUniform("gBuf");
    texProg->addUniform("norBuf");
    texProg->addUniform("colorBuf");
    texProg->addAttribute("vertPos");
    texProg->addUniform("Ldir");
    
    initBuffers();
    initQuad();

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

    // TV Static post processing effect
    TVstatic = std::make_shared<Program>();
    TVstatic->setVerbose(true);
    TVstatic->setShaderNames(resourceDirectory + "/TV_vertex.glsl", resourceDirectory + "/TV_fragment.glsl");
    if (!TVstatic->init())
    {
        std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
        exit(1);
    }
    TVstatic->addUniform("u_distortion");
    TVstatic->addUniform("u_stripe");
    TVstatic->addUniform("u_rgbshift");
    TVstatic->addAttribute("vertPos");
    TVstatic->addAttribute("vertTex");

    /*
    cout << prog->pid << endl;
    cout << progL->pid << endl;
    cout << heightshader->pid << endl;*/
  }

  /**** geometry set up for a quad *****/
	void initQuad() {
		//now set up a simple quad for rendering FBO
		glGenVertexArrays(1, &quad_VertexArrayID);
		glBindVertexArray(quad_VertexArrayID);

		static const GLfloat g_quad_vertex_buffer_data[] =
		{
			-1.0f, -1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			1.0f,  1.0f, 0.0f,
		};

		glGenBuffers(1, &quad_vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
	}

  void initBuffers()
  {
    int width, height;
    glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);

    // initialize the buffers -- from learnopengl.com
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    // - position color buffer
    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

    // - normal color buffer
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

    // - color + specular color buffer
    glGenTextures(1, &gColorSpec);
    glBindTexture(GL_TEXTURE_2D, gColorSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gColorSpec, 0);

    // more FBO set up
    GLenum DrawBuffers[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, DrawBuffers);
  }

  void initGame() { 

    cout << objects.size() << endl;
    myManager = make_shared<gameManager>(cat, heart, &mycam, objects);
    //mycam.setManager(myManager.get());
  }

  mat4 GetOrthoMatrix() {
      float wS = 2.5;
      mat4 ortho = glm::ortho(-15.0f * wS, 15.0f * wS, -15.0f * wS, 15.0f * wS, 2.1f, 100.f);
      return ortho;
  }

  /* camera controls - this is the camera for the top down view */
  mat4 GetTopView() {
      vec3 pos = vec3(mycam.getPos().x, 0, mycam.getPos().z);
      mat4 Cam = lookAt(g_eye + vec3(0, 6, 0) + pos, g_eye + pos, g_lookAt - g_eye);
      return Cam;
  }

  mat4 SetOrthoMatrix(shared_ptr<Program> curShade) {
      mat4 ortho = mat4(1.0);
      ortho = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);

      glUniformMatrix4fv(curShade->getUniform("LP"), 1, GL_FALSE, value_ptr(ortho));
      return ortho;
  }

  mat4 SetLightView(shared_ptr<Program> curShade, vec3 pos, vec3 LA, vec3 up) {
      //mat4 Cam = mat4(1.0);
      mat4 Cam = glm::lookAt(pos, LA, up);
      glUniformMatrix4fv(curShade->getUniform("LV"), 1, GL_FALSE, value_ptr(Cam));
      return Cam;
  }

  void drawHealth(mat4 P, mat4 V) {
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glBindTexture(GL_TEXTURE_2D, 0);

    prog->bind();
    
    glm::mat4 M = glm::mat4(1.0f);
    glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(0.35f, 0.5f, 0.35f));
    glm::mat4 R = glm::rotate(glm::mat4(1.0f), -1.5708f, vec3(1, 0, 0));
    glm::mat4 T = glm::mat4(1.0f);

    glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P));
    glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(V));
    glUniform3fv(prog->getUniform("campos"), 1, &mycam.getPos()[0]);

    glm::vec4 red = glm::vec4(1, 0.302, 0.302, 1);
    glm::vec4 green = glm::vec4(0.302, 1, 0.302, 1);
    if (mycam.getSpeedBoost() > 0.0f)
      glUniform4fv(prog->getUniform("objColor"), 1, &green[0]);
    else
      glUniform4fv(prog->getUniform("objColor"), 1, &red[0]);

    glm::vec3 pos = mycam.getPos();
    pos.y += 2.5f;
    float offset = 1.75;

    for (int i = 0; i < mycam.getHealth(); i++) {
      T = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x + offset, pos.y, pos.z));
      M = T * R * S;
      glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
      heart->draw(prog, true); 
      offset -= 0.8;
    }

    prog->unbind();

    //glDisable(GL_DEPTH_TEST);
  }

  /**** DRAW
  This is the most important function in your program - this is where you
  will actually issue the commands to draw any geometry you have set up to
  draw
  ********/
  void drawScene(mat4 P, mat4 V) {

      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LESS);

      prog->bind();

      /*glEnableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, gPosition);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

      glEnableVertexAttribArray(1);
      glBindBuffer(GL_ARRAY_BUFFER, gNormal);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

      glEnableVertexAttribArray(2);
      glBindBuffer(GL_ARRAY_BUFFER, gColorSpec);
      glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);*/

      double frametime = get_last_elapsed_time();
      tail_dt += frametime;

      vec3 camPos = mycam.getPos() - mycam.getFront() * vec3(1.5);

      vec3 lightDir = vec3(-8.1, 7.0, 8.7);
      glUniform3fv(prog->getUniform("lightPos1"), 1, &lightDir[0]);
      lightDir = vec3(-7.9, 7.0, 1.5);
      glUniform3fv(prog->getUniform("lightPos2"), 1, &lightDir[0]);
      lightDir = vec3(-10.7, 7.0, -8.3);
      glUniform3fv(prog->getUniform("lightPos3"), 1, &lightDir[0]);
      lightDir = vec3(-4, 7.0, -8.5);
      glUniform3fv(prog->getUniform("lightPos4"), 1, &lightDir[0]);
      lightDir = vec3(4.2, 7.0, -8.5);
      glUniform3fv(prog->getUniform("lightPos5"), 1, &lightDir[0]);
      lightDir = vec3(11.2, 7.0, -8.3);
      glUniform3fv(prog->getUniform("lightPos6"), 1, &lightDir[0]);
      lightDir = vec3(8.6, 7.0, 2.0);
      glUniform3fv(prog->getUniform("lightPos7"), 1, &lightDir[0]);
      lightDir = vec3(8.0, 7.0, 8.3);
      glUniform3fv(prog->getUniform("lightPos8"), 1, &lightDir[0]);
      lightDir = vec3(0.0, -1.0, 0.0);
      glUniform3fv(prog->getUniform("lightDir"), 1, &lightDir[0]);
      //glUniform1fv(prog->getUniform("lightRad"), 1, cos(radians(12.5f)));

      // set up all the matrices
      auto Model = make_shared<MatrixStack>();

      glm::mat4 M = glm::mat4(1.0f);
      glm::mat4 S = glm::mat4(1.0f);
      glm::mat4 T = glm::mat4(1.0f);
      glm::mat4 R = glm::mat4(1.0f);

      static float w = 0.0;
      w += 1.0 * frametime; // rotation angle

      glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P));
      glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(V));
      //glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
      glUniform3fv(prog->getUniform("campos"), 1, &camPos[0]);
      //glUniform3fv(prog->getUniform("campos"), 1, &mycam.getPos()[0]);
      glActiveTexture(GL_TEXTURE0);
      //glBindTexture(GL_TEXTURE_2D, Texture);

      glBindTexture(GL_TEXTURE_2D, 0);

      // draw room
      M = glm::mat4(1);
      S = glm::scale(glm::mat4(1.0f), glm::vec3(12.5, 12.5, 12.5));
      T = glm::translate(glm::mat4(1.0f), glm::vec3(0, 3.8, 0));
      M = T * S;
      glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
      room->draw(prog, false);

      glm::vec4 pink = glm::vec4(1.0, 0.357, 0.796, 1);
      glm::vec4 green = glm::vec4(0.486, 0.988, 0, 1);
      glm::vec3 blue = glm::vec3(0.2588, 0.4, 0.9608);
      glm::vec4 red = glm::vec4(1, 0.302, 0.302, 1);

      static float fcount = 0.0;
      fcount += 0.01;

      T = glm::translate(glm::mat4(1.0f), vec3(mycam.getPos().x, mycam.getPos().y - 0.5, mycam.getPos().z));
      S = glm::scale(glm::mat4(1.0f), glm::vec3(0.5));
      static float rot = 0;
      if (mycam.newKey) {
          if (mycam.wkey())
          {
              rot = -atan(mycam.getFront().z / mycam.getFront().x) + 90.0 / 180.0 * M_PI;
              if (mycam.getFront().x < 0)
                  rot += M_PI;
              //mycam.newKey = false;
          }
          else if (mycam.dkey())
          {
              rot = -M_PI / 2 + -atan(mycam.getFront().z / mycam.getFront().x) + 90.0 / 180.0 * M_PI;
              if (mycam.getFront().x < 0)
                  rot += M_PI;
              //mycam.newKey = false;
          }
          else if (mycam.skey())
          {
              rot = M_PI + -atan(mycam.getFront().z / mycam.getFront().x) + 90.0 / 180.0 * M_PI;
              if (mycam.getFront().x < 0)
                  rot += M_PI;
              //mycam.newKey = false;
          }
          else if (mycam.akey())
          {
              rot = M_PI / 2 + -atan(mycam.getFront().z / mycam.getFront().x) + 90.0 / 180.0 * M_PI;
              if (mycam.getFront().x < 0)
                  rot += M_PI;
              //mycam.newKey = false;
          }
      }
      else
      {
          rot = rot;
      }
      R = glm::rotate(glm::mat4(1.0f), rot, vec3(0, 1, 0));
      M = T * S * R;

      // draw the game objects
      for (int i = 0; i < myManager->getObjects().size(); i++) {
        glBindTexture(GL_TEXTURE_2D, CatTex2);
        std::shared_ptr<gameObject> currObj = myManager->getObjects().at(i);
        vec3 currPos = currObj->getPos();
        if (!currObj->getIsStatic())
        {
          // object is a cat -- use matrix stack
          glUniform4fv(prog->getUniform("objColor"), 1, &pink[0]);
          Model->pushMatrix();
          Model->loadIdentity();
          Model->multMatrix(myManager->getObjects().at(i)->getMatrix() * S);
          glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
          // torso transforms
          for (int i = 0; i < 3; i++)
          {
            if (i != 4)
              currObj->getMesh()->draw(prog, i, true);
          }
          // tail transform
          Model->pushMatrix();
          float angle = sin(glfwGetTime() * 5) / 3;
          Model->rotate(-25.0f, vec3(1, 0, 0));
          Model->rotate(angle, vec3(0, 0, 1));
          // clip the tail slightly back into the model -- this helps hide any disjoint
          Model->translate(vec3(0, -0.05f, 0.05f));
          glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
          currObj->getMesh()->draw(prog, 4, true);
          Model->popMatrix();
          glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
          // right rear leg
          Model->pushMatrix();
          angle = sin(glfwGetTime() * 3) / 3;
          Model->rotate(angle, vec3(1, 0, 0));
          Model->translate(vec3(0, fabs(angle) / 2, 0));
          glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
          currObj->getMesh()->draw(prog, 8, true);
          Model->popMatrix();
          // right front leg
          Model->pushMatrix();
          angle = sin(glfwGetTime() * 3 - 1) / 3;
          Model->rotate(angle, vec3(1, 0, 0));
          Model->translate(vec3(0, fabs(angle) / 2, 0));
          glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
          currObj->getMesh()->draw(prog, 6, true);
          Model->popMatrix();
          // left rear leg
          Model->pushMatrix();
          angle = sin(glfwGetTime() * 3 - 2) / 3;
          Model->rotate(angle, vec3(1, 0, 0));
          Model->translate(vec3(0, fabs(angle) / 2, 0));
          glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
          currObj->getMesh()->draw(prog, 7, true);
          Model->popMatrix();
          // left front leg
          Model->pushMatrix();
          angle = sin(glfwGetTime() * 3 - 3) / 3;
          Model->rotate(angle, vec3(1, 0, 0));
          Model->translate(vec3(0, fabs(angle) / 2, 0));
          glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
          currObj->getMesh()->draw(prog, 5, true);
          Model->popMatrix();
          Model->popMatrix();
        }
        else
        {
          glBindTexture(GL_TEXTURE_2D, 0);
          glUniform4fv(prog->getUniform("objColor"), 1, &green[0]);
          // transform kibble
          S = glm::scale(glm::mat4(1.0f), glm::vec3(currObj->getRad()));
          currPos.y = 0.1f;
          T = glm::translate(glm::mat4(1.0f), currPos);
          // kibble rotation
          R = glm::rotate(glm::mat4(1), w * 3, glm::vec3(0.0f, 1.0f, 0.0f));
          M = T * R * S;
          glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
          currObj->getMesh()->draw(prog, true);
        }
      }
      glBindTexture(GL_TEXTURE_2D, 0);


      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, CatTex1);
      // base transforms for player model
      Model->pushMatrix();
        Model->loadIdentity();
        Model->translate(vec3(mycam.getPos().x, mycam.getPos().y - 0.5, mycam.getPos().z));
        if (mycam.getSpeedBoost() > 0.0f) /* SQUASH & STRETCH */
          Model->scale(glm::vec3(mycam.getSscale(), 0.5f, mycam.getSscale()));
        else {
          Model->scale(0.5f);
        }
        Model->rotate(rot, vec3(0, 1, 0));  
        glUniform4fv(prog->getUniform("objColor"), 1, &red[0]);
        glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
        // texture effects
        if (mycam.getDisplayDamage()) // damage 
          glBindTexture(GL_TEXTURE_2D, 0);
        else if (mycam.getSpeedBoost() > 0.0f) // power up 
          glBindTexture(GL_TEXTURE_2D, CatTex3);
        else // normal 
          glBindTexture(GL_TEXTURE_2D, CatTex1);
        // draw everything but the tail with these transforms
        for (int i = 0; i < cat->getObjCount(); i++) {
          if (i != 4)
            cat->draw(prog, i, true);
        }
        // tail transforms
        Model->pushMatrix();
          float angle = sin(glfwGetTime() * 3) / 3;
          Model->rotate(angle, vec3(0, 0, 1));
          // clip the tail slightly back into the model -- this helps hide any disjoint
          Model->translate(vec3(0, 0, 0.05f));
          glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
          cat->draw(prog, 4, true);
        Model->popMatrix();
      Model->popMatrix();
      glBindTexture(GL_TEXTURE_2D, 0);


      // draw room
      /*M = glm::mat4(1);
      S = glm::scale(glm::mat4(1.0f), glm::vec3(12.5, 12.5, 12.5));
      T = glm::translate(glm::mat4(1.0f), glm::vec3(0, 3.8, 0));
      M = T * S;
      //glUniform3fv(prog->getUniform("campos"), 1, &camPos[0]);
      //glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P));
      //glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(V));
      glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);

      room->draw(prog, false);*/
        
      /*glDisableVertexAttribArray(0);
      glDisableVertexAttribArray(1);
      glDisableVertexAttribArray(2);*/

      prog->unbind();

      //glDisable(GL_DEPTH_TEST);
      
      assert(glGetError() == GL_NO_ERROR);
  }


  void render() {

    glBindFramebuffer(GL_FRAMEBUFFER, fb);

    // DEFERRED SHADING
    /*if (DEFER)
      glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    else
      glBindFramebuffer(GL_FRAMEBUFFER, 0);*/

    double frametime = get_last_elapsed_time();
    myManager->process(&mycam, frametime);
    mycam.setDt(mycam.getDt() + frametime);

    // Get current frame buffer size.
    int width, height;
    glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
    float aspect = width / (float)height;
    glViewport(0, 0, width, height);

    // Clear framebuffer.
    glClearColor(0.098f, 0.098f, 0.439f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Create the matrix stacks
    glm::mat4 V, P; // View and Perspective matrix
    V = glm::mat4(1);
    // Apply orthographic projection....
    P = glm::ortho(-1 * aspect, 1 * aspect, -1.0f, 1.0f, -2.0f, 100.0f);
    if (width < height) {
      P = glm::ortho(-1.0f, 1.0f, -1.0f / aspect, 1.0f / aspect, -2.0f, 100.0f);
    }
    // ...but we overwrite it (optional) with a perspective projection.
    P = glm::perspective(
        (float)(3.14159 / 4.), (float)((float)width / (float)height), 0.1f,
        1000.0f); // so much type casting... GLM metods are quite funny ones
    V = glm::lookAt(mycam.getPos() - mycam.getFront() * vec3(1.5), mycam.getPos() + mycam.getFront(),
        mycam.getUp());
    
    mycam.processKeyboard(frametime, objects);

   /*glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);*/

    // 3RD PERSON CAMERA
    drawScene(P, V);


    // DEFERRED SHADING
    /*if (DEFER & !FirstTime)
    {
      // now draw the actual output
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      texProg->bind();
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, gPosition);
      glActiveTexture(GL_TEXTURE0 + 1);
      glBindTexture(GL_TEXTURE_2D, gNormal);
      glActiveTexture(GL_TEXTURE0 + 2);
      glBindTexture(GL_TEXTURE_2D, gColorSpec);
      // glUniform1i(texProg->getUniform("texBuf"), 0);
      glUniform1i(texProg->getUniform("gBuf"), 0);
      glUniform1i(texProg->getUniform("norBuf"), 1);
      glUniform1i(texProg->getUniform("colorBuf"), 2);
      glUniform3f(texProg->getUniform("Ldir"), g_light.x, g_light.y, g_light.z);
      glEnableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
      glDrawArrays(GL_TRIANGLES, 0, 6);
      glDisableVertexAttribArray(0);
      texProg->unbind();
    }
    if (DEFER & FirstTime)
    {
      // code to write out the FBO (texture) just once
      assert(GLTextureWriter::WriteImage(gBuffer, "gBuf.png"));
      assert(GLTextureWriter::WriteImage(gPosition, "gPos.png"));
      assert(GLTextureWriter::WriteImage(gNormal, "gNorm.png"));
      assert(GLTextureWriter::WriteImage(gColorSpec, "gColorSpec.png"));
      FirstTime = false;
    }*/

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // MINI MAP
    mat4 OrthoProj = GetOrthoMatrix();
    mat4 TopView = GetTopView();
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(0, height-180, 320, 180);
    drawScene(P, TopView); // draw the complete scene from a top down camera

    // HEALTH
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(width-640, height-180, 640, 180);
    drawHealth(P, TopView); /* HEALTH */

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, FBOtex);
    glGenerateMipmap(GL_TEXTURE_2D);
  }

  void render_with_framebuffer()
  {
      // Get current frame buffer size.
      int width, height;
      glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
      glViewport(0, 0, width, height);
      // Clear framebuffer.
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      TVstatic->bind();
      static float value = 0.0f;
      value += 0.01;
      glUniform1f(TVstatic->getUniform("u_distortion"), 0.0f / 100.0f);
      glUniform1f(TVstatic->getUniform("u_stripe"), 0.0f  / 100.0f);
      glUniform1f(TVstatic->getUniform("u_rgbshift"), 2.5 / 1000.0f);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, FBOtex);
      glBindVertexArray(VertexArrayIDScreen);
      glDrawArrays(GL_TRIANGLES, 0, 6);
      TVstatic->unbind();
  }
};

//******************************************************************************************
int main(int argc, char **argv) {
  std::string resourceDir =
      "../resources"; // Where the resources are loaded from
  if (argc >= 2) {
    resourceDir = argv[1];
  }

  // *** audio engine ***
  audioManager myaudio;
  myaudio.initEngine(1);
  myaudio.initEngine(2);
  myaudio.initEngine(3);
  myaudio.initEngine(4);
  mycam.setAudio(myaudio);
  myaudio.playRoost();

  Application *application = new Application();

  /* your main will always include a similar set up to establish your window
          and GL context, etc. */
  WindowManager *windowManager = new WindowManager();
  windowManager->init(1920, 1080);
  //windowManager->init(960, 540);
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
  application->initRoomGeo();
  application->initGame();

  // Loop until the user closes the window.
  while (!glfwWindowShouldClose(windowManager->getHandle())) {
    // Render scene.
    application->render();
    application->render_with_framebuffer();

    // Swap front and back buffers.
    glfwSwapBuffers(windowManager->getHandle());
    // Poll for and process events.
    glfwPollEvents();
  }

  // *** audio engine ***
  myaudio.uninitEngine();

  // Quit program.
  windowManager->shutdown();

  return 0;
}
