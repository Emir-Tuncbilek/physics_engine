cmake_minimum_required(VERSION 3.24)
project(physics_engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GoogleTests
include(CTest)
enable_testing()
add_subdirectory(external)
add_subdirectory(tests)

# Manually add GLEW paths if find_package fails
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
list(APPEND EXTRA_LIBS ${SDL2_LIBRARIES})


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
        graphics/scene.cpp graphics/scene.h
        graphics/camera.cpp graphics/camera.h
        graphics/Resources.cpp graphics/Resources.h
        graphics/models.cpp graphics/models.h
        graphics/shapes.cpp graphics/shapes.h
        objects/RenderObject.cpp objects/RenderObject.h
        physics/physicsState.h physics/physicsState.cpp
        physics/forces.cpp physics/forces.h
        objects/FloorObject.cpp objects/FloorObject.h
        objects/BasicObject.cpp objects/BasicObject.h
        objects/ParticuleObject.cpp objects/ParticuleObject.h
        objects/CloudParticuleObject.cpp objects/CloudParticuleObject.h
        objects/collision-detection/BoundingVolume.cpp objects/collision-detection/BoundingVolume.h
        objects/collision-detection/BoundingSphere.cpp objects/collision-detection/BoundingSphere.h
        objects/collision-detection/BoundingBox.cpp objects/collision-detection/BoundingBox.h
        objects/collision-detection/CollisionMesh.cpp objects/collision-detection/CollisionMesh.h
        objects/collision-detection/SAT_collisions.cpp objects/collision-detection/SAT_collisions.h
        graphics/sceneContext.h
        objects/CollisionObject.cpp objects/CollisionObject.h graphics/Texture2D.cpp graphics/Texture2D.h)

# Include Dear ImGui
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)

include_directories(${IMGUI_DIR})
include_directories(${IMGUI_DIR}/backends)

set(IMGUI_SOURCES
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
        ${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
        )

# Add the ImGui sources to the executable
target_sources(physics_engine PRIVATE ${IMGUI_SOURCES})


# Link the OpenGL, GLEW, and context libraries
target_link_libraries(physics_engine
        ${OPENGL_LIBRARIES}
        ${EXTRA_LIBS}
        -lfreeimage
        )

# Link necessary frameworks on macOS
if(APPLE)
    target_link_libraries(physics_engine "-framework OpenGL")
endif()

