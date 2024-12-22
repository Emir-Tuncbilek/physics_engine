cmake_minimum_required(VERSION 3.24)
project(physics_engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Manually add GLEW paths if find_package fails
find_package(OpenGL REQUIRED)

# Find and include SDL2 or GLFW based on your choice
set(CONTEXT "sdl2")  # Change to "glfw3" if you prefer GLFW

if(${CONTEXT} STREQUAL "sdl2")
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})
    list(APPEND EXTRA_LIBS ${SDL2_LIBRARIES})
elseif(${CONTEXT} STREQUAL "glfw3")
    find_package(GLFW3 REQUIRED)
    include_directories(${GLFW_INCLUDE_DIRS})
    list(APPEND EXTRA_LIBS ${GLFW_LIBRARIES})
endif()

# Manually specify the GLEW paths (adjust these if your paths differ)
include_directories(/opt/homebrew/include)
link_directories(/opt/homebrew/lib)
list(APPEND EXTRA_LIBS GLEW)

# Find and include GLM
find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

# Add the executable and link the necessary libraries
add_executable(physics_engine
        main.cpp
        graphics/public-libs/stb_image.h
        graphics/public-libs/obj_loader.h
        graphics/shaderProgram.cpp graphics/shaderProgram.h
        graphics/window.cpp graphics/window.h
        graphics/textures.cpp graphics/textures.h
        graphics/scene.cpp graphics/scene.h
        graphics/camera.cpp graphics/camera.h
        graphics/Resources.cpp graphics/Resources.h
        graphics/models.cpp graphics/models.h
        graphics/shapes.cpp graphics/shapes.h
        objects/RenderObject.cpp objects/RenderObject.h physics/physicsState.h physics/forces.cpp physics/forces.h objects/FloorObject.cpp objects/FloorObject.h objects/F1Object.cpp objects/F1Object.h objects/ParticuleObject.cpp objects/ParticuleObject.h objects/CloudParticuleObject.cpp objects/CloudParticuleObject.h physics/physicsState.cpp)

# Link the OpenGL, GLEW, and context libraries
target_link_libraries(physics_engine
        ${OPENGL_LIBRARIES}
        ${EXTRA_LIBS}
        -lfreeimage
        )

# Link necessary frameworks on macOS
if(APPLE)
    target_link_libraries(physics_engine "-framework OpenGL")
    if(${CONTEXT} STREQUAL "glfw3")
        target_link_libraries(physics_engine "-lobjc -framework Foundation -framework Cocoa")
    endif()
endif()
