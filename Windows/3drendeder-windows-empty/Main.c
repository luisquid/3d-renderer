#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>


bool isRunning;

SDL_Window* window;
SDL_Renderer* renderer;

uint32_t* colorBuffer = NULL;
SDL_Texture* colorBufferTexture = NULL;

int windowWidth = 800;
int windowHeight = 600;

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

  window = SDL_CreateWindow(NULL, 
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED,
                            windowWidth,
                            windowHeight,
                            SDL_WINDOW_BORDERLESS);
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

  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
  return true;
}

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


void update(void)
{

}

void DrawGrid(void)
{
  for (int y = 0; y < windowHeight; y+= 10)
  {
    for (int x = 0; x < windowWidth; x+=10)
    {
      //if(y % 10 == 0 || x%10 == 0)
        colorBuffer[(windowWidth * y) + x] = 0xFFFFFF00;
    }
  }
}

void DrawRectangle(int width, int height, int x, int y, uint32_t color)
{
  for (int i = y; i < height + y; i++)
  {
    for (int j = x; j < width + x; j++)
    {
      colorBuffer[(windowWidth * i) + j] = color;
    }
  }
}

void renderColorBuffer(void)
{
  SDL_UpdateTexture(colorBufferTexture, NULL, colorBuffer, (int)(windowWidth *sizeof(uint32_t)));
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

void render(void)
{ 
  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  SDL_RenderClear(renderer);

  DrawGrid();
  DrawRectangle(300, 150, 300, 200, 0xFFFF00FF);
  renderColorBuffer();
  clearColorBuffer(0x00000000);

  
  SDL_RenderPresent(renderer);
}

void destroyWindow(void)
{
  free(colorBuffer);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int main(int argc, char* args[])
{
  isRunning = initializeWindow();
  
  isRunning = setup();

  while (isRunning)
  {
    processInput();
    update();
    render();
  }
  
  destroyWindow();
  return 0;
}