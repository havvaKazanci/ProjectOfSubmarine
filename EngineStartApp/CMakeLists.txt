cmake_minimum_required(VERSION 3.5)
project(EngineStartApp)

# Find the necessary packages
find_package(Qt5 COMPONENTS Core Widgets Network REQUIRED)
find_package(OpenGL REQUIRED)

set(CMAKE_AUTORCC ON)

# Add executable target
add_executable(EngineStartApp EngineStartApp.cpp)

# Link libraries
target_link_libraries(EngineStartApp PRIVATE
    Qt5::Core
    Qt5::Network
    Qt5::Widgets
    ${OPENGL_LIBRARIES}
)

# Include directories
target_include_directories(EngineStartApp PRIVATE
    ${OPENGL_INCLUDE_DIR}
)