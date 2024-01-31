#include "Display.h"

extern SDL_Window* window = NULL;
extern SDL_Renderer* renderer = NULL;
extern uint32_t* colorBuffer = NULL;
extern SDL_Texture* colorBufferTexture = NULL;
extern int windowWidth = 800;
extern int windowHeight = 600;

bool initializeWindow(void)
{
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    fprintf(stderr, "ERROR INITIALIZING SDL\n");
    return false;
  }

  SDL_DisplayMode displayMode;
  SDL_GetCurrentDisplayMode(0, &displayMode);

  windowWidth = displayMode.w;
  windowHeight = displayMode.h;

  window = SDL_CreateWindow(
    NULL,
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    windowWidth,
    windowHeight,
    SDL_WINDOW_BORDERLESS
  );

  if (!window)
  {
    fprintf(stderr, "ERROR CREATING SDL WINDOW\n");
    return false;
  }

  renderer = SDL_CreateRenderer(window, -1, 0);

  if (!renderer)
  {
    fprintf(stderr, "ERROR CREATING SDL RENDERER\n");
    return false;
  }

  //SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
  return true;
}

void DrawPixel(int x, int y, uint32_t color)
{
  if (x >= 0 && x < windowWidth && y >= 0 && y < windowHeight)
  {
    colorBuffer[(windowWidth * y) + x] = color;
  }
}

void DrawLine(int x0, int y0, int x1, int y1, uint32_t color)
{
  int deltaX = (x1 - x0);
  int deltaY = (y1 - y0);

  int sideLength = (abs(deltaX) >= abs(deltaY)) ? abs(deltaX) : abs(deltaY);

  float xInc = deltaX / (float)sideLength;
  float yInc = deltaY / (float)sideLength;

  float currentX = x0;
  float currentY = y0;

  for (int i = 0; i < sideLength; i++)
  {
    DrawPixel(round(currentX), round(currentY), color);
    currentX += xInc;
    currentY += yInc;
  }
}

void DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
  DrawLine(x0, y0, x1, y1, color);
  DrawLine(x1, y1, x2, y2, color);
  DrawLine(x2, y2, x0, y0, color);
}

void DrawGrid(void)
{
  for (int y = 0; y < windowHeight; y += 10)
  {
    for (int x = 0; x < windowWidth; x += 10)
    {
      //if(y % 10 == 0 || x%10 == 0)
      colorBuffer[(windowWidth * y) + x] = 0xFF444444;
    }
  }
}

void DrawRectangle(int x, int y, int width, int height, uint32_t color)
{
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      int currentX = x + j;
      int currentY = y + i;
      //colorBuffer[(windowWidth * i) + j] = color;
      DrawPixel(currentX, currentY, color);
    }
  }
}

void renderColorBuffer(void)
{
  SDL_UpdateTexture(colorBufferTexture, NULL, colorBuffer, (int)(windowWidth * sizeof(uint32_t)));
  SDL_RenderCopy(renderer, colorBufferTexture, NULL, NULL);
}

void clearColorBuffer(uint32_t color)
{
  for (int y = 0; y < windowHeight; y++)
  {
    for (int x = 0; x < windowWidth; x++)
    {
      colorBuffer[(windowWidth * y) + x] = color;
    }
  }
}

void DestroyWindow(void)
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}