cmake_policy(SET CMP0048 NEW)
project(ExampleApp)
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -mfma -mavx2 -m64 -ggdb -fno-omit-frame-pointer")

set(V8_REPO_ROOT "/v8")

include_directories( "src")
include_directories(run_app PRIVATE "${V8_REPO_ROOT}/include")

link_directories(${V8_REPO_ROOT}/out.gn/x64.release.sample/obj/)

add_executable(run_app src/main.cpp)
target_link_libraries(run_app PUBLIC stdc++fs v8_monolith pthread)
target_compile_definitions(run_app PRIVATE V8_COMPRESS_POINTERS)
