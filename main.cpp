#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "graphics/window.h"
#include "graphics/camera.h"
#include "graphics/scene.h"
#include "graphics/Resources.h"

#include "objects/BasicObject.h"
#include "objects/FloorObject.h"
#include "objects/CloudParticuleObject.h"

/* Physics constants */
#define NUM_PARTICULES 100
#define CLOUD_SPEED_VEC 5.0f, 0.0f, 1.0f
#define CLOUD_WIDTH 0.0f
#define CLOUD_HEIGHT 2.0f
#define CLOUD_DEPTH 2.0f

/* Window constants */
#define CAMERA_SPEED 0.01f
#define VIEW_ANGLE 70.0f
#define CAMERA_DISTANCE 5.0f
#define MIN_DISTANCE_VIEW 0.01f
#define MAX_DISTANCE_VIEW 400.0f

/* Scene constants */
// Car
#define SU57_MASS_KG 100.0f
#define SU57_MOMENT_OF_INERTIA 0.0f  /* For now, inertia is not implemented */
#define SU57_ELASITICITY 0.3f
#define SU57_FRICTION_COEFFICIENT 0.0f /* For now, friction is not implemented */

// Floor
#define FLOOR_MASS_KG std::numeric_limits<float>::infinity()
#define FLOOR_MOMENT_OF_INERTIA 0.0f  /* For now, inertia is not implemented */
#define FLOOR_ELASITICITY 0.9f
#define FLOOR_FRICTION_COEFFICIENT 0.0f /* For now, friction is not implemented */

// Particules
#define PARTICULE_MASS_KG 0.1f
#define PARTICULE_MOMENT_OF_INERTIA 0.0f  /* For now, inertia is not implemented */
#define PARTICULE_ELASITICITY 0.9f
#define PARTICULE_FRICTION_COEFFICIENT 0.0f /* For now, friction is not implemented */

void buildScene(Resources& resources);

int main() {

    Window w("My physics engine");
    if (!w.init())
        return -1;

    GLenum rev = glewInit();
    if (rev != GLEW_OK) {
        std::cout << "Could not initialize glew! GLEW_Error: " << glewGetErrorString(rev) << std::endl;
        return -2;
    }

    auto cameraPosition = glm::vec3(0);
    auto cameraOrientation = glm::vec2(0);
    Camera c(cameraPosition, cameraOrientation);

    std::cout << "Rendering..." << std::endl;

    Resources resources;

    buildScene(resources);

    Scene scene(resources);

    bool isRunning = true;

    std::cout << "Drawing..." << std::endl;

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);   // Gray
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    const std::vector<float> force = { 0.0f, 0.0f, -GRAVITY_ACCELERATION_RATE };      // Gravity is only applied vertically and towards the bottom
    scene.addForces(std::make_shared<ForceField>(force));

    while (isRunning) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        c.update(w, CAMERA_SPEED);

        glm::mat4 projPersp = glm::perspective(glm::radians(VIEW_ANGLE), (float)w.getWidth() / (float)w.getHeight(), MIN_DISTANCE_VIEW, MAX_DISTANCE_VIEW);
        glm::mat4 view = c.getViewMatrix();
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -CAMERA_DISTANCE));

        scene.render(view, projPersp);

        w.swap();
        w.pollEvent();
        isRunning = !(w.shouldClose() || w.getKeyPress(Window::Keys::ESCAPE));
    }
    return 0;
}

void buildScene(Resources& resources) {
    const float delta_t = 0.0f;

    // su57 state
    const std::vector<float> su57Pos = { 0.0, 0.0f, 1.0f };
    const std::vector<float> su57Orientation = { ZERO_VECTOR };
    const std::vector<float> su57Velocity = { ZERO_VECTOR };
    const std::vector<float> su57AngVelocity = { ZERO_VECTOR };
    auto su57State = std::make_shared<PhysicsState>(delta_t,
                           SU57_MASS_KG,
                           SU57_MOMENT_OF_INERTIA,
                           SU57_ELASITICITY,
                           SU57_FRICTION_COEFFICIENT,
                           su57Pos,
                           su57Orientation,
                           su57Velocity,
                           su57AngVelocity);

    // Particule states
    const std::vector<float> particulePos = { -5.0, 1.0f, 0.0f };
    const std::vector<float> particuleOrientation = { ZERO_VECTOR };
    const std::vector<float> particuleVelocity = { CLOUD_SPEED_VEC };
    const std::vector<float> particuleAngVelocity = { ZERO_VECTOR };
    auto particuleState = std::make_shared<PhysicsState>(delta_t,
                           PARTICULE_MASS_KG,
                           PARTICULE_MOMENT_OF_INERTIA,
                           PARTICULE_ELASITICITY,
                           PARTICULE_FRICTION_COEFFICIENT,
                           particulePos,
                           particuleOrientation,
                           particuleVelocity,
                           particuleAngVelocity);

    // Floor state
    const std::vector<float> floorPos = { 0.0, 0.0f, -3.0f };
    const std::vector<float> floorOrientation = { ZERO_VECTOR };
    const std::vector<float> floorVelocity = { ZERO_VECTOR };
    const std::vector<float> floorAngVelocity = { ZERO_VECTOR };
    auto floorState = std::make_shared<PhysicsState>(delta_t,
                                 FLOOR_MASS_KG,
                                 FLOOR_MOMENT_OF_INERTIA,
                                 FLOOR_ELASITICITY,
                                 FLOOR_FRICTION_COEFFICIENT,
                                 floorPos,
                                 floorOrientation,
                                 floorVelocity,
                                 floorAngVelocity);

    std::shared_ptr<BasicObject> su57 = std::make_shared<BasicObject>("../3DObjects/su57.obj");
    su57->shaderPaths.emplace_back("../shaders/objectShader.vs.glsl", GL_VERTEX_SHADER);
    su57->shaderPaths.emplace_back("../shaders/objectShader.fs.glsl", GL_FRAGMENT_SHADER);
    su57->modelTexture = std::make_shared<Texture2D>("../textures/su57_texture.png");
    su57->init(su57State);
    su57->collisionMeshScale(0.7f, 0.95f, 0.2f);
    su57->nudgeCollisionMesh({0.0f, 0.15f, 0.2f});
    resources.addObject(su57);

    /*
    std::shared_ptr<CloudParticuleObject> particuleCloud = std::make_shared<CloudParticuleObject>(
            "../3DObjects/cube.obj",
            NUM_PARTICULES,
            std::pair<float, float> ({ -CLOUD_WIDTH, CLOUD_WIDTH }),
            std::pair<float, float> ({ -CLOUD_HEIGHT, CLOUD_HEIGHT }),
            std::pair<float, float> ({ -CLOUD_DEPTH, CLOUD_DEPTH}));

    particuleCloud->shaderPaths.emplace_back("../shaders/particule.vs.glsl", GL_VERTEX_SHADER);
    particuleCloud->shaderPaths.emplace_back("../shaders/particule.fs.glsl", GL_FRAGMENT_SHADER);
    particuleCloud->init(particuleState);
    resources.addObject(particuleCloud);
     */

    std::shared_ptr<FloorObject> floor = std::make_shared<FloorObject>("../3DObjects/cube.obj");
    floor->shaderPaths.emplace_back("../shaders/floor.vs.glsl", GL_VERTEX_SHADER);
    floor->shaderPaths.emplace_back("../shaders/floor.fs.glsl", GL_FRAGMENT_SHADER);
    floor->init(floorState);

    resources.addObject(floor);
}
