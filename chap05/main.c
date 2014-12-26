/* usage: gcc -o game main.c `sdl-config --cflags --libs` */
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>

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

    SDL_Rect src_rect;
    src_rect.w = 200;
    src_rect.h = 125;
    src_rect.y = 0;
    SDL_Rect dest_rect = {(screen->w - 200)/2, (screen->h - 125)/2, 200, 125};

    int i = 0;

    /* To pause the program */
    while(1){
        /* clear screen, Fill screen with some color */
        SDL_FillRect(screen, NULL, 0xff009900);
        /* display pic */
        src_rect.x = i * src_rect.w;
        SDL_BlitSurface(card_surface, &src_rect, screen, &dest_rect);
        SDL_Flip(screen);
        i ++;
        if(i==7) i = 0;
        SDL_Delay(150);
    }

    SDL_FreeSurface(card_surface);
    printf("Now, I will Quit!\n");
    return 0;
}
