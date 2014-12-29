# 第四章 载入并显示PNG图片 #


在第三章中，我们讲解了如何在SDL程序中载入并显示BMP图像。虽然SDL直接支持BMP图片的载入，但BMP格式的图片占用较多的硬盘空间。如果游戏的图片资源都使用BMP格式，会增大我们小游戏的体积。

那么我们可以使用别的图片格式吗？`SDL_image`帮我们提供了解决方案。使用`SDL_image`，可以载入JPG,PNG,GIF,TIF等多种图片格式。我们将使用流行的`PNG`格式。

## 一、安装SDL_image ##

这一节其实主要为在windows下使用MinGW的用户写的，因为Linux上安装SDL_image的开发包是非常简单的事。

下面主要讲解怎么在`MinGw`中安装`SDL_image`，使用Linux的用户可以直接跳到下一节。

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

其实，如果没有VC的导入库，MinGW还是有办法自己生成导入库的。比如我们有头文件和`SDL_image.dll`，分两步生成`SDL_image.dll`的导入库。

1. 用 *pexports* (从网上搜索下载)导出`SDL_image.dll`中的函数定义： `pexports SDL_image.dll > SDL_image.def`。生成的`SDL_image.def`是个文本文件，可以用文本编辑器查看。
2. 用mingw自带的`dlltool`生成导入库： `dlltool -d SDL_image.def -l libSDL_image.dll.a`，会生成`libSDL_image.dll.a`，这就是我们需要的东西，`dll.a`的后缀名告诉MinGW这是对应dll的导入库。把它放入MinGW的lib目录就可以了。

`SDL_image`安装好后，还有一件事，就是SDL_image的文档，文档页面在这里: <http://www.libsdl.org/projects/SDL_image/docs/SDL_image.html>。可以下载下来看，也可以在线看。

## 二、装载和显示PNG图片 ##

这里我们以下面图片（文件名: 200x125-skill.png）为例：

![200x125-skill.png](http://upload-images.jianshu.io/upload_images/121239-60095c38a2582487.png)


如何装载 `200x125-skill.png`，最简单的方式是使用`SDL_image`的`IMG_Load`函数，原型如下：

    SDL_Surface *IMG_Load(const char *file)

给定该函数一个图片路径，得到一个包含图像数据的SDL surface。得到surface后，后续的处理就和上面一样了。代码片段如下：

    /* 装载PNG图片，得到一个SDL页面 */
    SDL_Surface *temp = IMG_Load("200x125-skill.png");
    /* 转换SDL页面，得到一个像素格式和screen相同的页面 */
    SDL_Surface *skill_surface = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);

    /* 在screen的中间选定一个矩形区域来显示图像 */
    SDL_Rect dest_rect = {(screen->w - 200)/2, (screen->h - 125)/2, 200, 125};
    SDL_BlitSurface(skill_surface, NULL, screen, &dest_rect);
    SDL_Flip(screen);

注意，别忘了在程序开头`#include <SDL_image.h>`。`make`然后运行一下程序，看看效果如何。

## 三、设置屏幕背景色 ##

我们使用的图片背景色是黑色，刚好`screen`默认的页面颜色也是黑色，如果`screen`是其它的背景色，显示效果就没有那么好了。

现在我们用`SDL_FillRect`函数把屏幕显示区域填充成别的颜色。

    int SDL_FillRect(SDL_Surface *dst, SDL_Rect *dstrect, Uint32 color);

- `dst`表示要填充颜色的页面， 我们要填充的是`screen`页面。
- `dstrect`指向要填充的矩形区域，我们要填充整个`screen`页面，可以传递`NULL`给`dstrect`。
- `color`指定要填充的颜色，我们用十六进制来表示这个值，这次我们给它的值是：`0xff009900`。`0x`之后每两位数表示一个字节，4个字节一次表示Alpha值（透明度）、红色分量R、绿色分量G和蓝色分量B。这里我们将红色分量和蓝色分量置为0，而绿色分量是`0x99`。因此`0xff009900`表示一个暗绿色。

综上，`SDL_FillRect(screen, NULL, 0xff009900);`将把屏幕填充成暗绿色。
改写原来的代码片段，增加`SDL_FillRect`语句：

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

`make`然后运行程序，效果如图：

![图像的黑色背景很突兀](http://upload-images.jianshu.io/upload_images/121239-9d7f22abe0c05ae2.jpg)

现在图像的黑色背景给人的感觉很不好。

## 四、去除图片背景色 ##

在显示一个页面时，怎么把它的背景色去掉呢？使用`SDL_SetColorKey`来设置页面的透明色。

    int SDL_SetColorKey(SDL_Surface *surface, Uint32 flag, Uint32 key);

`surface`是要设置关键色的页面，我们这里要过滤背景的页面是`skill_surface`。

`flag`的值通常是 `SDL_SRCCOLORKEY | SDL_RLEACCEL`, 其中 `SDL_SRCCOLORKEY` 表示为页面设置的关键色在BlitSurface时将被过滤掉。`SDL_RLEACCEL`表示用 *行程编码* (?) 提高BlitSurface的效率。

`key`是要设置的关键色，即在BlitSurface时要过滤掉的透明色。是32位整数，包含4个字节。可以表示为`0xAARRGGBB`。其中`AA`是表示alpha通道的字节，`RR`, `GG`, `BB`分别表示红绿蓝三个分量。因为我们图片的背景色是纯黑，直接把`0`传递给`key`即可。

> 注意：当你知道图片的背景色（即你知道RGB的分量各是多少）时，你并不能直接用这个值来设置关键色。因为你图片的像素格式转换成`screen`的像素格式时，背景色RGB的值极可能改变，这样，背景色仍然过滤不掉。这时候，我们可以把RGB的值传递给`SDL_MapRGB`函数，从而得到一个转换后的`Uint32`的关键色表示。如果用黑色做图片的背景色，则可以省略这一步，因为不管像素格式怎么变，RGB分量的值始终是0。

综上，我们可以调用`SDL_SetColorKey(skill_surface, SDL_SRCCOLORKEY|SDL_RLEACCEL, 0)`来将`skill_surface`的黑色背景色设为透明色。

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
![滤掉背景色的图像显示](http://upload-images.jianshu.io/upload_images/121239-d19f9f363a7fa72f.jpg)

##五、小结 ##

第4章的源代码和资源可以从这里查看、下载： <https://github.com/jollywing/make-linux-rpg/tree/master/chap04>。

这一章中，我们学到了哪些新东西呢？

1. 在MinGW下安装`SDL_image`开发库。
2. 给定dll文件，如何在MinGW下生成导出库。（注意，这个方法可能要视dll中函数的调用方式做适当调整）
3. 用`SDL_image`的`IMG_Load`的函数加载PNG, JPG, GIF等格式的图片。当然，用`IMG_Load`一样可以加载BMP图片。
4. 用`SDL_FillRect`填充屏幕背景。
5. 用`SDL_SetColorKey`设置页面的关键色，这样当该页面Blit到其它页面上时，关键色将被过滤掉。

下一章，我们将学习更激动人心的内容：**动起来的画面**。

