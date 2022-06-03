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
	scale(1.0f),
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
	v.x = randFloat(-5.0f, 5.0f);
	v.y = randFloat(5.0f, 10.0f);
	v.z = randFloat(-3.0f, 3.0f);
	lifespan = randFloat(0.0f, 2.0f); 
	tEnd = t + lifespan;
	scale = randFloat(0.2, 1.0f);
  color.r = 1.0f;
	color.g = 0.0f;
	color.b = 0.0f;
  // color.g = randFloat(0.5f, 0.6f);
  // color.b = randFloat(0.0f, 0.1f);
	color.a = randFloat(1.0f, 1.0f);
}

void Particle::update(float t, float h, const vec3 &g, const vec3 start)
{
	if(t > tEnd) {
		rebirth(t, start);
	}

	v += g;

	//very simple update
	x += h * v;
	//To do - how do you want to update the forces?
	color.a = (tEnd-t) / lifespan;
}
