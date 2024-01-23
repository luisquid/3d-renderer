#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>


bool isRunning;

SDL_Window* window;
SDL_Renderer* renderer;


bool initializeWindow(void)
{
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    fprintf(stderr, "ERROR INITIALIZING SDL\n");
    return false;
  }
  
  window = SDL_CreateWindow(NULL, 
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED,
                            800,
                            600,
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

  return true;
}

void setup(void)
{

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

void render(void)
{ 
  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  SDL_RenderClear(renderer);

  SDL_RenderPresent(renderer);
}

int main(int argc, char* args[])
{
  isRunning = initializeWindow();

  setup();

  while (isRunning)
  {
    processInput();
    update();
    render();
  }

  return 0;
}