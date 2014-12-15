
程序清单1(经典C程序):

    #include <stdio.h>
    
    int main(int argc, char *argv[])
    {
        printf("Hello, Linux Game!\n");
        return 0;
    }

这是经典的C程序的结构，`main`是一个C程序的入口函数。
参数`argc`是命令行参数的个数，`argv`是存放命令行参数的字符串数组。
`printf("Hello, Linux Game!\n");`将输出 `Hello, Linux Game!`。

程序清单2(用SDL显示一个黑色窗口):

    /* usage: gcc -o game `sdl-config --cflags --libs` main.c */
    #include <stdio.h>
    #include <SDL.h>
    
    int main(int argc, char *argv[])
    {
        /* Before calling other SDL functions, you should SDL_Init first */
        if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
            /* SDL_GetError get the information of last SDL Error, return a
             * null terminated string */
            printf("Unable to initialize SDL: %s\n", SDL_GetError());
            exit(1);
        }
        /* SDL_Quit will shut down SDL subsystems */
        atexit(SDL_Quit);
    
        /* SDL_Surface *SDL_SetVideoMode(int width, int height, int bpp, Uint32 flags); */
        /* bpp = bits per pixel. If bpp is 0, it is treated as the current
         * display bits per pixel. */
        /* SDL_SWSURFACE, Create the video surface in system memory */
        /* SDL_HWSURFACE, Create the video surface in video memory */
        /* SDL_DOUBLEBUF, Enable double buffering, you can use SDL_Flip to update screen */
        /* SDL_FULLSCREEN, full screen mode */
        /* return NULL on Setting Video Mode fail */
        SDL_Surface *screen = SDL_SetVideoMode(640, 480, 0, SDL_HWSURFACE|SDL_DOUBLEBUF);
    
        if (screen == NULL) {
            printf("Unable to set video mode: %s\n", SDL_GetError());
            exit(1);
        }
    
        SDL_WM_SetCaption("Hello, Linux Game!", NULL);
    
        /* To pause the program */
        getchar();
        return 0;
    }
