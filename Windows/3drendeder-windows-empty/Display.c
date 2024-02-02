#include "display.h"
#include <stdio.h>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
uint32_t* colorBuffer = NULL;
SDL_Texture* colorBufferTexture = NULL;
int windowWidth = 800;
int windowHeight = 600;

bool InitializeWindow(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL.\n");
    return false;
  }

  // Set width and height of the SDL window with the max screen resolution
  SDL_DisplayMode displayMode;
  SDL_GetCurrentDisplayMode(0, &displayMode);
  windowWidth = displayMode.w;
  windowHeight = displayMode.h;

  // Create a SDL Window
  window = SDL_CreateWindow(
    NULL,
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    windowWidth,
    windowHeight,
    SDL_WINDOW_BORDERLESS
  );
  if (!window) {
    fprintf(stderr, "Error creating SDL window.\n");
    return false;
  }

  // Create a SDL renderer
  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    fprintf(stderr, "Error creating SDL renderer.\n");
    return false;
  }

  return true;
}

void DrawGrid(void) {
  for (int y = 0; y < windowHeight; y += 10) {
    for (int x = 0; x < windowWidth; x += 10) {
      colorBuffer[(windowWidth * y) + x] = 0xFF444444;
    }
  }
}

void DrawPixel(int x, int y, uint32_t color) {
  if (x >= 0 && x < windowWidth && y >= 0 && y < windowHeight) {
    colorBuffer[(windowWidth * y) + x] = color;
  }
}

void DrawLine(int x0, int y0, int x1, int y1, uint32_t color) {
  int deltaX = (x1 - x0);
  int deltaY = (y1 - y0);

  int sideLength = (abs(deltaX) >= abs(deltaY)) ? abs(deltaX) : abs(deltaY);

  float xInc = deltaX / (float)sideLength;
  float yInc = deltaY / (float)sideLength;

  float currentX = x0;
  float currentY = y0;
  for (int i = 0; i <= sideLength; i++) {
    DrawPixel(round(currentX), round(currentY), color);
    currentX += xInc;
    currentY += yInc;
  }
}

void DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
  DrawLine(x0, y0, x1, y1, color);
  DrawLine(x1, y1, x2, y2, color);
  DrawLine(x2, y2, x0, y0, color);
}

void DrawRect(int x, int y, int width, int height, uint32_t color) {
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      int currentX = x + i;
      int currentY = y + j;
      DrawPixel(currentX, currentY, color);
    }
  }
}

void RenderColorBuffer(void) {
  SDL_UpdateTexture(
    colorBufferTexture,
    NULL,
    colorBuffer,
    (int)(windowWidth * sizeof(uint32_t))
  );
  SDL_RenderCopy(renderer, colorBufferTexture, NULL, NULL);
}

void ClearColorBuffer(uint32_t color) {
  for (int y = 0; y < windowHeight; y++) {
    for (int x = 0; x < windowWidth; x++) {
      colorBuffer[(windowWidth * y) + x] = color;
    }
  }
}

void DestroyWindow(void) {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
