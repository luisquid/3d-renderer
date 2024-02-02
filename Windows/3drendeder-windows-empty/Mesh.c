#include <stdio.h>
#include <string.h>
#include "Array.h"
#include "Mesh.h"
#pragma warning(disable : 4996)

mesh_t mesh = {
    .vertices = NULL,
    .faces = NULL,
    .rotation = { 0, 0, 0 }
};

vec3_t cubeVertices[N_CUBE_VERTICES] = {
    {.x = -1, .y = -1, .z = -1 }, // 1
    {.x = -1, .y = 1, .z = -1 }, // 2
    {.x = 1, .y = 1, .z = -1 }, // 3
    {.x = 1, .y = -1, .z = -1 }, // 4
    {.x = 1, .y = 1, .z = 1 }, // 5
    {.x = 1, .y = -1, .z = 1 }, // 6
    {.x = -1, .y = 1, .z = 1 }, // 7
    {.x = -1, .y = -1, .z = 1 }  // 8
};

face_t cubeFaces[N_CUBE_FACES] = {
  // front
  {.a = 1, .b = 2, .c = 3 },
  {.a = 1, .b = 3, .c = 4 },
  // right
  {.a = 4, .b = 3, .c = 5 },
  {.a = 4, .b = 5, .c = 6 },
  // back
  {.a = 6, .b = 5, .c = 7 },
  {.a = 6, .b = 7, .c = 8 },
  // left
  {.a = 8, .b = 7, .c = 2 },
  {.a = 8, .b = 2, .c = 1 },
  // top
  {.a = 2, .b = 7, .c = 5 },
  {.a = 2, .b = 5, .c = 3 },
  // bottom
  {.a = 6, .b = 8, .c = 1 },
  {.a = 6, .b = 1, .c = 4 }
};

void LoadCubeMeshData(void) {
  for (int i = 0; i < N_CUBE_VERTICES; i++) {
    vec3_t cubeVertex = cubeVertices[i];
    array_push(mesh.vertices, cubeVertex);
  }
  for (int i = 0; i < N_CUBE_FACES; i++) {
    face_t cube_face = cubeFaces[i];
    array_push(mesh.faces, cube_face);
  }
}

void LoadObjFileData(char* filename) {
  FILE* file;
  file = fopen(filename, "r");
  char line[1024];

  while (fgets(line, 1024, file)) {
    // Vertex information
    if (strncmp(line, "v ", 2) == 0) {
      vec3_t vertex;
      sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
      array_push(mesh.vertices, vertex);
    }
    // Face information
    if (strncmp(line, "f ", 2) == 0) {
      int vertexIndices[3];
      int textureIndices[3];
      int normalIndices[3];
      sscanf(
        line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
        &vertexIndices[0], &textureIndices[0], &normalIndices[0],
        &vertexIndices[1], &textureIndices[1], &normalIndices[1],
        &vertexIndices[2], &textureIndices[2], &normalIndices[2]
      );
      face_t face = {
          .a = vertexIndices[0],
          .b = vertexIndices[1],
          .c = vertexIndices[2]
      };
      array_push(mesh.faces, face);
    }
  }
}
