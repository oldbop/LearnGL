#ifndef VEC3F_H
#define VEC3F_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  float vals[3];
} Cute_Vec3f;

Cute_Vec3f Cute_MakeVec3f(float val1, float val2, float val3);
void Cute_PrintVec3f(const Cute_Vec3f *v);
Cute_Vec3f Cute_ScaleVec3f(const Cute_Vec3f *v, float scalar);
Cute_Vec3f Cute_AddVec3f(const Cute_Vec3f *v1, const Cute_Vec3f *v2);
Cute_Vec3f Cute_SubVec3f(const Cute_Vec3f *v1, const Cute_Vec3f *v2);
float Cute_DotVec3f(const Cute_Vec3f *v1, const Cute_Vec3f *v2);
Cute_Vec3f Cute_CrossVec3f(const Cute_Vec3f *v1, const Cute_Vec3f *v2);

#ifdef __cplusplus
}
#endif

#endif
