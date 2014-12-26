## 为使用PNG图片资源做准备，安装SDL_image ##

BMP格式的图片占用较多的硬盘空间，如果游戏的图片资源都使用BMP格式，会增大我们小游戏的体积。
那么我们可以使用别的图片格式吗？`SDL_image`帮我们提供了解决方案。使用`SDL_image`，可以载入JPG,PNG,GIF,TIF等多种图片格式。我们将使用流行的`PNG`格式。

要使用`SDL_image`的功能，我们首先需要在`MinGw`中安装`SDL_image`。

SDL_image 1.2的主页在这里：<http://www.libsdl.org/projects/SDL_image/release-1.2.html>，我们要从这里下载 SDL_image的开发包。咦？怎么只有VC的开发包，没有MinGW的开发包呢？

没有关系，就下载VC的开发包吧：<http://www.libsdl.org/projects/SDL_image/release/SDL_image-devel-1.2.12-VC.zip>。

一个开发库如果以动态链接库的形式发布，包含三个组成部分：头文件、动态库、导入库。下面我们一一搞定。

- 解压压缩包，把include目录中的SDL_image.h拷贝到MinGW的include/SDL目录中，头文件搞定。
- 打开开发包中的lib目录，发现有SDL_image.dll和jpeg、png等图片格式的解码库，把这些动态库全部放入MinGW中的bin目录中，动态库搞定。
- lib目录中还有一个SDL_image.lib，这是VC格式的导入库，MinGW能识别吗？把它放入MinGW的lib目录中，我们试试吧。

修改makefile，在链接选项后增加 `-lSDL_image`，表示生成执行程序时，链接SDL_image库。

    game: main.c
        gcc -o game main.c `sdl-config --cflags --libs` -lSDL_image

make一下，发现成功了。原来MinGW可以识别VC的lib格式的导入库，真是宽容，真是伟大！

其实，如果没有VC的导入库，mingw还是有办法自己生成导入库的。比如我们有头文件和SDL_image.dll，分两步生成SDL_image.dll的导入库。

1. 用pexports(从网上搜索下载)导出SDL_image.dll中的函数定义： `pexports SDL_image.dll > SDL_image.def`。生成的`SDL_image.def`是个文本文件，可以用文本编辑器查看。
2. 用mingw自带的`dlltool`生成导入库： `dlltool -d SDL_image.def -l libSDL_image.dll.a`，会生成libSDL_image.dll.a，这就是我们需要的东西，`dll.a`的后缀名告诉mingw这是对应dll的导入库。把它放入mingw的lib目录就可以了。

SDL_image安装好后，还有一件事，就是SDL_image的文档，
文档页面在这里: http://www.libsdl.org/projects/SDL_image/docs/SDL_image.html
可以下载下来看，也可以在线看。

## 装载和显示PNG图片 ##

这里我们以下面图片（文件名: 200x125-skill.png）为例：
![200x125-skill.png](200x125-skill.png)

如何装载 `200x125-skill.png`，最简单的方式是使用SDL_image的IMG_Load函数，原型如下：

    SDL_Surface *IMG_Load(const char *file)

给定一个图片路径，得到一个SDL surface。得到surface后，后续的处理就和上面一样了。代码片段如下：

    /* Load a given png image, get a SDL surface */
    SDL_Surface *temp = IMG_Load("200x125-skill.png");
    /* Get a converted surface */
    SDL_Surface *skill_surface = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);

    /* set the destination Rect to the center of screen */
    SDL_Rect dest_rect = {(screen->w - 200)/2, (screen->h - 125)/2, 200, 125};
    SDL_BlitSurface(skill_surface, NULL, screen, &dest_rect);
    SDL_Flip(screen);

注意，别忘了在程序开头`#include <SDL_image.h>`。

## 关键色 ##

在显示一个页面时，怎么把它的背景色去掉呢？使用`SDL_SetColorKey`来设置页面的透明色。

    int SDL_SetColorKey(SDL_Surface *surface, Uint32 flag, Uint32 key);

`surface`是要设置关键色的页面，我们这里要过滤背景的页面是`skill_surface`。

`flag`的值通常是 `SDL_SRCCOLORKEY | SDL_RLEACCEL`, 其中 `SDL_SRCCOLORKEY` 表示为页面设置的关键色在BlitSurface时将被过滤掉。`SDL_RLEACCEL`表示用 *行程编码* (?) 提高BlitSurface的效率。

`key`是要设置的关键色，即在BlitSurface时要过滤掉的透明色。是32位整数，包含4个字节。可以表示为`0xAARRGGBB`。其中`AA`是表示alpha通道的字节，`RR`, `GG`, `BB`分别表示红绿蓝三个分量。因为我们图片的背景色是纯黑，直接把`0`传递给`key`即可。

> 注意：当你知道图片的背景色（即你知道RGB的分量各是多少）时，你并不能直接用这个值来设置关键色。因为你图片的像素格式转换成`screen`的像素格式时，RGB的值极可能改变。这时候，我们可以把RGB的值传递给`SDL_MapRGB`函数，从而得到一个`Uint32`的关键色表示。如果用黑色做图片的背景色，则可以省略这一步，因为不管像素格式怎么变，RGB分量的值始终是0。

综上，我们可以调用`SDL_SetColorKey(skill_surface, SDL_SRCCOLORKEY|SDL_RLEACCEL, 0)`来将`skill_surface`的背景色设为透明色。

加载并显示图片的代码变为：

    /* Load PNG */
    SDL_Surface *temp = IMG_Load("200x125-skill.png");
    /* set transparent color before converting surface with SDL_DisplayFormat */
    /* in order to take advantage of hardware acceleration*/
    SDL_SetColorKey(temp, SDL_SRCCOLORKEY|SDL_RLEACCEL, 0x00000000);
    SDL_Surface *skill_surface = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);

    SDL_Rect dest_rect = {(screen->w - 200)/2, (screen->h - 125)/2, 200, 125};
    SDL_BlitSurface(skill_surface, NULL, screen, &dest_rect);
    SDL_Flip(screen);

重新`make`，运行`./game`，效果如图：
![滤掉背景色的图像显示](chap03/dislay-png.jpg)
