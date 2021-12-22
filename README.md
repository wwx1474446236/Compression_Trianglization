# Compression_Trianglization
 这是计算机图形学的代码

##环境
### PCL1.8 + VS2017
### 或者直接将代码部分考出来，放到有 PCl + C++ 环境下IDEA
### 实测Linux可行，CMakelist需要自己写

## 主代码 
### PCD_viewer.cpp 是测试有无PCL环境的代码， 可以先运行这个测试一下
### MyOctree.cpp  是定义控制台应用程序的入口点。运行这个就好！


## Linux下的 CMakelist
```
cmake_minimum_required(VERSION 2.6)

project(Compression_Trianglization)

find_package(PCL 1.8 REQUIRED)

include_directories(${PCL_INCLUDE_DIRS})

link_directories(${PCL_LIBRARY_DIRS})

add_definitions(${PCL_DEFINITIONS})

add_executable(file_dealing 改这/MyOctree.cpp)     %改一下这

target_link_libraries (file_dealing ${PCL_LIBRARIES})

```
