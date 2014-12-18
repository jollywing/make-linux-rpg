# 第2章 开始前的准备 #

> 《来，我们一起写个角色扮演游戏》系列教程

本章内容共分三节：安装C编译器，安装SDL库，用makefile管理项目。内容较多，一定要保持耐心，后面越来越有意思。

##安装C编译器

安装*gcc*（*clang*也可以，用法和*gcc*差不多）。如果你使用*Debian/Ubuntu Linux*，可能要安装 `build-essential`这个包来安装*gcc*、*make*等工具。当然，有的Linux发行版安装完就带了`gcc`和`make`。

如果你在windows上使用*MinGW*，确认安装了*gcc*和*make*。如果需要debug程序，还要安装*gdb*。别忘了把*MinGW*的`bin`目录加入windows的路径（环境变量*PATH*）中，如果你只在msys的命令行窗口中编译程序，这一步可以省略。因为我喜欢在Emacs编译器中调用*gcc*和*make*，所以我习惯把它们加入路径。

在终端输入 `gcc`，回车。如果安装成功，会看到类似下面的信息。

    gcc: fatal error: no input files
    compilation terminated.

本教程假设你有了C编程的基础，如果你还没有接触过C语言，可以找个C语言入门教程学习一下。这是绝对值得学的语言，大多数操作系统都是C写的，大多数编程语言也是用C实现的。

下面我们写第一个C程序。

Program1: *Hello, RPG*

    #include <stdio.h>
    
    int main(int argc, char *argv[])
    {
        printf("Hello, Role Play Game!\n");
        return 0;
    }

这是一个最简单的C程序。`main`是一个C程序的入口函数，即不管它放在什么位置，程序都将从`main`开始执行。参数`argc`是命令行参数的个数，`argv`是存放命令行参数的字符串数组。以后我们可能会用到命令行参数，现在我们我不用理会它们。`printf("Hello, Linux Game!\n");`将输出 `Hello, Linux Game!`。

将上述代码保存为 `main.c` （或者你想叫的其它文件名，但最好以`.c`为后缀名），然后 用`gcc -o game main.c`编译`main.c`。`-o game`表示输出的执行文件名是`game`，在windows下，会生成game.exe。执行`./game`，会输出`Hello, Role Play Game!`。

对于gcc的编译选项，我们可以先记这么几个：

+ -o <goal> ，指定生成的目标名称。
+ -I<include_path>，指定include路径，gcc搜索头文件的时候会包含这些路径。
+ -L<lib_path>，指定lib路径，gcc在链接库的时候，会搜索这些路径。
+ -l<libname>，指定生成二进制时要链接的库。比如 -lSDL 要链接 SDL库。
+ -c，只编译生成目标文件（以`.o`为后缀）。这个选项以后会用到。

##安装SDL库

