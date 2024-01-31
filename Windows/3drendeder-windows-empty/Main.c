#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "Array.h"
#include "Display.h"
#include "Vector.h"
#include "Mesh.h"

//#define N_POINTS (9 * 9 * 9)

vec3_t cameraPosition = {0, 0, -5};
//vec3_t cubeRotation = {0, 0, 0};

triangle_t* trianglesToRender = NULL;

//triangle_t trianglesToRender [N_MESH_FACES];

//vec3_t cubePoints[N_POINTS];
//vec2_t projectedPoints[N_POINTS];

float fovFactor = 640;

bool isRunning;
int previousFrameTime = 0;

bool setup(void)
{
  colorBuffer = (uint32_t*)malloc(sizeof(uint32_t) * windowWidth * windowHeight);
  colorBufferTexture = SDL_CreateTexture(renderer, 
                                         SDL_PIXELFORMAT_ARGB8888,
                                         SDL_TEXTUREACCESS_STREAMING,
                                         windowWidth,
                                         windowHeight);
  if (!colorBuffer)
  {
    fprintf(stderr, "ERROR SETTING UP COLOR BUFFER");
    return false;
  }
  
  LoadCubeMeshData();
  /*int pointCount = 0;

  for (float x = -1; x <= 1; x += 0.25)
  {
    for (float y = -1; y <= 1; y += 0.25)
    {
      for (float z = -1; z <= 1; z += 0.25)
      {
        vec3_t newPoint = {.x = x, .y = y, .z = z};
        cubePoints[pointCount++] = newPoint;
      }
    }
  }*/
  
  return true;
}

void processInput(void)
{
  SDL_Event event;
  SDL_PollEvent(&event);
  
  switch (event.type)
  {
    case SDL_QUIT:
        isRunning = false;
      break;
    case SDL_KEYDOWN:
      if(event.key.keysym.sym == SDLK_ESCAPE)
        isRunning = false;
      break;
    default:
      break;
  }
}

vec2_t Project(vec3_t point)
{
  vec2_t projectedPoint = {
    .x = (fovFactor * point.x) / point.z,
    .y = (fovFactor * point.y) / point.z
  };

  return projectedPoint;
}

void update(void)
{
  int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - previousFrameTime);

  if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME)
  {
    SDL_Delay(timeToWait);
  }

  //while(!SDL_TICKS_PASSED(SDL_GetTicks(), previousFrameTime + FRAME_TARGET_TIME));

  previousFrameTime = SDL_GetTicks();
  
  trianglesToRender = NULL;  
  
  mesh.rotation.x += 0.01;
  mesh.rotation.y += 0.01;
  mesh.rotation.z += 0.01;
  
  int numFaces = ArrayLength(mesh.faces);
  for (int i = 0; i < numFaces; i++)
  {
    face_t meshFace = mesh.faces[i];
    
    vec3_t faceVertices[3];
    faceVertices[0] = mesh.vertices[meshFace.a - 1];
    faceVertices[1] = mesh.vertices[meshFace.b - 1];
    faceVertices[2] = cubeVertices[meshFace.c - 1];
    
    triangle_t projectedTriangle;

    for (int j = 0; j < 3; j++)
    {
        vec3_t transfomedVertex = faceVertices[j];
        transfomedVertex = Vec3RotateX(transfomedVertex, mesh.rotation.x);
        transfomedVertex = Vec3RotateY(transfomedVertex, mesh.rotation.y);
        transfomedVertex = Vec3RotateZ(transfomedVertex, mesh.rotation.z);
        
        transfomedVertex.z -= cameraPosition.z;

        vec2_t projectedPoint = Project(transfomedVertex);

        projectedPoint.x += (windowWidth / 2);
        projectedPoint.y += (windowHeight / 2);

        projectedTriangle.points [j] = projectedPoint;
    }
    
    //Save the projected triangles to render
    //trianglesToRender[i] = projectedTriangle;
    ArrayPush(trianglesToRender, projectedTriangle);
  }

  /*for (int i = 0; i < N_POINTS; i++)
  {
    vec3_t point = cubePoints[i];

    vec3_t transformedPoint = Vec3RotateX(point, cubeRotation.x);
    transformedPoint = Vec3RotateY(transformedPoint, cubeRotation.y);
    transformedPoint = Vec3RotateZ(transformedPoint, cubeRotation.z);
  
    transformedPoint.z -= cameraPosition.z;

    vec2_t projectedPoint = Project(transformedPoint);
    projectedPoints[i] = projectedPoint;
  }*/
}

void render(void)
{ 
  /*SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  SDL_RenderClear(renderer);
  */
  DrawGrid();

  int meshTrianglesSize = ArrayLength(trianglesToRender);

  for (int i = 0; i < meshTrianglesSize; i++)
  {
    triangle_t triangle = trianglesToRender[i];
    DrawRectangle(triangle.points[0].x, triangle.points[0].y, 3, 3, 0xFFFFFF00);
    DrawRectangle(triangle.points[1].x, triangle.points[1].y, 3, 3, 0xFFFFFF00);
    DrawRectangle(triangle.points[2].x, triangle.points[2].y, 3, 3, 0xFFFFFF00);

    DrawTriangle(triangle.points[0].x, 
                 triangle.points[0].y, 
                 triangle.points[1].x, 
                 triangle.points[1].y, 
                 triangle.points[2].x, 
                 triangle.points[2].y, 
                 0xFF00FF00);
  }

  /*for (int i = 0; i < N_POINTS; i++)
  {
    vec2_t projectedPoint = projectedPoints[i];
    DrawRectangle(projectedPoint.x + (windowWidth / 2),
                  projectedPoint.y + (windowHeight / 2), 
                  4,
                  4, 
                  0xFFFFFF00
                  );
  }*/

  /*DrawPixel(20, 20, 0xFFFF0000);
  
  DrawRectangle(300, 150, 300, 200, 0xFFFF00FF);*/
  
  ArrayFree(trianglesToRender);
  
  renderColorBuffer();

  clearColorBuffer(0xFF000000);
  
  SDL_RenderPresent(renderer);
}


void FreeResources(void)
{
  free(colorBuffer);
  ArrayFree(mesh.faces);
  ArrayFree(mesh.vertices);
}

int main(int argc, char* args[])
{
  isRunning = initializeWindow();
  
  setup();
  
  vec3_t myVector = { 1.0, 2.5, 3.0 };

  while (isRunning)
  {
    processInput();
    update();
    render();
  }
  
  DestroyWindow();
  FreeResources();
  return 0;
}