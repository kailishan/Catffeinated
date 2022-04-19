/*
CPE/CSC 471 Lab base code Wood/Dunn/Eckhardt
*/
#define _USE_MATH_DEFINES

#include <iostream>
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "GLSL.h"
#include "Program.h"
#include "MatrixStack.h"
#include "math.h"

#include "WindowManager.h"
#include "Shape.h"
// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;
using namespace glm;
shared_ptr<Shape> shape;


double get_last_elapsed_time()
{
	static double lasttime = glfwGetTime();
	double actualtime = glfwGetTime();
	double difference = actualtime- lasttime;
	lasttime = actualtime;
	return difference;
}

float distance(float x1, float y1,
	float z1, float x2,
	float y2, float z2)
{
	float d = sqrt(pow(x2 - x1, 2) +
		pow(y2 - y1, 2) +
		pow(z2 - z1, 2) * 1.0);
	return d;
}

glm::vec3 midpoint(shared_ptr<Shape> shape)
{
    glm::vec3 midpoint;
    midpoint.x = (shape->min.x + shape->max.x) / 2;
    midpoint.y = (shape->min.y + shape->max.y) / 2;
    midpoint.z = (shape->min.z + shape->max.z) / 2;

    return midpoint;
}

class gameObject
{
public:

	glm::vec3 pos, vel;
	float rad, rot;
	bool destroying = false;
	bool destroyed = false;
	glm::mat4 matrix = mat4(1);

	gameObject()
	{
		//pos = glm::vec3(rand() % 25 - 12, 0, rand() % 25 - 12);
		pos = glm::vec3((rand() % 25) -12, 0.35, (rand() % 25) - 12);
		//rot = glm::radians((float)(rand() % 361)); // y-axis
		//vel = vec3(0, 0, 0); // random x and y velocity
		float velocities[] = { 0.05f, 0.025f, 0.0125f, -0.0125f, -0.025f, -0.05f };
		vel = vec3(velocities[rand() % 6], 0.0f, velocities[rand() % 6]);
		rot = tan(vel.z / vel.x);
		//vel = vec3(static_cast <float> (rand()) / static_cast <float> (1) * 0.00000000075, 0, static_cast <float> (rand()) / static_cast <float> (1) * 0.00000000075); // random x and y velocity
		vec3 posDirection = glm::normalize(pos);
		vec3 velDirection = glm::normalize(vel);
		float angle = acos(glm::dot(posDirection, velDirection));

		rad = .5;
		cout << "x: " << pos.x << " z: " << pos.z << endl;
	}

	bool isColliding(gameObject other)
	{
		if (destroying || other.destroying)
			return false;
		float d = distance(pos.x, pos.y, pos.z, other.pos.x, other.pos.y, other.pos.z);
		if (d > rad + other.rad)
			return false;
		else if (d <= rad + other.rad && !other.destroying) {
			vel.x = -vel.x;
			vel.z = -vel.z;
			return true;
		}
		else
			return false;
	}

	void destroy(double ftime)
	{
		rad -= 0.01;
		vel.x = 0;
		vel.y = 0;
		if (rad <= 0)
		{
			//destroying = false;
			//cout << "(CAT) x:" << pos.x << " z: " << pos.z << endl;
			destroyed = true;
		}
	}

	void move(double ftime)
	{	
		/*
		if (pos.x > 12.5 && vel.x > 0)
			vel.x = -vel.x;
		if (pos.x < -12.5 && vel.x < 0)
			vel.x = -vel.x;
		if (pos.z > 12.5 && vel.z > 0)
			vel.z = -vel.z;
		if (pos.z < -12.5 && vel.z < 0)
			vel.z = -vel.z;
			*/
		rot = atan(vel.z / vel.x);
		if (vel.z < 0)
			rot += radians(180.f);
		
		glm::mat4 R = glm::rotate(glm::mat4(1), rot, glm::vec3(0.0f, 1.0f, 0.0f));
		R = formRotationMatrix(ftime);
		vec4 dir = vec4(vel, 1);

		if (pos.x + dir.x > 12.5 || pos.x + dir.x < -12.5) {
			pos = pos;
			vel.x = -vel.x;
		}
		else if (pos.z + dir.z > 12.5 || pos.z + dir.z < -12.5) {
			pos = pos;
			vel.z = -vel.z; 
		}
		else
			pos += glm::vec3(dir.x, dir.y, dir.z);

		glm::mat4 T = glm::translate(glm::mat4(1), pos);
		matrix = T * R;
	}

