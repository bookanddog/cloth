#include "particle.h"

Particle::Particle()
{

}

Particle::Particle(float x, float y, float z, float mass,float d, float xg, float yg)
{
  position = clothFunction(x, y, d,xg,yg);
  previous = clothFunction(x, y,d,xg,yg);
  original = clothFunction(x, y,d,xg,yg);
  a.set(0, 0, 0);
  this->mass = mass;
  invMass = 1 / mass;
}

Particle::Particle(const Particle &p)
{
  position = p.position;
  previous = p.previous;
  original = p.original;
  a = p.a;
  mass = p.mass;
  invMass = p.invMass;
  id = p.id;
}

void Particle::addAcceleration( Vector3 &force)
{
  a = a + (force * invMass);
  force = force / invMass;
}

void Particle::integrate(float timesq, float DRAG)
{
  Vector3 newPos = position - previous;
  newPos = newPos * DRAG + position;
  newPos = newPos + ( a * timesq);

  previous = position;
  position = newPos;

  a.set(0, 0, 0);
}

Vector3 Particle::clothFunction(float x, float y,float restDistance, float xSegs, float ySegs )
{
  return Vector3((x - 0.5) * (restDistance  * xSegs), (y + 0.5) *  (restDistance  * ySegs), 0);
}