[SDL](http://www.libsdl.org/) 目前有两个版本：1.2和2.0。因为1.2使用较广泛，我们也使用*SDL 1.2*。

如果你使用Linux，很简单，从软件仓库安装即可。具体软件包的名称因发行版而异（如果你用Debian/Ubuntu，`apt-cache search sdl`搜索有关*SDL*的软件包。如果你用ArchLinux，`pacman -Ss sdl`搜索有关*SDL*的软件包。别的发行版我就不献丑了）。要注意的是，你要安装的是 **开发包**，一般开发包名称中会含有 `dev` 或 `devel`的字样。

如果你在Windows上，用MinGW，跟随以下步骤安装SDL：

1. 下载一个SDL的win32开发包。现在的官网找不到这个包放在哪里了。还好，我这里有，可以从这里下载：<https://github.com/jollywing/make-linux-rpg/tree/master/attachments>。将鼠标放在`SDL-devel-1.2.15-mingw32.tar.gz`上，右键，另存为，即可下载。
2. 解压，进到目录里。把 `bin`, `include`, `lib`下的内容分别拷贝到*MinGW*的`bin`，`include`和`lib`目录里。
3. 在msys的命令行（msys的MinGW的组件，虽然不是必要，但最好安装它）窗口中，敲入`sdl-config --cflags`（这个命令会返回编译SDL程序的命令行选项），回车，看输出的信息对不对，主要是看`include`的路径对不对。如果不对，用编辑器打开MinGW的bin目录下的sdl-config，修改`prefix`的值。如果你的`include`目录放在`urs`目录下，则`prefix=/usr`。（用Linux的话，则没有个麻烦）。

最后，我们需要一个SDL的API手册。即使你用IDE，有函数原型的提示，也没有看API手册的效果好。如果你用sublime/vim/Emacs写程序，API手册更是不可少的啦。在你刚才下载的`SDL-devel-1.2.15-mingw32.tar.gz`中，docs目录下就是API手册，用浏览器打开`index.html`就可以啦。什么？你要看中文的？自己从网上搜搜吧。

## 小试牛刀，第一个SDL程序

注意看下面程序中的注释。

程序清单2(用SDL显示一个黑色窗口):


    #include <stdio.h>
    #include <SDL.h>
    
    int main(int argc, char *argv[])
    {
        /*在你调用SDL其它函数之前，先要调用SDL_Init初始化SDL子系统 */
        /*这里我们初始化了视频和音频两个子系统，你可以从手册中查看SDL_Init函数*/
        if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
            /* 如果出错，就打印错误信息，并退出。SDL_GetError获取最近的SDL错误的信息*/
            printf("Unable to initialize SDL: %s\n", SDL_GetError());
            exit(1);
        }
        /* 告诉程序在退出的时候，调用SDL_Quit关闭SDL子系统 */
        atexit(SDL_Quit);
    
        /* 下面我们创建游戏的画布，用SDL_SetVideoMode函数 */
        /* 函数原型: SDL_Surface *SDL_SetVideoMode(int width, int height, int bpp, Uint32 flags); */
        /* width和height指定了画面的宽和高。*/
        /* bpp = bits per pixel. 每个像素多少位，如果bpp等于8，就创建了一个256色的画布。*/
        /* 如果bpp等于0，表示用系统当前的显示模式。对于非全屏游戏，我们一般设置bpp为0*/
        /* flags有以下值： */
        /* SDL_SWSURFACE, 在系统内存中创建画图表面。 */
        /* SDL_HWSURFACE, 在显存中创建画图表面，有硬件加速 */
        /* SDL_DOUBLEBUF, 双缓冲，后台画图，前台显示，可避免画面闪烁 */
        /* SDL_FULLSCREEN, 全屏模式 */
        SDL_Surface *screen = SDL_SetVideoMode(640, 480, 0, SDL_HWSURFACE|SDL_DOUBLEBUF);
    
        /* 如果screen为空，说明创建画图表面失败 */
        if (screen == NULL) {
            printf("Unable to set video mode: %s\n", SDL_GetError());
            exit(1);
        }
    
        /*设置窗口标题*/
        SDL_WM_SetCaption("Hello, Linux Game!", NULL);
    
        /* 为了防止窗口一闪而过，用getchar暂停程序 */
        getchar();
        return 0;
    }

执行

    gcc -o game main.c `sdl-config --cflags --libs`

编译生成game程序。在命令行下输入 `./game`，回车，则看到如下窗口。

![第一个SDL程序](http://upload-images.jianshu.io/upload_images/121239-cad6a48778c2bb65.png)

注意，`sdl-config --cflags --libs`会返回SDL程序的编译和链接选项，而MinGW似乎要求把链接选项放在源文件后面，否则会报错。Linux无所谓。如果你使用MinGW，可以试试 <code>gcc -o game \`sdl-config --cflags --libs\` main.c</code>，在我这里会报错。

##用make管理项目

每次输入 <code>gcc -o game main.c \`sdl-config --cflags --libs\`</code> 编译程序，也很麻烦。我们还是写一个简单的makefile来管理项目吧。

在项目的目录下，建立文件`makefile`，输入以下内容：

    game: main.c
	    gcc -o game main.c `sdl-config --cflags --libs`

目前，项目目录里共有两个文件，`makefile`和`main.c`。在命令行里，敲入`make`，回车，则会自动编译程序，生成`game`。

原来，make会从`makefile`中读取规则，执行动作。make的规则符合下列格式：

    目标: 依赖项
        生成目标的规则

如果`依赖项`比`目标`新，则会执行`生成目标的规则`，重新生成`目标`。如果`依赖项`不比`目标`新，则什么都不做。比如你刚刚用`make`生成了`game`，再次执行`make`，make会告诉你`game`已经是最新的啦。也就是说，当你改了`main.c`，*make*才会重新生成`game`。

**要注意的是**：`生成目标的规则`以 **制表符** 而不是空格开头。当你写makefile时，要敲一下Tab键，再写`生成目标的规则`。

*make*很有意思，不仅仅用来管理程序。你可以在`makefile`中写入以下规则。

    love:
	    @echo "I don't know how to make love."

然后执行`make love`，*make*会告诉你 `I don't know how to make love.`

下一章我们将学习如何在SDL程序中显示图像。

-----------------

**如果你喜欢我的文章，可以点 [这里](http://weibo.com/p/1001603784418899696375) 给我打赏，五分一毛也是对我的认同。**
