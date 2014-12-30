/* usage: gcc -o game main.c `sdl-config --cflags --libs` */
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>

typedef unsigned char bool;
#define TRUE 1
#define FALSE 0

#define ANIM_COUNT 3
#define FRAME_WIDTH 200
#define FRAME_HEIGHT 125

typedef struct {
    SDL_Rect src_rect;
    SDL_Rect dest_rect;
    SDL_Surface *src_surface;
    size_t current_frame;
    size_t frame_count;
} Sprite;

void update_sprite(Sprite *sp)
{
    sp->current_frame ++;
    if (sp->current_frame == sp->frame_count){
        sp->current_frame = 0;
    }

    sp->src_rect.x = FRAME_WIDTH * sp->current_frame;
    sp->src_rect.y = 0;
    sp->src_rect.w = FRAME_WIDTH;
    sp->src_rect.h = FRAME_HEIGHT;
}

void set_rect(SDL_Rect *r, int x, int y, int w, int h)
{
    r->x = x;
    r->y = y;
    r->w = w;
    r->h = h;
}

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
    atexit(SDL_Quit);

    SDL_Surface *screen = SDL_SetVideoMode(320, 480, 0, SDL_HWSURFACE|SDL_DOUBLEBUF);

    if (screen == NULL) {
        printf("Unable to set video mode: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_WM_SetCaption("Hello, Linux Game!", NULL);

    /* Load PNG and Display */
    SDL_Surface *temp = IMG_Load("200x125-skill.png");
    SDL_Surface *card_surface = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);
    SDL_SetColorKey(card_surface, SDL_SRCCOLORKEY|SDL_RLEACCEL, 0);

    Sprite sp[3];
    set_rect(&sp[0].dest_rect, (screen->w - FRAME_WIDTH)/2, 0, FRAME_WIDTH,
            FRAME_HEIGHT);
    sp[0].src_surface = card_surface;
    sp[0].frame_count = 7;
    sp[0].current_frame = 2;

    set_rect(&sp[1].dest_rect, (screen->w - FRAME_WIDTH)/2,
            (screen->h - FRAME_HEIGHT)/2, FRAME_WIDTH, FRAME_HEIGHT);
    sp[1].src_surface = card_surface;
    sp[1].frame_count = 7;
    sp[1].current_frame = 3;

    set_rect(&sp[2].dest_rect, (screen->w - FRAME_WIDTH)/2,
            (screen->h - FRAME_HEIGHT), FRAME_WIDTH, FRAME_HEIGHT);
    sp[2].src_surface = card_surface;
    sp[2].frame_count = 7;
    sp[2].current_frame = 4;

    int i;

    bool running = TRUE;
    SDL_Event event;
    /* To pause the program */
    while(running){
        SDL_PollEvent(&event);
        switch(event.type){
        case SDL_QUIT:
            running = FALSE;
            break;
        case SDL_KEYUP:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                running = FALSE;
        }
        /* clear screen, Fill screen with some color */
        SDL_FillRect(screen, NULL, 0xff009900);
        for(i = 0; i < ANIM_COUNT; i ++){
            update_sprite(&sp[i]);
            SDL_BlitSurface(sp[i].src_surface, &sp[i].src_rect, screen,
                    &sp[i].dest_rect);
        }
        SDL_Flip(screen);
        SDL_Delay(150);
    }

    SDL_FreeSurface(card_surface);
    printf("Now, I will Quit!\n");
    return 0;
}
