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

    SDL_Surface *temp = SDL_LoadBMP("card.bmp");
    SDL_Rect dest_rect = {0, 0, temp->w, temp->h};
    SDL_BlitSurface(card_surface, NULL, screen, &dest_rect);
    // DO NOT FORGET!
    SDL_Flip(screen);

    /* Load and Display a image */
    // SDL_Surface *temp = SDL_LoadBMP("card.bmp");
    // SDL_Surface *card_surface = SDL_DisplayFormat(temp);
    // SDL_FreeSurface(temp);
    // SDL_Rect dest_rect = {0, 0, card_surface->w, card_surface->h};
    // SDL_BlitSurface(card_surface, NULL, screen, &dest_rect);
    /* DO NOT FORGET! */
    // SDL_Flip(screen);

    /* Fill screen with some color */
    SDL_FillRect(screen, NULL, 0xff009900);

    /* Load PNG and Display */
    SDL_Surface *temp = IMG_Load("200x125-skill.png");
    SDL_SetColorKey(temp, SDL_SRCCOLORKEY|SDL_RLEACCEL, 0x00000000);
    SDL_Surface *skill_surface = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);

    SDL_Rect dest_rect = {(screen->w - 200)/2, (screen->h - 125)/2, 200, 125};
    SDL_BlitSurface(skill_surface, NULL, screen, &dest_rect);
    SDL_Flip(screen);

    /* To pause the program */
    while(1){
        SDL_Delay(100);
    }
    printf("Now, I will Quit!\n");
    return 0;
}
