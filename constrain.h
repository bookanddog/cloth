#ifndef CONSTRAIN_H
#define CONSTRAIN_H

#include "particle.h"

class constrain
{
public:
  Particle *a;
  Particle *b;
  float restDistance = 0.0;

  public:
  constrain();
  constrain(Particle *aa, Particle *bb, float restDistance);
  constrain(const constrain &c);
//  void satisifyConstrains(Particle &p1, Particle &p2, float distance);
  void satisifyConstrains();
};

#endif // CONSTRAIN_H
