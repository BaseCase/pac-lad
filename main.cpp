#include "vendor/SDL.h"


typedef struct InputStatus {
  int up;
  int down;
  int right;
  int left;
} InputStatus;


int main (int argc, char* argv[]);
static void do_game ();
static void render ();
static void handle_events ();
static void toggle_direction_key (SDL_KeyboardEvent event, int pressed);


static SDL_Window *win;
static SDL_Renderer *ren;
static bool running;
static InputStatus input = {};


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
  running = 1;

  while (running) {
    render();
    handle_events();
    SDL_Delay(10);
  }
}


static void render ()
{
  int r = 50, g = 50, b = 50, a = 255;

  if (input.up)
    r = 200;
  if (input.down)
    b = 200;
  if (input.right)
    g = 200;

  SDL_SetRenderDrawColor(ren, r, g, b, a);
  SDL_RenderClear(ren);
  SDL_RenderPresent(ren);
}


static void handle_events ()
{
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        running = 0;
        break;

      case SDL_KEYDOWN:
        toggle_direction_key(event.key, 1);
        break;

      case SDL_KEYUP:
        toggle_direction_key(event.key, 0);
        break;

      default:
        break;
    }
  }
}


static void toggle_direction_key (SDL_KeyboardEvent event, int pressed)
{
  switch (event.keysym.sym) {
    case SDLK_UP:
      input.up = pressed;
      break;

    case SDLK_DOWN:
      input.down = pressed;
      break;

    case SDLK_RIGHT:
      input.right = pressed;
      break;

    case SDLK_LEFT:
      input.left = pressed;
      break;
  }
}