	void process(vector <gameObject> others, int index, double ftime)
	{
		if (destroyed)
			return;
		if (destroying) {
			destroy(ftime);
			return;
		}
		else
		{
			for (int i = 0; i < others.size(); i++)
			{
				if (i == index)
					continue;
				else if (isColliding(others.at(i)))
					continue;
			}
		}
		move(ftime);
	}

	glm::mat4 formRotationMatrix(float frametime)
    {
        glm::vec3 dogMid = midpoint(shape);
        glm::vec3 curPos = pos;

        glm::vec3 dest = curPos + vel * frametime;

        // vector in direction to look at
        glm::vec3 forward = glm::normalize(dest - curPos);
        glm::vec3 left = glm::normalize(glm::cross(vec3(0, 1, 0), forward));
        glm::vec3 up = glm::cross(forward, left);
        // dog originally points forward, e.g. in +z direction
        // need to rotate dog to align with forward...
        glm::mat4 rot = glm::mat4(vec4(left.x, left.y, left.z, 0), vec4(0, 1, 0, 0),
            vec4(forward.x, forward.y, forward.z, 0), vec4(0, 0, 0, 1));

        return rot;
    }
};

class camera
{
public:

	glm::vec3 pos, rot;
	int w, a, s, d, p;
	GLFWwindow* window;
	float rad = 0.8f;
	int score = 0;

	camera()
	{
		w = a = s = d = p = 0;
		pos = glm::vec3(0, -1, 0);
		rot = glm::vec3(0, 0, 0);
	}

	bool isColliding(gameObject other)
	{
		float d = distance(-pos.x, -pos.y, -pos.z, other.pos.x, other.pos.y, other.pos.z);
		if (d > rad + other.rad)
			return false;
		else if (d <= rad + other.rad && !other.destroying)
		{
			score++;
			cout << "CATS BOOPED: " << score << endl;
			//cout << "(CAM) " << "x: " << pos.x << " z: " << pos.z << endl;
			other.destroying = true;
			return true;
		}
		else
			return false;
	}

	glm::mat4 process(double ftime)
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

		vec4 dir = vec4(0, 0, 0, 1);

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
};

camera mycam;


class gameManager
{
public:
	int maxObj = 15;
	int count = 0;
	int bound = 12.5; // min/max x/y
	int score = 0;
	int framecount = 0;
	vector <gameObject> objects;

	gameManager()
	{
		srand(glfwGetTime());
		while (count <= 14)
		{
			spawnGameObject();
		}
	}

	void spawnGameObject()
	{
		gameObject object = gameObject();
		objects.push_back(object);
		count++;
	}

	void process(double ftime)
	{
		vector <int> destroyList; 
		for (int i = 0; i < objects.size(); i++)
		{
			if (mycam.isColliding(objects.at(i)) && !objects.at(i).destroying) // CHECK COLLISION W/ PLAYER
			{
				objects.at(i).destroying = true;
				count--;
				cout << "CATS REMAINING: " << count << endl;
				//score++;
				//cout << "OBJECTS DESTROYED: " << score << endl;
			}
			objects.at(i).process(objects, i, ftime); // CHECK COLLISION W/ GAME OBJECTS
			if (objects.at(i).destroyed) // DESTROY OBJECT
			{
				if (std::find(destroyList.begin(), destroyList.end(), i) != destroyList.end())
					destroyList.push_back(i);
			}
		}


		// destroy list
		if (destroyList.size() == 0)
			return;
		for (int i = destroyList.size() - 1; i >= 0; i--)
		{
			objects.erase(objects.begin() + i);
			count--;
		}

		framecount++;
		if (count < 10 && framecount > 300)
		{
			count++;
			cout << "CATS REMAINING: " << count << endl;
			framecount = 0;
			spawnGameObject();
		}

	}
};

gameManager myManager;


class Application : public EventCallbacks
{

public:

	WindowManager * windowManager = nullptr;

	// Our shader program
	std::shared_ptr<Program> prog, progL, heightshader;

	// Contains vertex information for OpenGL
	GLuint VertexArrayID;

