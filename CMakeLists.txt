cmake_minimum_required(VERSION 3.15)

# Set up vcpkg toolchain if it's defined
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
else()
    message(FATAL_ERROR "VCPKG_ROOT environment variable not set.")
endif()

project(MyApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/branch_rend)
add_executable(MyApp main.cpp)


file(GLOB_RECURSE PHYSICS_SOURCES "physics/*.cpp")
file(GLOB_RECURSE PHYSICS_HEADERS "physics/*.h")


include_directories(${CMAKE_SOURCE_DIR}/physics
)


target_sources(MyApp PRIVATE
    ${PHYSICS_SOURCES} ${PHYSICS_HEADERS}
)

find_package(glad CONFIG REQUIRED)
target_link_libraries(MyApp PRIVATE glad::glad)

# Link the branch_rend library to the main application

find_package(Bullet CONFIG REQUIRED)


target_link_libraries(MyApp PRIVATE
    BulletDynamics
    BulletCollision
    LinearMath
)

target_link_libraries(MyApp PRIVATE branch_rend ${BULLET_LIBRARIES})

# Set additional compiler flags if needed (optional)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /GL")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG")
