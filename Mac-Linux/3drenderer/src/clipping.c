#include <math.h>
#include "clipping.h"

#define NUM_PLANES 6 
plane_t frustum_planes [NUM_PLANES];

void initialize_frustum_planes(float fovx, float fovy, float z_near, float z_far){
    float cos_half_povx = cos(fovx/2);
    float sin_half_povx = sin(fovx/2);

    float cos_half_povy = cos(fovy/2);
    float sin_half_povy = sin(fovy/2);

    frustum_planes[LEFT_FRUSTRUM_PLANE].point = vec3_new(0, 0, 0);
    frustum_planes[LEFT_FRUSTRUM_PLANE].normal.x = cos_half_povx;
    frustum_planes[LEFT_FRUSTRUM_PLANE].normal.y = 0;
    frustum_planes[LEFT_FRUSTRUM_PLANE].normal.z = sin_half_povx;

    frustum_planes[RIGHT_FRUSTRUM_PLANE].point = vec3_new(0, 0, 0);;
    frustum_planes[RIGHT_FRUSTRUM_PLANE].normal.x = -cos_half_povx;
    frustum_planes[RIGHT_FRUSTRUM_PLANE].normal.y = 0;
    frustum_planes[RIGHT_FRUSTRUM_PLANE].normal.z = sin_half_povx;

    frustum_planes[TOP_FRUSTRUM_PLANE].point = vec3_new(0, 0, 0);;
    frustum_planes[TOP_FRUSTRUM_PLANE].normal.x = 0;
    frustum_planes[TOP_FRUSTRUM_PLANE].normal.y = -cos_half_povy;
    frustum_planes[TOP_FRUSTRUM_PLANE].normal.z = sin_half_povy;

    frustum_planes[BOTTOM_FRUSTRUM_PLANE].point = vec3_new(0, 0, 0);;
    frustum_planes[BOTTOM_FRUSTRUM_PLANE].normal.x = 0;
    frustum_planes[BOTTOM_FRUSTRUM_PLANE].normal.y = cos_half_povy;
    frustum_planes[BOTTOM_FRUSTRUM_PLANE].normal.z = sin_half_povy;

    frustum_planes[NEAR_FRUSTRUM_PLANE].point = vec3_new(0, 0, z_near);
    frustum_planes[NEAR_FRUSTRUM_PLANE].normal.x = 0;
    frustum_planes[NEAR_FRUSTRUM_PLANE].normal.y = 0;
    frustum_planes[NEAR_FRUSTRUM_PLANE].normal.z = 1;

    frustum_planes[FAR_FRUSTRUM_PLANE].point = vec3_new(0, 0, z_far);
    frustum_planes[FAR_FRUSTRUM_PLANE].normal.x = 0;
    frustum_planes[FAR_FRUSTRUM_PLANE].normal.y = 0;
    frustum_planes[FAR_FRUSTRUM_PLANE].normal.z = -1;
}

polygon_t create_polygon_from_triangle(vec3_t v0, vec3_t v1, vec3_t v2, tex2_t t0, tex2_t t1, tex2_t t2){
    polygon_t result = {
        .vertices = {v0, v1, v2},
        .texcoords = {t0, t1, t2},
        .num_vertices = 3
    };

    return result;
}

void triangles_from_polygon(polygon_t * polygon, triangle_t triangles[], int* num_triangles){
    for (int i = 0; i < polygon->num_vertices - 2; i++)
    {
        int index0 = 0;
        int index1 = i + 1;
        int index2 = i + 2;

        triangles[i].points[0] = vec4_from_vec3(polygon->vertices[index0]);
        triangles[i].points[1] = vec4_from_vec3(polygon->vertices[index1]);
        triangles[i].points[2] = vec4_from_vec3(polygon->vertices[index2]);

        triangles[i].texcoords[0] = polygon->texcoords[index0];
        triangles[i].texcoords[1] = polygon->texcoords[index1];
        triangles[i].texcoords[2] = polygon->texcoords[index2];
    }
    
    *num_triangles = polygon->num_vertices - 2;
}

float float_lerp(float a, float b, float t){
    return a + t * (b - a);
}
void clip_polygon_against_plane(polygon_t * p, int plane){
    vec3_t plane_point = frustum_planes[plane].point;
    vec3_t plane_normal = frustum_planes[plane].normal;

    vec3_t inside_vertices[MAX_NUM_POLY_VERTICES];
    tex2_t inside_texcoords[MAX_NUM_POLY_VERTICES];

    int num_inside_vertices = 0;

    vec3_t * current_vertex = &p->vertices[0];
    tex2_t* current_texcoord = &p->texcoords[0];

    vec3_t * previous_vertex = &p->vertices[p->num_vertices - 1];
    tex2_t * previous_texcoord = &p->texcoords[p->num_vertices - 1];

    float current_dot = 0;
    float previous_dot = vec3_dot(vec3_substract(*previous_vertex, plane_point), plane_normal);

    while(current_vertex != &p->vertices[p->num_vertices]){
        current_dot = vec3_dot(vec3_substract(*current_vertex, plane_point), plane_normal);

        if(current_dot * previous_dot < 0){
            float t = previous_dot / (previous_dot - current_dot);
            vec3_t intersection_point = {
                .x = float_lerp(previous_vertex->x, current_vertex->x, t),
                .y = float_lerp(previous_vertex->y, current_vertex->y, t),
                .z = float_lerp(previous_vertex->z, current_vertex->z, t)
            };
            //vec3_clone(current_vertex);
            // intersection_point = vec3_substract(intersection_point, *previous_vertex);
            // intersection_point = vec3_mul(intersection_point, t);
            // intersection_point = vec3_add(intersection_point, *previous_vertex);

            tex2_t interpolated_texcoord = {
                .u = float_lerp(previous_texcoord->u, current_texcoord->u, t), 
                .v = float_lerp(previous_texcoord->v, current_texcoord->v, t)
            };

            inside_vertices[num_inside_vertices] = vec3_clone(&intersection_point);
            inside_texcoords[num_inside_vertices] = tex2_clone(&interpolated_texcoord);
            num_inside_vertices++;
        }

        if(current_dot > 0){
            inside_vertices[num_inside_vertices] = vec3_clone(current_vertex);
            inside_texcoords[num_inside_vertices] = tex2_clone(current_texcoord);
            num_inside_vertices++;
        }

        previous_dot = current_dot;
        previous_vertex = current_vertex;
        previous_texcoord = current_texcoord;
        current_texcoord++;
        current_vertex++;
    }

    for (int i = 0; i < num_inside_vertices; i++)
    {
        p->vertices[i] = vec3_clone(&inside_vertices[i]);
        p->texcoords[i] = tex2_clone(&inside_texcoords[i]);
    }

    p->num_vertices = num_inside_vertices; 
}

void clip_polygon(polygon_t * p){
    clip_polygon_against_plane(p, LEFT_FRUSTRUM_PLANE);
    clip_polygon_against_plane(p, RIGHT_FRUSTRUM_PLANE);
    clip_polygon_against_plane(p, TOP_FRUSTRUM_PLANE);
    clip_polygon_against_plane(p, BOTTOM_FRUSTRUM_PLANE);
    clip_polygon_against_plane(p, NEAR_FRUSTRUM_PLANE);
    clip_polygon_against_plane(p, FAR_FRUSTRUM_PLANE);
}