	// Data necessary to give our box to OpenGL
	GLuint MeshPosID, MeshTexID, IndexBufferIDBox;

	//texture data
	GLuint Texture;
	GLuint Texture2,HeightTex;

	void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		
		if (key == GLFW_KEY_W && action == GLFW_PRESS)
		{
			mycam.w = 1;
		}
		if (key == GLFW_KEY_W && action == GLFW_RELEASE)
		{
			mycam.w = 0;
		}
		if (key == GLFW_KEY_S && action == GLFW_PRESS)
		{
			mycam.s = 1;
		}
		if (key == GLFW_KEY_S && action == GLFW_RELEASE)
		{
			mycam.s = 0;
		}
		if (key == GLFW_KEY_A && action == GLFW_PRESS)
		{
			mycam.a = 1;
		}
		if (key == GLFW_KEY_A && action == GLFW_RELEASE)
		{
			mycam.a = 0;
		}
		if (key == GLFW_KEY_D && action == GLFW_PRESS)
		{
			mycam.d = 1;
		}
		if (key == GLFW_KEY_D && action == GLFW_RELEASE)
		{
			mycam.d = 0;
		}
		if (key == GLFW_KEY_P && action == GLFW_PRESS)
		{
			mycam.p = 1;
		}
	}

	bool firstMouse = true;
	float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
	float pitch = 0.0f;
	float lastX = 800.0f / 2.0;
	float lastY = 600.0 / 2.0;

	// callback for the mouse when clicked move the triangle when helper functions
	// written
	void mouseCallback(GLFWwindow *window, int button, int action, int mods)
	{
		double posX, posY;
		float newPt[2];
		if (action == GLFW_PRESS)
		{
			glfwGetCursorPos(window, &posX, &posY);
			std::cout << "Pos X " << posX <<  " Pos Y " << posY << std::endl;

			//change this to be the points converted to WORLD
			//THIS IS BROKEN< YOU GET TO FIX IT - yay!
			newPt[0] = 0;
			newPt[1] = 0;

			std::cout << "converted:" << newPt[0] << " " << newPt[1] << std::endl;
			glBindBuffer(GL_ARRAY_BUFFER, MeshPosID);
			//update the vertex array with the updated points
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(float)*6, sizeof(float)*2, newPt);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}

	//if the window is resized, capture the new size and reset the viewport
	void resizeCallback(GLFWwindow *window, int in_width, int in_height)
	{
		//get the window size - may be different then pixels for retina
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
	}
