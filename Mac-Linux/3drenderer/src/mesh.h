#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"
#include "upng.h"

////////////////////////////////////////////////////////////////////////////////
// Define a struct for dynamic size meshes, with array of vertices and faces
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    vec3_t* vertices; // dynamic array of vertices
    face_t* faces;    // dynamic array of faces
    upng_t* texture;  // mesh PNG Texture pointer
    vec3_t rotation;  // rotation with x, y, and z values
    vec3_t scale;
    vec3_t translation;
} mesh_t;
int get_num_meshes(void);
void load_mesh(char* mesh_filename, char* texture_filename, vec3_t scale, vec3_t position, vec3_t rotation);
void load_mesh_obj_data(mesh_t* mesh, char* mesh_filename);
void load_mesh_texture_data(mesh_t* mesh, char* texture_filename);

mesh_t* get_mesh(int index);
void free_meshes(void);
#endif
