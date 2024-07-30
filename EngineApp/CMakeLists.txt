cmake_minimum_required(VERSION 3.5)
project(EngineApp)

# Find the necessary packages
find_package(Qt5 COMPONENTS Core Network  Widgets REQUIRED)
find_package(OpenGL REQUIRED)

set(CMAKE_AUTORCC ON)

# Add executable target
add_executable(EngineApp 
EngineApp.cpp
)

# Link libraries
target_link_libraries(EngineApp PRIVATE
    Qt5::Core
    Qt5::Network
    Qt5::Widgets
    ${OPENGL_LIBRARIES}
)

# Include directories
target_include_directories(EngineApp PRIVATE
    ${OPENGL_INCLUDE_DIR}
)