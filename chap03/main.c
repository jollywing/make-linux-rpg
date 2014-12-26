/* usage: gcc -o game main.c `sdl-config --cflags --libs` */
#include <stdio.h>
#include <SDL.h>

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

    SDL_Surface *temp = SDL_LoadBMP("card.bmp");
    if(temp == NULL) {
        printf("Load bmp image failed!\n");
        exit(1);
    }

    SDL_Surface *card_surface = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);

    SDL_Rect dest_rect = {0, 0, card_surface->w, card_surface->h};
    SDL_BlitSurface(card_surface, NULL, screen, &dest_rect);
    // DO NOT FORGET!
    SDL_Flip(screen);

    SDL_FreeSurface(card_surface);
    /* To pause the program */
    while(1){
        SDL_Delay(100);
    }
    return 0;
}
