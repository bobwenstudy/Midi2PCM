# 简介

本项目是基于[schellingb/TinySoundFont: SoundFont2 synthesizer library in a single C/C++ file (github.com)](https://github.com/schellingb/TinySoundFont)的一个演示例程。实现MIDI到WAV文件的转换功能。

为了验证Code Size，搭建了Cortex-CM0 GCC环境，项目在Cortex-CM0下编译，整个Code Size和RAM Size如下。

注意：RAM SIZE由于库用了很多malloc行为，所以这里并不真实。

# 使用说明

## 环境搭建

目前暂时只支持Windows编译，最终生成exe，可以直接在PC上跑。

目前需要安装如下环境：

- [Python3](http://www.python.org/getit/)，用于RAM&ROM分析等，编译工具类都用这个。
- GCC环境，笔者用的msys64+mingw，用于编译生成exe，参考这个文章安装即可。[Win7下msys64安装mingw工具链 - Milton - 博客园 (cnblogs.com)](https://www.cnblogs.com/milton/p/11808091.html)。



## 编译说明

本项目都是由makefile组织编译的，编译整个项目只需要执行`make all`即可。

也就是可以通过如下指令来编译工程：

```shell
make all
```

![image-20230625143121305](https://markdown-1306347444.cos.ap-shanghai.myqcloud.com/img/image-20230625143121305.png)

## 使用说明

编译好的执行文件在output目录下，以Windows为例，是`output/main.exe`，程序运行输入需要`test.sf2`和`test.mid`两个文件，输出`test.wav`文件，替换目录中的对应输入文件可以更改输出。

![image-20230625143212067](https://markdown-1306347444.cos.ap-shanghai.myqcloud.com/img/image-20230625143212067.png)

# Code Size分析说明

## 介绍

本项目最终为嵌入式服务，大家对Code Size和RAM Size非常关心，项目集成了CM0编译环境，以便大家评估协议栈大小。

项目以Cortex-CM0来对芯片进行Code Size和RAM Size进行分析。

## 环境搭建

目前暂时只支持CM0编译，最终生成elf，程序并不能运行，但是可以用于评估协议栈Code Size。

在安装好Windows环境基础上，还需要安装如下环境：

- [Arm GNU Toolchain](http://www.python.org/getit/)，ARM Toolchain，交叉工具链，用于编译项目。

## 编译说明

切换到CM0/GCC目录下，调整Makefile中的Toolchain路径，并执行`make all`即可。

![image-20230625142828284](https://markdown-1306347444.cos.ap-shanghai.myqcloud.com/img/image-20230625142828284.png)

![image-20230625142843530](https://markdown-1306347444.cos.ap-shanghai.myqcloud.com/img/image-20230625142843530.png)

### RAM_Report&ROM_Report

参考zephyr，对生成的elf进行分析，最终会生成ram.json和rom.json。这两个文件也可以导入到nordic的vscode环境下，可以借助其图形化工具进行分析。

从rom_report可以看出，math和基本的库用了32KB，项目本身需要12KB左右。

![image-20221125112930355](https://markdown-1306347444.cos.ap-shanghai.myqcloud.com/img/image-20221125112930355.png)

![image-20230625142915877](C:/Users/wenbo/AppData/Roaming/Typora/typora-user-images/image-20230625142915877.png)









