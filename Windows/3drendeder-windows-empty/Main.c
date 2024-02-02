#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "Array.h"
#include "Display.h"
#include "Vector.h"
#include "Mesh.h"

///////////////////////////////////////////////////////////////////////////////
// Array of triangles that should be rendered frame by frame
///////////////////////////////////////////////////////////////////////////////
triangle_t* trianglesToRender = NULL;

///////////////////////////////////////////////////////////////////////////////
// Global variables for execution status and game loop
///////////////////////////////////////////////////////////////////////////////
bool isRunning = false;
int previousFrameTime = 0;

vec3_t cameraPosition = { .x = 0, .y = 0, .z = -5 };
float fovFactor = 640;

///////////////////////////////////////////////////////////////////////////////
// Setup function to initialize variables and game objects
///////////////////////////////////////////////////////////////////////////////
void setup(void) {
  // Allocate the required memory in bytes to hold the color buffer
  colorBuffer = (uint32_t*)malloc(sizeof(uint32_t) * windowWidth * windowHeight);

  // Creating a SDL texture that is used to display the color buffer
  colorBufferTexture = SDL_CreateTexture(
    renderer,
    SDL_PIXELFORMAT_ARGB8888,
    SDL_TEXTUREACCESS_STREAMING,
    windowWidth,
    windowHeight
  );

  // Loads the vertex and face values for the mesh data structure
   //load_cube_mesh_data();
  LoadObjFileData("S:\\Github\\3drenderer\\Windows\\3drendeder-windows-empty\\f22.obj");
}

///////////////////////////////////////////////////////////////////////////////
// Poll system events and handle keyboard input
///////////////////////////////////////////////////////////////////////////////
void ProcessInput(void) {
  SDL_Event event;
  SDL_PollEvent(&event);
  switch (event.type) {
  case SDL_QUIT:
    isRunning = false;
    break;
  case SDL_KEYDOWN:
    if (event.key.keysym.sym == SDLK_ESCAPE)
      isRunning = false;
    break;
  }
}

////////////////////////////////////////////////////////////////////////////////
// Function that receives a 3D vector and returns a projected 2D point
////////////////////////////////////////////////////////////////////////////////
vec2_t project(vec3_t point) {
  vec2_t projectedPoints = {
      .x = (fovFactor * point.x) / point.z,
      .y = (fovFactor * point.y) / point.z
  };
  return projectedPoints;
}

///////////////////////////////////////////////////////////////////////////////
// Update function frame by frame with a fixed time step
///////////////////////////////////////////////////////////////////////////////
void update(void) {
  // Wait some time until the reach the target frame time in milliseconds
  int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previousFrameTime);

  // Only delay execution if we are running too fast
  if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
    SDL_Delay(time_to_wait);
  }

  previousFrameTime = SDL_GetTicks();

  // Initialize the array of triangles to render
  trianglesToRender = NULL;

  mesh.rotation.x += 0.01;
  mesh.rotation.y += 0.00;
  mesh.rotation.z += 0.00;

  // Loop all triangle faces of our mesh
  int numFaces = array_length(mesh.faces);
  for (int i = 0; i < numFaces; i++) {
    face_t meshFace = mesh.faces[i];

    vec3_t faceVertices[3];
    faceVertices[0] = mesh.vertices[meshFace.a - 1];
    faceVertices[1] = mesh.vertices[meshFace.b - 1];
    faceVertices[2] = mesh.vertices[meshFace.c - 1];

    triangle_t projectedTriangle;

    // Loop all three vertices of this current face and apply transformations
    for (int j = 0; j < 3; j++) {
      vec3_t transformedVertex = faceVertices[j];

      transformedVertex = Vec3RotateX(transformedVertex, mesh.rotation.x);
      transformedVertex = Vec3RotateY(transformedVertex, mesh.rotation.y);
      transformedVertex = Vec3RotateZ(transformedVertex, mesh.rotation.z);

      // Translate the vertex away from the camera
      transformedVertex.z -= cameraPosition.z;

      // Project the current vertex
      vec2_t projectedPoint = project(transformedVertex);

      // Scale and translate the projected points to the middle of the screen
      projectedPoint.x += (windowWidth / 2);
      projectedPoint.y += (windowHeight / 2);

      projectedTriangle.points[j] = projectedPoint;
    }

    // Save the projected triangle in the array of triangles to render
    array_push(trianglesToRender, projectedTriangle);
  }
}

///////////////////////////////////////////////////////////////////////////////
// Render function to draw objects on the display
///////////////////////////////////////////////////////////////////////////////
void render(void) {
  DrawGrid();

  // Loop all projected triangles and render them
  int num_triangles = array_length(trianglesToRender);
  for (int i = 0; i < num_triangles; i++) {
    triangle_t triangle = trianglesToRender[i];

    // Draw vertex points
    DrawRect(triangle.points[0].x, triangle.points[0].y, 3, 3, 0xFFFFFF00); // vertex A
    DrawRect(triangle.points[1].x, triangle.points[1].y, 3, 3, 0xFFFFFF00); // vertex B
    DrawRect(triangle.points[2].x, triangle.points[2].y, 3, 3, 0xFFFFFF00); // vertex C

    // Draw unfilled triangle
    DrawTriangle(
      triangle.points[0].x, triangle.points[0].y, // vertex A
      triangle.points[1].x, triangle.points[1].y, // vertex B
      triangle.points[2].x, triangle.points[2].y, // vertex C
      0xFF00FF00
    );
  }

  // Clear the array of triangles to render every frame loop
  array_free(trianglesToRender);

  RenderColorBuffer();

  ClearColorBuffer(0xFF000000);

  SDL_RenderPresent(renderer);
}

///////////////////////////////////////////////////////////////////////////////
// Free the memory that was dynamically allocated by the program
///////////////////////////////////////////////////////////////////////////////
void FreeResources(void) {
  free(colorBuffer);
  array_free(mesh.faces);
  array_free(mesh.vertices);
}

///////////////////////////////////////////////////////////////////////////////
// Main function
///////////////////////////////////////////////////////////////////////////////
int main(void) {
  isRunning = InitializeWindow();

  setup();

  while (isRunning) {
    ProcessInput();
    update();
    render();
  }

  DestroyWindow();
  FreeResources();

  return 0;
}
