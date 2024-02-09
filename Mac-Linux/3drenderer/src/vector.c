#include <math.h>
#include "vector.h"

/// Vector 2D Function Implementations
float vec2_length(vec2_t v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}

vec2_t vec2_add(vec2_t v1, vec2_t v2)
{
    vec2_t result_vector;
    result_vector.x = v1.x + v2.x;
    result_vector.y = v1.y + v2.y;
    return result_vector;
}

vec2_t vec2_substract(vec2_t v1, vec2_t v2)
{
    vec2_t result_vector;
    result_vector.x = v1.x - v2.x;
    result_vector.y = v1.y - v2.y;
    return result_vector;
}

vec2_t vec2_mul(vec2_t v, float factor)
{
    vec2_t result_vector = {
        .x = v.x * factor,
        .y = v.y * factor
    };

    return result_vector;
}

vec2_t vec2_div(vec2_t v, float factor)
{
    vec2_t result_vector = {
        .x = v.x / factor,
        .y = v.y / factor
    };

    return result_vector;
}

float vec2_dot(vec2_t v1, vec2_t v2)
{
    float result;
    result = (v1.x * v2.x) + (v1.y * v2.y);
    return result;
}

void vec2_normalize(vec2_t * v)
{
    float magnitude = sqrt(v->x * v->x + v->y * v->y);
    v->x /= magnitude;
    v->y /= magnitude;
}

/// Vector 3D Function Implementations
float vec3_length(vec3_t v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec3_t vec3_add(vec3_t v1, vec3_t v2)
{
    vec3_t result_vector;
    result_vector.x = v1.x + v2.x;
    result_vector.y = v1.y + v2.y;
    result_vector.z = v1.z + v2.z;
    return result_vector;
}

vec3_t vec3_substract(vec3_t v1, vec3_t v2)
{
    vec3_t result_vector;
    result_vector.x = v1.x - v2.x;
    result_vector.y = v1.y - v2.y;
    result_vector.z = v1.z - v2.z;
    return result_vector;
}

vec3_t vec3_mul(vec3_t v, float factor)
{
    vec3_t result_vector = {
        .x = v.x * factor,
        .y = v.y * factor,
        .z = v.z * factor
    };

    return result_vector;
}

vec3_t vec3_div(vec3_t v, float factor)
{
    vec3_t result_vector = {
        .x = v.x / factor,
        .y = v.y / factor,
        .z = v.z /factor
    };

    return result_vector;
}

vec3_t vec3_cross(vec3_t v1, vec3_t v2)
{
    vec3_t result_vector = 
    {
        .x = v1.y * v2.z - v1.z * v2.y,
        .y = v1.z * v2.x - v1.x * v2.z,
        .z = v1.x * v2.y - v1.y * v2.x
    };

    return result_vector;
}

float vec3_dot(vec3_t v1, vec3_t v2)
{
    float result;
    result = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
    return result;
}

void vec3_normalize(vec3_t * v)
{
    float magnitude = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
    v->x /= magnitude;
    v->y /= magnitude;
    v->z /= magnitude;
}

vec3_t vec3_rotate_x(vec3_t v, float angle) {
    vec3_t rotated_vector = {
        .x = v.x,
        .y = v.y * cos(angle) - v.z * sin(angle),
        .z = v.y * sin(angle) + v.z * cos(angle)
    };
    return rotated_vector;
}

vec3_t vec3_rotate_y(vec3_t v, float angle) {
    vec3_t rotated_vector = {
        .x = v.x * cos(angle) - v.z * sin(angle),
        .y = v.y,
        .z = v.x * sin(angle) + v.z * cos(angle)
    };
    return rotated_vector;
}

vec3_t vec3_rotate_z(vec3_t v, float angle) {
    vec3_t rotated_vector = {
        .x = v.x * cos(angle) - v.y * sin(angle),
        .y = v.x * sin(angle) + v.y * cos(angle),
        .z = v.z
    };
    return rotated_vector;
}


// Vector 4 Operations
vec4_t vec4_from_vec3(vec3_t v){
    vec4_t result;
    result.x = v.x;
    result.y = v.y;
    result.z = v.z;
    result.w = 1.0;

    return result;
}

vec3_t vec3_from_vec4(vec4_t v){
    vec3_t result;
    result.x = v.x;
    result.y = v.y;
    result.z = v.z;
    return result;
}