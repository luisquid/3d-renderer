#include <math.h>
#include "clipping.h"

#define NUM_PLANES 6 
plane_t frustum_planes [NUM_PLANES];

void initialize_frustum_planes(float fov, float z_near, float z_far){
    float cos_half_pov = cos(fov/2);
    float sin_half_pov = sin(fov/2);

    frustum_planes[LEFT_FRUSTRUM_PLANE].point = vec3_new(0, 0, 0);
    frustum_planes[LEFT_FRUSTRUM_PLANE].normal.x = cos_half_pov;
    frustum_planes[LEFT_FRUSTRUM_PLANE].normal.y = 0;
    frustum_planes[LEFT_FRUSTRUM_PLANE].normal.z = sin_half_pov;

    frustum_planes[RIGHT_FRUSTRUM_PLANE].point = vec3_new(0, 0, 0);;
    frustum_planes[RIGHT_FRUSTRUM_PLANE].normal.x = -cos_half_pov;
    frustum_planes[RIGHT_FRUSTRUM_PLANE].normal.y = 0;
    frustum_planes[RIGHT_FRUSTRUM_PLANE].normal.z = sin_half_pov;

    frustum_planes[TOP_FRUSTRUM_PLANE].point = vec3_new(0, 0, 0);;
    frustum_planes[TOP_FRUSTRUM_PLANE].normal.x = 0;
    frustum_planes[TOP_FRUSTRUM_PLANE].normal.y = -cos_half_pov;
    frustum_planes[TOP_FRUSTRUM_PLANE].normal.z = sin_half_pov;

    frustum_planes[BOTTOM_FRUSTRUM_PLANE].point = vec3_new(0, 0, 0);;
    frustum_planes[BOTTOM_FRUSTRUM_PLANE].normal.x = 0;
    frustum_planes[BOTTOM_FRUSTRUM_PLANE].normal.y = cos_half_pov;
    frustum_planes[BOTTOM_FRUSTRUM_PLANE].normal.z = sin_half_pov;

    frustum_planes[NEAR_FRUSTRUM_PLANE].point = vec3_new(0, 0, z_near);
    frustum_planes[NEAR_FRUSTRUM_PLANE].normal.x = 0;
    frustum_planes[NEAR_FRUSTRUM_PLANE].normal.y = 0;
    frustum_planes[NEAR_FRUSTRUM_PLANE].normal.z = 1;

    frustum_planes[FAR_FRUSTRUM_PLANE].point = vec3_new(0, 0, z_far);
    frustum_planes[FAR_FRUSTRUM_PLANE].normal.x = 0;
    frustum_planes[FAR_FRUSTRUM_PLANE].normal.y = 0;
    frustum_planes[FAR_FRUSTRUM_PLANE].normal.z = -1;
}

polygon_t create_polygon_from_triangle(vec3_t v0, vec3_t v1, vec3_t v2){
    polygon_t result = {
        .vertices = {v0, v1, v2},
        .num_vertices = 3
    };

    return result;
}

void clip_polygon_against_plane(polygon_t * p, int plane){
    
}

void clip_polygon(polygon_t * p){
    clip_polygon_against_plane(p, LEFT_FRUSTRUM_PLANE);
    clip_polygon_against_plane(p, RIGHT_FRUSTRUM_PLANE);
    clip_polygon_against_plane(p, TOP_FRUSTRUM_PLANE);
    clip_polygon_against_plane(p, BOTTOM_FRUSTRUM_PLANE);
    clip_polygon_against_plane(p, NEAR_FRUSTRUM_PLANE);
    clip_polygon_against_plane(p, FAR_FRUSTRUM_PLANE);
}


