# OpenGL Samples

    一个跨平台的OpenGL示例项目。本项目所用的语言为C++，提供了非常详尽的中文代码注释，非常适合计算机图形学和OpenGL的初学者学习参考。

    在学习过程中遇到任何问题或建议，欢迎大家和我交流讨论。我的QQ号为：53517099

## 使用方法
----
### 1. Linux平台

* 使用apt包管理工具下载OpenGL、GLUT和GLEW工具包，具体操作步骤如下：
    
  1. 安装基本编译环境。如已安装，无需此步
     > sudo apt-get install build-essential
  2. 安装OpenGL基本库
     > sudo apt-get install libgl1-mesa-dev
  3. 安装OpenGL Utilities 工具库
     > sudo apt-get install libglu1-mesa-dev
  4. 安装OpenGL Utility Toolkit 用于支持用户界面窗口
     > sudo apt-get install freeglut3-dev

* 下载并安装linux版本的CMake工具,具体步骤如下：

  1. 从官网下载最新版本的cmake压缩包，输入以下命令解压缩：
     > sudo tar -zxvf cmake-3.9.6.tar.gz
  2. 进入到解压后的目录，执行以下操作:
     > sudo ./bootstrap  
     > sudo make  
     > sudo make install
  3. 查看是否安装成功，若输出版本号，说明安装成功，否则失败，可能上述步骤有误。操作如下：
     > cmake --version

* 安装Git版本管理工具, 具体步骤如下：
  
  1. 首先执行以下命令检测是否已经安装Git，如果已经安装，则会输出git的版本号，否则执行第2步操作。
     > git --versoin
  2. 执行以下命令即可完成Git的安装
     > sudo apt-get install git

* 下载并使用本项目，以此为基础开始Linux平台下OpenGL的学习之旅
  
  1. 使用Git直接下载本项目源码：
     > git clone https://github.com/baiwen1979/opengl-samples.git
  
  2. 进入项目文件夹,使用cmake命令构建项目
     > cd opengl-samples  
     > cd build  
     > cmake .. 
  3. 使用make命令编译和链接项目源码，并运行可行性程序 
     > make  
     > bin/CGLab

### 2. MACOS平台

* 从苹果应用商店安装Xcode，具体安装步骤请参考苹果官方文档。安装好Xcode后，OpenGL，GLUT库都已经安装好，无需专门安装。MACOS版的GLEW本项目自带，无需安装。

* CMake的源码安装方法与Linux相同。也可以直接下载安装[二进制DMG版本](https://cmake.org/download/)进行安装。

* 安装Git版本控制工具。有两种安装方法：

  1. 先安装homebrew，然后通过homebrew安装Git，具体方法请参考homebrew的文档：http://brew.sh/
  2. 第二种方法更简单，也是推荐的方法，就是直接使用Xcode的集成的Git，默认没有安装，你需要运行Xcode，选择菜单“Xcode”->“Preferences”，在弹出窗口中找到“Downloads”，选择“Command Line Tools”，点“Install”就可以完成安装了。

* 下载并使用本项目的步骤和LINUX相同。

## Windows + Visual Studio 2017 平台

* 从微软官方网站下载并安装Visual Studio Community 2017。【注意】一定要安装C++语言支持，具体步骤问度娘。

* 下载Cmake的Windows安装包(.msi)，注意要根据您Windows的位数（32/64）选择相应的版本（x86/x64)。下载地址为：[https://cmake.org/download/]  下载完成后，双击安装即可，在安装向导中,要选择为所有用户添加Cmake到PATH系统环境变量，如图所示：

    <img src="http://img.blog.csdn.net/20160830035822836"/>

* 点击Github上本项目源码首页的“clone or download"->"Download Zip"下载[本项目源码](https://github.com/baiwen1979/opengl-samples)的压缩包。

* 解压得到opengl-samples-master文件夹，重命名为opengl-samples
