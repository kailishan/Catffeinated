#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <algorithm>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include "particleSys.h"
#include "GLSL.h"

using namespace std;

GLenum glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__) 

particleSys::particleSys(vec3 source) {

	numP = 50;
	t = 0.0f;
	h = 0.01f;
	g = vec3(0.0f, -0.0098, 0.0f);
	start = source;
	theCamera = glm::mat4(1.0);
}

void particleSys::gpuSetup() {

	cout << "start: " << start.x << " " << start.y << " " << start.z << endl;
	for (int i = 0; i < numP; i++) {
		points[i * 3 + 0] = start.x;
		points[i * 3 + 1] = start.y;
		points[i * 3 + 2] = start.z;

		pointColors[i * 4 + 0] = 0.878;
		pointColors[i * 4 + 1] = 0.68;
		pointColors[i * 4 + 2] = 0.004;
		pointColors[i * 4 + 3] = 1.0;

		auto particle = make_shared<Particle>(start);
		particles.push_back(particle);
		particle->load(start);
	}

	//generate the VAO
	glGenVertexArrays(1, &vertArrObj);
	glCheckError(); 
	glBindVertexArray(vertArrObj);
	glCheckError(); 
	//generate vertex buffer to hand off to OGL - using instancing
	glGenBuffers(1, &vertBuffObj);
	glCheckError(); 
	//set the current state to focus on our vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertBuffObj);
	glCheckError(); 
	//actually memcopy the data - only do this once
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points[0], GL_STREAM_DRAW);
	glCheckError(); 

	glGenBuffers(1, &colorbuffer);
	glCheckError(); 
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glCheckError(); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(pointColors), &pointColors[0], GL_STREAM_DRAW);
	glCheckError(); 

	assert(glGetError() == GL_NO_ERROR);
}

void particleSys::reSet() {
	for (int i = 0; i < numP; i++) {
		particles[i]->load(start);
	}
}

void particleSys::drawMe(std::shared_ptr<Program> prog) {

	glBindVertexArray(vertArrObj);
	int h_col = prog->getAttribute("partColor");
	GLSL::enableVertexAttribArray(h_col);
	//std::cout << "Any Gl errors1: " << glGetError() << std::endl;
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	//std::cout << "Any Gl errors2: " << glGetError() << std::endl;
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

	int h_pos = prog->getAttribute("vertPos");
	GLSL::enableVertexAttribArray(h_pos);
	//std::cout << "Any Gl errors1: " << glGetError() << std::endl;
	glBindBuffer(GL_ARRAY_BUFFER, vertBuffObj);
	//std::cout << "Any Gl errors2: " << glGetError() << std::endl;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribDivisor(0, 1);
	glVertexAttribDivisor(1, 1);
	// Draw the points!
	glPointSize(10.0f * (1080 / 512.0f));
	glDrawArraysInstanced(GL_POINTS, 0, 1, numP);

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 0);

	glDisableVertexAttribArray(0);
}



void particleSys::update() {

	vec3 pos;
	vec4 col;

	//update the particles
	for (auto particle : particles) {
		particle->update(t, h, g, start);
	}
	t += h;

	// Sort the particles by Z
	//temp->rotate(camRot, vec3(0, 1, 0));
	//be sure that camera matrix is updated prior to this update
	vec3 s, t, sk;
	vec4 p;
	quat r;
	glm::decompose(theCamera, s, r, t, sk, p);
	sorter.C = glm::toMat4(r);
	sort(particles.begin(), particles.end(), sorter);

	//go through all the particles and update the CPU buffer
	for (int i = 0; i < numP; i++) {
		pos = particles[i]->getPosition();
		col = particles[i]->getColor();
		points[i * 3 + 0] = pos.x;
		points[i * 3 + 1] = pos.y;
		points[i * 3 + 2] = pos.z;
		// To do - how can you integrate unique colors per particle?
		pointColors[i * 4 + 0] = col.r + col.a / 10;

		pointColors[i * 4 + 1] = col.g + col.g / 10;
		pointColors[i * 4 + 2] = col.b + col.b / 10;
		pointColors[i * 4 + 3] = col.a;
	}

	//update the GPU data
	glBindBuffer(GL_ARRAY_BUFFER, vertBuffObj);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * numP * 3, points);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pointColors), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * numP * 4, pointColors);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
