#ifndef PARTICLE_H
#define PARTICLE_H

#include "vector.h"

class Particle
{
  public:
    Vector3 position;
    Vector3 previous;
    Vector3 original;
    Vector3 a;
    float mass = 0.0;
    float invMass = 0.0;
//    Vector3 tmp;
//    Vector3 tmp2;
    int id = -1;

public:
  Particle();
  Particle(float x, float y, float z, float mass, float d, float xg, float yg);
  Particle(const Particle &p);

  void addAcceleration(Vector3 &force);
  void integrate(float timesq, float DRAG);

  Vector3 clothFunction(float x, float y, float restDistance, float xSegs, float ySegs);


};

#endif // PARTICLE_H
