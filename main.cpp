#include <stdbool.h>
#include "vendor/SDL.h"


typedef struct InputStatus {
  bool up;
  bool down;
  bool left;
  bool right;
} InputStatus;


typedef struct Sprite {
  int x;
  int y;
  int width;
  int height;
} Sprite;


int main (int argc, char* argv[]);
static void do_game ();
static void handle_events ();
static void update_universe ();
static void render ();
static SDL_Texture* load_image_as_texture (const char* filename);
static void toggle_key (SDL_KeyboardEvent event, bool is_pressed);


static SDL_Window *win;
static SDL_Renderer *ren;
static SDL_Texture *pactex;
static bool running;
static InputStatus input = {};
static Sprite* pacman;


int main (int argc, char* argv[])
{
  SDL_Init(SDL_INIT_VIDEO);

  win = SDL_CreateWindow(
      "Wow it's so good",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      640,
      480,
      SDL_INIT_VIDEO);

  ren = SDL_CreateRenderer(win, -1, 0);
  pactex = load_image_as_texture("./pac_man.bmp");

  pacman = (Sprite*)malloc(sizeof(Sprite));
  pacman->x = 40;
  pacman->y = 40;
  pacman->width = 30;
  pacman->height = 30;

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
    handle_events();
    update_universe();
    render();
    SDL_Delay(10);
  }
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
        toggle_key(event.key, true);
        break;

      case SDL_KEYUP:
        toggle_key(event.key, false);
        break;

      default:
        break;
    }
  }
}


static void update_universe ()
{
  if (input.up) pacman->y--;
  if (input.down) pacman->y++;
  if (input.left) pacman->x--;
  if (input.right) pacman->x++;
}


static void render ()
{
  SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
  SDL_RenderClear(ren);
  SDL_Rect r = {0};
  r.x = pacman->x;
  r.y = pacman->y;
  r.w = pacman->width;
  r.h = pacman->height;
  SDL_RenderCopy(ren, pactex, NULL, &r);
  SDL_RenderPresent(ren);
}


static SDL_Texture* load_image_as_texture (const char* filename)
{
  SDL_Surface* img;
  SDL_Texture* tex;

  img = SDL_LoadBMP(filename);
  tex = SDL_CreateTextureFromSurface(ren, img);
  SDL_FreeSurface(img);

  return tex;
}


static void toggle_key (SDL_KeyboardEvent event, bool is_pressed)
{
  switch (event.keysym.sym) {
    case SDLK_UP:
      input.up = is_pressed;
      break;

    case SDLK_DOWN:
      input.down = is_pressed;
      break;

    case SDLK_LEFT:
      input.left = is_pressed;
      break;

    case SDLK_RIGHT:
      input.right = is_pressed;
      break;
  }
}

