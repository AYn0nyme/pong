#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <cstring>
#include <cmath>
#include <iostream>

int ballX = 256, ballY = 128, playerX, playerY, botX = 500, botY, playerScore, botScore;

int deltaX = -5, deltaY = -3, botDelta = 0;

bool colliding(int y1, int y2);

int main(void) {
  if(!SDL_Init(SDL_INIT_VIDEO)) {
    std::cout << strerror(errno) << std::endl;
    return 1;
  }

  SDL_Window* window = SDL_CreateWindow("Hi", 512, 256, SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP);

  if(!window) {
    std::cout << strerror(errno) << std::endl;
    return 1;
  }

  SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

  bool not_run;
  SDL_Event event;

  int now = SDL_GetTicks();
  int lastRender = now;
  while(!not_run) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            not_run = true;
        }
        if (event.type == SDL_EVENT_KEY_DOWN) {
          if (event.key.key == SDLK_DOWN) {
            playerY+=5;
          } else if (event.key.key == SDLK_UP) {
            playerY-=5;
          }
        }
    }
    now = SDL_GetTicks();
    
    SDL_Rect ballRect = {ballX, ballY, 12, 12};
    SDL_Rect playerRect = {playerX, playerY, 12, 32};
    SDL_Rect botRect = {botX, botY, 12, 32};

    
    
    if (now - lastRender >= 50) {
      if (SDL_HasRectIntersection(&ballRect, &playerRect) || SDL_HasRectIntersection(&ballRect, &botRect)) {
        deltaX *= -1;
      }
      
      if (ballY <= 0 || ballY >= 244) {
        deltaY *= -1;
      }
      if (ballX < 0) {
        ballX = 256;
        ballY = 128;
        botScore++;
        deltaX*=-1;
      } else if (ballX > 512) {
        ballX = 256;
        ballY = 128;
        playerScore++;
        deltaX*=-1;
      }
      ballX+=deltaX;
      ballY+=deltaY;
      if (botY != ballY) {
        if (ballY > botY) botDelta = 2;
        else botDelta = -2;
        botY+=botDelta;
      }
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      SDL_RenderClear(renderer);

      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

      SDL_FRect ball = {(float)ballX, (float)ballY, 12, 12};
      SDL_RenderFillRect(renderer, &ball);

      SDL_FRect player = {(float)playerX, (float)playerY, 12, 64};
      SDL_RenderFillRect(renderer, &player);

      SDL_FRect bot = {(float)botX, (float)botY, 12, 64};
      SDL_RenderFillRect(renderer, &bot);

      SDL_RenderDebugTextFormat(renderer, 128, 12, "%d", playerScore);
      SDL_RenderDebugTextFormat(renderer, 256+128, 12, "%d", botScore);

      lastRender = now;

      SDL_RenderPresent(renderer);
    }

    SDL_Delay(1);

  }
  
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}

bool colliding(int y1, int y2) {
  return sqrt((pow(y2, 2)-pow(y1, 2))) <= 1;
}
