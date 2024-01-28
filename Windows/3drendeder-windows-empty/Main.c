#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "Display.h"
#include "vector.h"

#define N_POINTS (9 * 9 * 9)

vec3_t cameraPosition = {0, 0, -5};

vec3_t cubePoints[N_POINTS];
vec2_t projectedPoints[N_POINTS];

float fovFactor = 640;

bool isRunning;

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

  int pointCount = 0;

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
  }
  
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
  for (int i = 0; i < N_POINTS; i++)
  {
    vec3_t point = cubePoints[i];
    point.z -= cameraPosition.z;

    vec2_t projectedPoint = Project(point);
    projectedPoints[i] = projectedPoint;
  }
}

void render(void)
{ 
  /*SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  SDL_RenderClear(renderer);
  */
  DrawGrid();

  for (int i = 0; i < N_POINTS; i++)
  {
    vec2_t projectedPoint = projectedPoints[i];
    DrawRectangle(projectedPoint.x + (windowWidth / 2),
                  projectedPoint.y + (windowHeight / 2), 
                  4,
                  4, 
                  0xFFFFFF00
                  );
  }

  /*DrawPixel(20, 20, 0xFFFF0000);
  
  DrawRectangle(300, 150, 300, 200, 0xFFFF00FF);*/
  
  renderColorBuffer();

  clearColorBuffer(0xFF000000);
  
  SDL_RenderPresent(renderer);
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
  
  destroyWindow();
  return 0;
}