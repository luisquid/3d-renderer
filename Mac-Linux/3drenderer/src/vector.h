#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    float x;
    float y;
} vec2_t;

typedef struct {
    float x;
    float y;
    float z;
} vec3_t;

typedef struct {
    float x;
    float y;
    float z;
    float w;
} vec4_t;

// Vector 2D Functions
vec2_t vec2_new(float x, float y);
float vec2_length(vec2_t v);
vec2_t vec2_substract(vec2_t v1, vec2_t v2);
vec2_t vec2_mul(vec2_t v, float factor);
vec2_t vec2_div(vec2_t v, float factor);
float vec2_dot(vec2_t v1, vec2_t v2);
void vec2_normalize(vec2_t * v);


// Vector 3D Functions
vec3_t vec3_new(float x, float y, float z);
vec3_t vec3_clone(vec3_t * v);
float vec3_length(vec3_t v);
vec3_t vec3_add(vec3_t v1, vec3_t v2);
vec3_t vec3_substract(vec3_t v1, vec3_t v2);
vec3_t vec3_mul(vec3_t v, float factor);
vec3_t vec3_div(vec3_t v, float factor);
vec3_t vec3_cross(vec3_t v1, vec3_t v2);
float vec3_dot(vec3_t v1, vec3_t v2);
void vec3_normalize(vec3_t * v);

vec3_t vec3_rotate_x(vec3_t v, float angle);
vec3_t vec3_rotate_y(vec3_t v, float angle);
vec3_t vec3_rotate_z(vec3_t v, float angle);


// Vector 4D Functions
vec4_t vec4_from_vec3(vec3_t v);
vec3_t vec3_from_vec4(vec4_t v);
vec2_t vec2_from_vec4(vec4_t v);
#endif
