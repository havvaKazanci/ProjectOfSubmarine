cmake_minimum_required(VERSION 3.5)
project(Joystick10Axis)

find_package(Qt5 COMPONENTS Core Widgets Network REQUIRED)

find_package(SDL2 CONFIG REQUIRED)

set(CMAKE_AUTORCC ON)

add_executable(Joystick10Axis Joystick10Axis.cpp)

target_link_libraries(Joystick10Axis PRIVATE
    Qt5::Core
    Qt5::Network
    Qt5::Widgets
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    ${OPENGL_LIBRARIES}
)
