#include "constrain.h"

constrain::constrain()
{
  a = nullptr;
  b = nullptr;
}

constrain::constrain(Particle *aa, Particle *bb, float restDistance)
{
  a = aa;
  b = bb;
  this->restDistance = restDistance;
}

constrain::constrain(const constrain &c)
{
  a = c.a;
  b = c.b;
  restDistance = c.restDistance;
}

void constrain::satisifyConstrains()
{
  Vector3 diff = b->position - a->position;
  float currentDist = diff.length();
  if(currentDist == 0)return;
  diff = diff * (1 - restDistance / currentDist) * 0.5;
  a->position = a->position + diff;
  b->position = b->position - diff;
}

