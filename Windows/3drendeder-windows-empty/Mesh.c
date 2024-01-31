#include <stdint.h>
#include "Mesh.h"

mesh_t mesh = {
  .vertices = NULL,
  .faces = NULL,
  .rotation = {0,0,0}
};

vec3_t cubeVertices [N_CUBE_VERTICES]= {
  {-1,-1,-1},
  {-1,1,-1},
  {1, 1,-1},
  {1, -1,-1},
  {1, 1,1},
  {1, -1, 1},
  {-1, 1, 1},
  {-1, -1, 1}
};

face_t cubeFaces [N_CUBE_FACES] = {
  //Front 
  {1, 2, 3},
  {1, 3, 4},

  //Right 
  {4, 3, 5},
  {4, 5, 6},

  //Back 
  {6, 5, 7},
  {6, 7, 8},

  //Left 
  {8, 7, 2},
  {8, 2, 1},

  //Top 
  {2, 7, 5},
  {2, 5, 3},

  //Bottom 
  {6, 8, 1},
  {6, 1, 4},
};
 
void LoadCubeMeshData(void)
{
  for (int i = 0; i < N_CUBE_VERTICES; i++)
  {
    vec3_t cubeVertex = cubeVertices[i];
    ArrayPush(mesh.vertices, cubeVertex);
  }

  for (int i = 0; i < N_CUBE_FACES; i++)
  {
    face_t cubeFace = cubeFaces[i];
    ArrayPush(mesh.faces, cubeFace);
  }
}