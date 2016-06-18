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
  int current_frame;
  int last_frame_update;
  int update_speed_ms;
  int rotation;
  SDL_RendererFlip flip;
} Sprite;

enum DIRECTION {
  UP,
  DOWN,
  LEFT,
  RIGHT
};

typedef struct Ghost {
  struct Sprite sprite;
  DIRECTION current_direction;
} Ghost;


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
static SDL_Texture *ghosttex;
static bool running;
static InputStatus input = {};
static Sprite* paclad;
static Ghost* funky;


int main (int argc, char* argv[])
{
  SDL_Init(SDL_INIT_VIDEO);

  win = SDL_CreateWindow(
      "Pac-Lad",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      640,
      480,
      SDL_INIT_VIDEO);

  ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  pactex = load_image_as_texture("./pac-sheet.bmp");
  ghosttex = load_image_as_texture("./ghost.bmp");

  paclad = (Sprite*)malloc(sizeof(Sprite));
  paclad->x = 40;
  paclad->y = 40;
  paclad->width = 50;
  paclad->height = 50;
  paclad->current_frame = 0;
  paclad->last_frame_update = SDL_GetTicks();
  paclad->update_speed_ms = 100;
  paclad->rotation = 0;
  paclad->flip = SDL_FLIP_NONE;

  funky = (Ghost*)malloc(sizeof(Ghost));
  funky->sprite.x = 100;
  funky->sprite.y = 100;
  funky->sprite.width = 50;
  funky->sprite.height = 50;
  funky->sprite.current_frame = 0;
  funky->sprite.last_frame_update = SDL_GetTicks();
  funky->sprite.update_speed_ms = 400;
  funky->sprite.rotation = 0;
  funky->sprite.flip = SDL_FLIP_NONE;
  funky->current_direction = DOWN;

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
    SDL_Delay(33);
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
  int win_h, win_w, t;
  SDL_GetWindowSize(win, &win_w, &win_h);
  t = SDL_GetTicks();

  if (input.up) {
    paclad->y -= 5;
    paclad->rotation = 270;
    paclad->flip = SDL_FLIP_NONE;
  } else if (input.down) {
    paclad->y += 5;
    paclad->rotation = 90;
    paclad->flip = SDL_FLIP_NONE;
  } else if (input.left) {
    paclad->x -= 5;
    paclad->rotation = 180;
    paclad->flip = SDL_FLIP_VERTICAL;
  } else if (input.right) {
    paclad->x += 5;
    paclad->rotation = 0;
    paclad->flip = SDL_FLIP_NONE;
  }

  if (paclad->x <= 0) paclad->x = 0;
  if (paclad->x + paclad->width >= win_w) paclad->x = win_w - paclad->width;
  if (paclad->y <= 0) paclad->y = 0;
  if (paclad->y + paclad->height >= win_h) paclad->y = win_h - paclad->height;

  if (input.up || input.down || input.left || input.right) {
    if (t - paclad->last_frame_update >= paclad->update_speed_ms) {
      paclad->current_frame = (paclad->current_frame + 1) % 4;
      paclad->last_frame_update = t;
    }
  }

  if (t - funky->sprite.last_frame_update >= funky->sprite.update_speed_ms) {
    funky->sprite.current_frame = (funky->sprite.current_frame + 1) % 2;
    funky->sprite.last_frame_update = t;
  }

  if (funky->sprite.y + funky->sprite.height >= win_h) {
    funky->sprite.y--;
    funky->current_direction = RIGHT;
  } else if (funky->sprite.x + funky->sprite.width >= win_w) {
    funky->sprite.x--;
    funky->current_direction = UP;
  } else if (funky->sprite.y <= 0) {
    funky->sprite.y++;
    funky->current_direction = LEFT;
  } else if (funky->sprite.x <= 0) {
    funky->sprite.x++;
    funky->current_direction = DOWN;
  }

  switch (funky->current_direction) {
    case UP:
      funky->sprite.y -= 5;
      break;

    case DOWN:
      funky->sprite.y += 5;
      break;

    case LEFT:
      funky->sprite.x -= 5;
      break;

    case RIGHT:
      funky->sprite.x += 5;
      break;
  }
}


static void render ()
{
  SDL_SetRenderDrawColor(ren, 40, 40, 40, 255);
  SDL_RenderClear(ren);

  // render paclad
  SDL_Rect frame = {0};
  frame.x = 50 * paclad->current_frame;
  frame.y = 0;
  frame.w = 50;
  frame.h = 50;

  SDL_Rect target = {0};
  target.x = paclad->x;
  target.y = paclad->y;
  target.w = paclad->width;
  target.h = paclad->height;

  SDL_RenderCopyEx(ren, pactex, &frame, &target, paclad->rotation, NULL, paclad->flip);

  // render funky
  frame.x = 50 * funky->sprite.current_frame;
  frame.y = 0;
  frame.w = 50;
  frame.h = 50;

  target.x = funky->sprite.x;
  target.y = funky->sprite.y;
  target.w = funky->sprite.width;
  target.h = funky->sprite.height;

  SDL_RenderCopyEx(ren, ghosttex, &frame, &target, funky->sprite.rotation, NULL, funky->sprite.flip);


  SDL_RenderPresent(ren);
}


static SDL_Texture* load_image_as_texture (const char* filename)
{
  SDL_Surface* img;
  SDL_Texture* tex;

  img = SDL_LoadBMP(filename);
  // fuchsia becomes a transparent color
  SDL_SetColorKey(img, 1, SDL_MapRGB(img->format, 0xFF, 0x00, 0x80));
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
