#include <stdio.h>
#include "vendor/SDL.h"


int main (int argc, char* argv[]);
static void do_game ();
static void render ();
static void handle_events ();


static SDL_Window *win;
static SDL_Renderer *ren;
static bool running;


int main (int argc, char* argv[])
{
  SDL_Init(SDL_INIT_VIDEO);

  win = SDL_CreateWindow(
      "Wow it's so good",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      640,
      480,
      SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  ren = SDL_CreateRenderer(win, -1, 0);

  do_game();

  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();

  return 0;
}


static void do_game ()
{
  running = true;

  while (running) {
    render();
    handle_events();
    SDL_Delay(10);
  }
}


static void render ()
{
  SDL_SetRenderDrawColor(ren, 50, 50, 50, 255);
  SDL_RenderClear(ren);
  SDL_RenderPresent(ren);
}


static void handle_events ()
{
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        running = false;
        break;

      default:
        break;
    }
  }
}

