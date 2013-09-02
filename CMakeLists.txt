cmake_minimum_required(VERSION 2.8)

project(glgb)

add_executable(glgb src/main.cpp
                    src/oam.cpp
                    src/disassembler.cpp
                    src/debugger.cpp 
                    src/input.cpp
                    src/cpu.cpp 
                    src/emulator.cpp 
                    src/cart.cpp 
                    src/header.cpp 
                    src/mmu.cpp 
                    src/gpu.cpp
                    )

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_link_libraries(glgb ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )

add_definitions(-Wall -Wextra)

