![image.png](https://upload-images.jianshu.io/upload_images/35286-eb41010f6d4e8764.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
> 图引用自稚晖君的[《配置CLion用于STM32开发【优雅の嵌入式开发】》](https://www.bilibili.com/read/cv6308000)

# 前言
前一阵子接触到STM32开发，从零开始现学现用，还好有点Arduino开发经验，上手起来不算是太困难，然而，开发STM32的IDE或者叫“编辑器”简直就是上古产物啊！！！  Keil5完全没有代码提示，连代码自动对齐都是奢望

跑通几个Demo后就实在受不了了，由于我习惯使用Idea系的IDE，如：AndroidStudio、PyCharm、WebStorm、CLion，所以就在找用CLion开发STM32的方法，然后就找的了稚晖君的[《配置CLion用于STM32开发【优雅の嵌入式开发】》](https://www.bilibili.com/read/cv6308000)，如获至宝啊

不过，稚晖君的教程是基于【HAL库】的，然而，网上的大部分样例代码也是基于【标准库】的，对于我这种小白，样例代码无比重要，毕竟也就只会搬砖嘛，【标准库】基本都是使用的Keil5，也有一部分人推荐IAR+SourceInsight开发，都是上古产物，完全无感

好奇的跟着稚晖君的教程试了下，发现是通过STM32CubeIDE生成一个基于CMake的基础项目，既然是CMake项目，那跑【标准库】不就是改下CMake的事情吗？作为一个写过两年C++开发的菜鸟，改个CMake岂不是易如反掌，所以就有了这篇文章了

# 配置环境
> 详细步骤参考稚晖君的文章，写的很详细了，我这只列依赖的软件，STM32CubeMX其实并不必要，我们只需要配一份CMake和startup的*.s文件即可，可以通过STM32CubeMX生成，同一块板子也可以直接copy过来用就好，生成逻辑参考稚晖君的文章

* MinGW 【https://sourceforge.net/projects/mingw-w64/files/】
* arm-none-eabi-gcc 【https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads】
* OpenOCD 【https://gnutoolchains.com/arm-eabi/openocd/】

这三个是必要软件，只需要下压缩包解压放到一个的目录即可，最好是 **没有中文和空格** ，配置好Path环境，保证在cmd下这几行命令都能正常运行

``` bash
gcc -v
arm-none-eabi-gcc -v
```
运行成功如下：

![image.png](https://upload-images.jianshu.io/upload_images/35286-02512060ad194751.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 配置CLion

### 1. 设置里配置编译工具链

![image.png](https://upload-images.jianshu.io/upload_images/35286-815534c61019c8a4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

> 只需要配置下路径，其他都不用改

![image.png](https://upload-images.jianshu.io/upload_images/35286-c59747954ab31a89.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

### 2. 确认CMake使用了刚才配置的工具链

![image.png](https://upload-images.jianshu.io/upload_images/35286-a7cc8606c774e62b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

### 3. 创建CubeMX工程

![image.png](https://upload-images.jianshu.io/upload_images/35286-c20c5c6ad7f9b032.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

### 4. 将相关文件Copy到项目里【或者基于STM32CubeMX生成的项目删除不必要的文件】

> 如下图所示，include&source是我自己的代码，startup和STM32F103C8Tx_FLASH.ld是从STM32CubeMX生成的项目里copy过来的

![image.png](https://upload-images.jianshu.io/upload_images/35286-e751d540a5fda7bd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

### 5. 修改CMakeLists.txt

![image.png](https://upload-images.jianshu.io/upload_images/35286-1a6810f9c1b4b951.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

修改完CMake后记得刷新下，要不然编译时会出现找不到文件，也可设置自动刷新

![image.png](https://upload-images.jianshu.io/upload_images/35286-4870b7994a06f1c2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

正常刷新后可以看到自动创建了一个运行任务，代码配置没问题的话可以直接编译出固件了

![image.png](https://upload-images.jianshu.io/upload_images/35286-3e08eb5ec9a72eba.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

### 6. 配置烧录

运行配置里新建OpenOCD烧录

![image.png](https://upload-images.jianshu.io/upload_images/35286-f140721f3690dbe3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![image.png](https://upload-images.jianshu.io/upload_images/35286-628e6c98b3accb04.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

选择好可执行程序和板子配置

![image.png](https://upload-images.jianshu.io/upload_images/35286-db29b16e881b56bc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

在OpenOCD里有很多默认的配置，我这里用的F103，所以选了一个103的配置

![image.png](https://upload-images.jianshu.io/upload_images/35286-82ce71d4db5fc593.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

很多文章都建议修改下配置文件，注释掉最后一行“reset_config srst_only”，好像不注释也没问题

![image.png](https://upload-images.jianshu.io/upload_images/35286-bc373e7d160ad5c7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

### 7. 大功告成

![image.png](https://upload-images.jianshu.io/upload_images/35286-ad0c4e09dbf5b6b8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![image.png](https://upload-images.jianshu.io/upload_images/35286-d250d2ea4ea584ff.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 最后

放一个我编译烧录都OK的项目在github上，供大家参考，我是基于STM32F103C8T6的板子移植的，这两天用CLion开发STM32简直不要太爽，希望大家都能优雅的嵌入式开发
https://github.com/vell001/CLionSTM32Demo