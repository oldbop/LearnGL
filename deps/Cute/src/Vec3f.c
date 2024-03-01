#include <Cute/Vec3f.h>
#include <stdio.h>

Cute_Vec3f Cute_MakeVec3f(float val1, float val2, float val3) {

  Cute_Vec3f new;

  new.vals[0] = val1;
  new.vals[1] = val2;
  new.vals[2] = val3;

  return new;
}

void Cute_PrintVec3f(const Cute_Vec3f *v) {
  printf("%f\n%f\n%f\n", v->vals[0], v->vals[1], v->vals[2]);
}

Cute_Vec3f Cute_ScaleVec3f(const Cute_Vec3f *v, float scalar) {

  Cute_Vec3f res = {
    scalar * v->vals[0],
    scalar * v->vals[1],
    scalar * v->vals[2]
  };

  return res;
}

Cute_Vec3f Cute_AddVec3f(const Cute_Vec3f *v1, const Cute_Vec3f *v2) {

  Cute_Vec3f res = {
    v1->vals[0] + v2->vals[0],
    v1->vals[1] + v2->vals[1],
    v1->vals[2] + v2->vals[2]
  };

  return res;
}

Cute_Vec3f Cute_SubVec3f(const Cute_Vec3f *v1, const Cute_Vec3f *v2) {

  Cute_Vec3f res = {
    v1->vals[0] - v2->vals[0],
    v1->vals[1] - v2->vals[1],
    v1->vals[2] - v2->vals[2]
  };

  return res;
}

float Cute_DotVec3f(const Cute_Vec3f *v1, const Cute_Vec3f *v2) {

  float res = v1->vals[0] * v2->vals[0] +\
              v1->vals[1] * v2->vals[1] +\
              v1->vals[2] * v2->vals[2];

  return res;
}

Cute_Vec3f Cute_CrossVec3f(const Cute_Vec3f *v1, const Cute_Vec3f *v2) {

  Cute_Vec3f res = {
    v1->vals[1] * v2->vals[2] - v1->vals[2] * v2->vals[1],
    v1->vals[2] * v2->vals[0] - v1->vals[0] * v2->vals[2],
    v1->vals[0] * v2->vals[1] - v1->vals[1] * v2->vals[0]
  };

  return res;
}
