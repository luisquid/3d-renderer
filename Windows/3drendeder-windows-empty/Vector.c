#include <math.h>
#include "Vector.h"

vec3_t Vec3RotateX(vec3_t v, float angle) {
  vec3_t rotatedVector = {
      .x = v.x,
      .y = v.y * cos(angle) - v.z * sin(angle),
      .z = v.y * sin(angle) + v.z * cos(angle)
  };
  return rotatedVector;
}

vec3_t Vec3RotateY(vec3_t v, float angle) {
  vec3_t rotatedVector = {
      .x = v.x * cos(angle) - v.z * sin(angle),
      .y = v.y,
      .z = v.x * sin(angle) + v.z * cos(angle)
  };
  return rotatedVector;
}

vec3_t Vec3RotateZ(vec3_t v, float angle) {
  vec3_t rotatedVector = {
      .x = v.x * cos(angle) - v.y * sin(angle),
      .y = v.x * sin(angle) + v.y * cos(angle),
      .z = v.z
  };
  return rotatedVector;
}
