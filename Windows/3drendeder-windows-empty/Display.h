#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define FPS 30
#define FRAME_TARGET_TIME (1000 / FPS)

SDL_Window* window;
SDL_Renderer* renderer;

uint32_t* colorBuffer;
SDL_Texture* colorBufferTexture;

int windowWidth;
int windowHeight;


bool initializeWindow(void);
void DrawPixel(int x, int y, uint32_t color);
void DrawGrid(void);
void DrawRectangle(int x, int y, int width, int height, uint32_t color);
void renderColorBuffer(void);
void clearColorBuffer(uint32_t color);
void destroyWindow(void);

#endif
