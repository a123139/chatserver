cmake_minimum_required(VERSION 3.0)
project(main)

# 配置编译选项（开启调试 + C++17）
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++17")

# 配置头文件搜索路径（你系统真实路径）
include_directories(/usr/local/include  /usr/local/boost/include)

# 配置库文件搜索路径（你系统真实路径）
link_directories( /usr/local/lib)

# 设置需要编译的源文件列表
set(SRC_LIST muduo_server.cpp)

#二进制可执行文件输出位置
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) 

# 把.指定路径下的所有源文件名字放入变量名SRC_LIST里面
# aux_source_directory(. SRC_LIST)

# 生成可执行文件 server
add_executable(cmakeserver ${SRC_LIST})

# 链接 muduo 库 + 线程库（必须！）
target_link_libraries(cmakeserver  muduo_net  muduo_base pthread)