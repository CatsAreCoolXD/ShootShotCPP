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
        
        # ImGui
        imgui/imgui.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/imgui_demo.cpp

        # ImGui backends
        imgui/backends/imgui_impl_glfw.cpp
        imgui/backends/imgui_impl_opengl3.cpp

        # Wrapper
        imgui/misc/cpp/imgui_stdlib.cpp
        )

target_include_directories(ShootShot
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui
)

target_include_directories(ShootShot
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
)

target_include_directories(ShootShot
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/misc/cpp
)

target_link_libraries(ShootShot
        PRIVATE
        CGGL::CGGL
)
