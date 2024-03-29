#ifndef MESH_H
#define MESH_H

#include "Vector.h"
#include "Triangle.h"

#define N_CUBE_VERTICES 8
#define N_CUBE_FACES (6 * 2) // 6 cube faces, 2 triangles per face

extern vec3_t cubeVertices[N_CUBE_VERTICES];
extern face_t cubeFaces[N_CUBE_FACES];

////////////////////////////////////////////////////////////////////////////////
// Define a struct for dynamic size meshes, with array of vertices and faces
////////////////////////////////////////////////////////////////////////////////
typedef struct {
  vec3_t* vertices; // dynamic array of vertices
  face_t* faces;    // dynamic array of faces
  vec3_t rotation;  // rotation with x, y, and z values
} mesh_t;

extern mesh_t mesh;

void LoadCubeMeshData(void);
void LoadObjFileData(char* filename);

#endif
