#ifndef MESH_H
#define MESH_H
#include "Vector.h"
#include "Triangle.h"
#define N_MESH_VERTICES 8

extern vec3_t meshVertices[N_MESH_VERTICES];

#define N_MESH_FACES (6*2) //6 cube faces, 2 triangles per face
extern face_t meshFaces[N_MESH_FACES];

#endif // !MESH_H
