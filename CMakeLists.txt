cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "-std=c++17 -O0")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "/std:c++17 /O0")
endif()

project(AMCAXStepIO)

set(JSON_PATH ./3rd/Json)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(./include)
include_directories(${JSON_PATH})

set(AMCAX_ROOT_PATH ./3rd/AMCAXKernel_Release)
include_directories(${AMCAX_ROOT_PATH}/include)
link_directories(${AMCAX_ROOT_PATH}/lib)

file(GLOB_RECURSE AMCAX_SRC ./src/*.cpp)

add_executable(${PROJECT_NAME} ./src/main.cpp ${AMCAX_SRC})

target_link_libraries(${PROJECT_NAME} PUBLIC
  AMCAXCommon
  AMCAXGCS
  AMCAXIges
  AMCAXMeshing
  AMCAXOCCTIO
  AMCAXPart
  AMCAXStep
  AMCAXSubD
  AMCAXTMSpline)
