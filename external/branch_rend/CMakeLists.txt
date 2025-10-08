cmake_minimum_required(VERSION 3.15)

project(branch_rand)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_library(branch_rend STATIC)

file(GLOB_RECURSE BRANCH_REND_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)


target_sources(branch_rend PRIVATE ${BRANCH_REND_SRC})

target_include_directories(branch_rend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


file(GLOB SHADERS "shaders/*")
file(COPY ${SHADERS} DESTINATION ${CMAKE_BINARY_DIR}/shaders)


find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(assimp REQUIRED)


target_link_libraries(branch_rend
    PRIVATE
        OpenGL::GL
        glad::glad
        GLEW::GLEW
        assimp::assimp
        glfw
)
