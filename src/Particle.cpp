//
// sueda - geometry edits Z. Wood
// 3/16
//

#include <iostream>
#include "Particle.h"


float randFloat(float l, float h)
{
	float r = rand() / (float) RAND_MAX;
	return (1.0f - r) * l + r * h;
}

Particle::Particle(vec3 start) :
	charge(1.0f),
	m(1.0f),
	d(0.0f),
	x(start),
	v(0.0f, 0.0f, 0.0f),
	lifespan(1.0f),
	tEnd(0.0f),
	color(1.0f, 1.0f, 1.0f, 1.0f)
{
}

Particle::~Particle()
{
}

void Particle::load(vec3 start)
{
	// Random initialization
	rebirth(0.0f, start);
}

/* all particles born at the origin */
void Particle::rebirth(float t, vec3 start)
{
	charge = randFloat(0.0f, 1.0f) < 0.5 ? -1.0f : 1.0f;	
	m = 1.0f;
  	d = randFloat(0.0f, 0.02f);
	x = start;
	v.x = randFloat(-0.5f, 0.5f);
	v.y = randFloat(-0.5f, 0.5f);
	v.z = randFloat(-0.5f, 0.5f);
	lifespan = randFloat(0.0f, 2.0f); 
	tEnd = t + lifespan;
	scale = randFloat(0.2, 1.0f);
  	color.r = randFloat(0.9f, 1.0f);
	color.g = randFloat(0.9f, 1.0f);
  	color.b = randFloat(0.9f, 1.0f);
	color.a = randFloat(0.2f, 1.0f);
}

void Particle::update(float t, float h, const vec3 &g, const vec3 start)
{
	if(t > tEnd) {
		rebirth(t, start);
	}

	//v += g;

	//very simple update
	x += h * v;
	//To do - how do you want to update the forces?
	color.g -= 0.015f;
	color.b -= 0.025f;
	color.a = (tEnd-t) / lifespan;
}
