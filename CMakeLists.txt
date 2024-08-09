cmake_minimum_required(VERSION 3.5)
project(JoystickCode)

# Find the necessary packages
find_package(Qt5 COMPONENTS Core Network  Widgets REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

set(CMAKE_AUTORCC ON)

# Add executable target
add_executable(JoystickCode
JoystickCode.cpp
)

# Link libraries
target_link_libraries(JoystickCode PRIVATE
    Qt5::Core
    Qt5::Network
    Qt5::Widgets
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    
)

# Include directories
target_include_directories(JoystickCode PRIVATE
    ${OPENGL_INCLUDE_DIR}
)

