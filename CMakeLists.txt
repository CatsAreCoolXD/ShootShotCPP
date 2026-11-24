cmake_minimum_required(VERSION 4.3)
project(ShootShot)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_PREFIX_PATH /home/dkooistra/Documents/CGGL/install)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# CGGL
find_package(CGGL CONFIG REQUIRED)

add_executable(ShootShot 
        main.cpp

        game.h
        game.cpp

        gui.h
        gui.cpp

        audio.h
)

target_link_libraries(ShootShot
        PRIVATE
        CGGL::CGGL
)
