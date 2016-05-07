#include "vendor/SDL.h"


typedef struct InputStatus {
  int red;
  int green;
  int blue;
} InputStatus;


int main (int argc, char* argv[]);
static void do_game ();
static void render ();
static void handle_events ();
static void toggle_direction_key (SDL_KeyboardEvent event, int is_pressed);


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

  if (input.red)
    r = 200;
  if (input.green)
    g = 200;
  if (input.blue)
    b = 200;

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


static void toggle_direction_key (SDL_KeyboardEvent event, int is_pressed)
{
  switch (event.keysym.sym) {
    case SDLK_r:
      input.red = is_pressed;
      break;

    case SDLK_g:
      input.green = is_pressed;
      break;

    case SDLK_b:
      input.blue = is_pressed;
      break;
  }
}