#define MESHSIZE 25
	void init_mesh()
	{
		//generate the VAO
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);

		//generate vertex buffer to hand off to OGL
		glGenBuffers(1, &MeshPosID);
		glBindBuffer(GL_ARRAY_BUFFER, MeshPosID);
		vec3 vertices[MESHSIZE * MESHSIZE * 4];
		for(int x=0;x<MESHSIZE;x++)
			for (int z = 0; z < MESHSIZE; z++)
				{
				vertices[x * 4 + z*MESHSIZE * 4 + 0] = vec3(0.0, 0.0, 0.0) + vec3(x, 0, z);
				vertices[x * 4 + z*MESHSIZE * 4 + 1] = vec3(1.0, 0.0, 0.0) + vec3(x, 0, z);
				vertices[x * 4 + z*MESHSIZE * 4 + 2] = vec3(1.0, 0.0, 1.0) + vec3(x, 0, z);
				vertices[x * 4 + z*MESHSIZE * 4 + 3] = vec3(0.0, 0.0, 1.0) + vec3(x, 0, z);
				}
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * MESHSIZE * MESHSIZE * 4, vertices, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		//tex coords
		float t = 1. / 100;
		vec2 tex[MESHSIZE * MESHSIZE * 4];
		for (int x = 0; x<MESHSIZE; x++)
			for (int y = 0; y < MESHSIZE; y++)
			{
				tex[x * 4 + y*MESHSIZE * 4 + 0] = vec2(0.0, 0.0)+ vec2(x, y)*t;
				tex[x * 4 + y*MESHSIZE * 4 + 1] = vec2(t, 0.0)+ vec2(x, y)*t;
				tex[x * 4 + y*MESHSIZE * 4 + 2] = vec2(t, t)+ vec2(x, y)*t;
				tex[x * 4 + y*MESHSIZE * 4 + 3] = vec2(0.0, t)+ vec2(x, y)*t;
			}
		glGenBuffers(1, &MeshTexID);
		//set the current state to focus on our vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, MeshTexID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * MESHSIZE * MESHSIZE * 4, tex, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glGenBuffers(1, &IndexBufferIDBox);
		//set the current state to focus on our vertex buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferIDBox);
		GLushort elements[MESHSIZE * MESHSIZE * 6];
		int ind = 0;
		for (int i = 0; i<MESHSIZE * MESHSIZE * 6; i+=6, ind+=4)
			{
			elements[i + 0] = ind + 0;
			elements[i + 1] = ind + 1;
			elements[i + 2] = ind + 2;
			elements[i + 3] = ind + 0;
			elements[i + 4] = ind + 2;
			elements[i + 5] = ind + 3;
			}			
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*MESHSIZE * MESHSIZE * 6, elements, GL_STATIC_DRAW);
		glBindVertexArray(0);
	}
	/*Note that any gl calls must always happen after a GL state is initialized */
	void initGeom()
	{
		//initialize the net mesh
		init_mesh();

		string resourceDirectory = "../resources" ;
		// Initialize mesh.
		shape = make_shared<Shape>();
		//shape->loadMesh(resourceDirectory + "/t800.obj");
		shape->loadMesh(resourceDirectory + "/Cat_Low.obj");
		shape->resize();
		shape->init();

		int width, height, channels;
		char filepath[1000];

		//texture 1
		string str = resourceDirectory + "/grass.jpg";
		strcpy(filepath, str.c_str());
		unsigned char* data = stbi_load(filepath, &width, &height, &channels, 4);
		glGenTextures(1, &Texture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		//texture 2
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		//texture 3
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);


		//[TWOTEXTURES]
		//set the 2 textures to the correct samplers in the fragment shader:
		GLuint Tex1Location = glGetUniformLocation(prog->pid, "tex");//tex, tex2... sampler in the fragment shader
		GLuint Tex2Location = glGetUniformLocation(prog->pid, "tex2");
		// Then bind the uniform samplers to texture units:
		glUseProgram(prog->pid);
		glUniform1i(Tex1Location, 0);
		glUniform1i(Tex2Location, 1);

		Tex1Location = glGetUniformLocation(heightshader->pid, "tex");//tex, tex2... sampler in the fragment shader
		Tex2Location = glGetUniformLocation(heightshader->pid, "tex2");
		// Then bind the uniform samplers to texture units:
		glUseProgram(heightshader->pid);
		glUniform1i(Tex1Location, 0);
		glUniform1i(Tex2Location, 1);


		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	}

	//General OGL initialization - set OGL state here
	void init(const std::string& resourceDirectory)
	{
		GLSL::checkVersion();

		// Set background color.
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		// Enable z-buffer test.
		glEnable(GL_DEPTH_TEST);

		// Initialize the GLSL program.
		prog = std::make_shared<Program>();
		prog->setVerbose(true);
		prog->setShaderNames(resourceDirectory + "/shader_vertex.glsl", resourceDirectory + "/shader_fragment.glsl");
		if (!prog->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
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
		progL->setShaderNames(resourceDirectory + "/shader_vertex_light.glsl", resourceDirectory + "/shader_fragment_light.glsl");
		if (!progL->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
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
		heightshader->setShaderNames(resourceDirectory + "/height_vertex.glsl", resourceDirectory + "/height_frag.glsl");
		if (!heightshader->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
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
	void render()
	{
		double frametime = get_last_elapsed_time();
		myManager.process(frametime);

		// Get current frame buffer size.
		int width, height;
		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
		float aspect = width/(float)height;
		glViewport(0, 0, width, height);

		// Clear framebuffer.
		glClearColor(0.8f, 0.8f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Create the matrix stacks - please leave these alone for now
		
		glm::mat4 V, M, P; //View, Model and Perspective matrix
		V = glm::mat4(1);
		M = glm::mat4(1);
		// Apply orthographic projection....
		P = glm::ortho(-1 * aspect, 1 * aspect, -1.0f, 1.0f, -2.0f, 100.0f);		
		if (width < height)
			{
			P = glm::ortho(-1.0f, 1.0f, -1.0f / aspect,  1.0f / aspect, -2.0f, 100.0f);
			}
		// ...but we overwrite it (optional) with a perspective projection.
		P = glm::perspective((float)(3.14159 / 4.), (float)((float)width/ (float)height), 0.1f, 1000.0f); //so much type casting... GLM metods are quite funny ones

		//animation with the model matrix:
		static float w = 0.0;
		w += 1.0 * frametime;//rotation angle
		float trans = 0;// sin(t) * 2;
		glm::mat4 RotateY = glm::rotate(glm::mat4(1.0f), w, glm::vec3(0.0f, 1.0f, 0.0f));
		float angle = -3.1415926/2.0;
		glm::mat4 RotateX = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3 + trans));
		
		glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 0.3f, 0.3f));
		glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		//M =  TransZ * RotateY * RotateX * S;
		M = S * T;

		// Draw the box using GLSL.
		progL->bind();

		V = mycam.process(frametime);
		//send the matrices to the shaders
		glUniformMatrix4fv(progL->getUniform("P"), 1, GL_FALSE, &P[0][0]);
		glUniformMatrix4fv(progL->getUniform("V"), 1, GL_FALSE, &V[0][0]);
		glUniformMatrix4fv(progL->getUniform("M"), 1, GL_FALSE, &M[0][0]);
		glUniform3fv(progL->getUniform("campos"), 1, &mycam.pos[0]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);

		glm::vec4 pink = glm::vec4(1.0, 0.357, 0.796, 1);
		glUniform4fv(progL->getUniform("objColor"), 1, &pink[0]);

		for (int i = 0; i < myManager.objects.size(); i++)
		{
			gameObject currObj = myManager.objects.at(i);
			vec3 currPos = currObj.pos;
			S = glm::scale(glm::mat4(1.0f), glm::vec3(currObj.rad));
			T = glm::translate(glm::mat4(1.0f), currPos);
			glm::mat4 R = glm::rotate(glm::mat4(1), currObj.rot, glm::vec3(0, 1, 0));
			M = myManager.objects.at(i).matrix * S;
			glUniformMatrix4fv(progL->getUniform("M"), 1, GL_FALSE, &M[0][0]);
			shape->draw(progL, false);
		}
		//shape->draw(prog,FALSE);

		heightshader->bind();
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glm::mat4 TransY = glm::translate(glm::mat4(1.0f), glm::vec3(-50.0f, -3.0f, -50));
		//M = TransY;
		M = glm::translate(glm::mat4(1.0f), glm::vec3(-12.5f, 0.0f, -12.5f));
		glUniformMatrix4fv(heightshader->getUniform("M"), 1, GL_FALSE, &M[0][0]);
		glUniformMatrix4fv(heightshader->getUniform("P"), 1, GL_FALSE, &P[0][0]);
		glUniformMatrix4fv(heightshader->getUniform("V"), 1, GL_FALSE, &V[0][0]);
		
		
		vec3 offset = mycam.pos;
		offset.y = 0;
		offset.x = 0; // (int)offset.x;
		offset.z = 0; // (int)offset.z;
		glUniform3fv(heightshader->getUniform("camoff"), 1, &offset[0]);
		glUniform3fv(heightshader->getUniform("campos"), 1, &mycam.pos[0]);
		glBindVertexArray(VertexArrayID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferIDBox);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, HeightTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glDrawElements(GL_TRIANGLES, MESHSIZE*MESHSIZE*6, GL_UNSIGNED_SHORT, (void*)0);

		heightshader->unbind();

	}

};
//******************************************************************************************
int main(int argc, char **argv)
{
	std::string resourceDir = "../resources"; // Where the resources are loaded from
	if (argc >= 2)
	{
		resourceDir = argv[1];
	}

	Application *application = new Application();

	/* your main will always include a similar set up to establish your window
		and GL context, etc. */
	WindowManager * windowManager = new WindowManager();
	windowManager->init(1920, 1080);
	windowManager->setEventCallbacks(application);
	application->windowManager = windowManager;
	mycam.window = windowManager->getHandle();

	/* This is the code that will likely change program to program as you
		may need to initialize or set up different data and state */
	// Initialize scene.
	application->init(resourceDir);
	application->initGeom();

	// Loop until the user closes the window.
	while(! glfwWindowShouldClose(windowManager->getHandle()))
	{
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
