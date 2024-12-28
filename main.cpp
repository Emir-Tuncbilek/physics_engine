#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "graphics/window.h"
#include "graphics/camera.h"
#include "graphics/scene.h"
#include "graphics/Resources.h"

#include "objects/F1Object.h"
#include "objects/FloorObject.h"
#include "objects/CloudParticuleObject.h"

/* Physics constants */
#define NUM_PARTICULES 100
#define CLOUD_SPEED 5.0f
#define CLOUD_WIDTH 2.0f
#define CLOUD_HEIGHT 2.0f
#define GRAVITY_ACCELERATION_RATE 9.81f

/* Window constants */
#define CAMERA_SPEED 0.01f
#define VIEW_ANGLE 70.0f
#define CAMERA_DISTANCE 5.0f
#define MIN_DISTANCE_VIEW 0.1f
#define MAX_DISTANCE_VIEW 200.0f


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

    std::shared_ptr<F1Object> car = std::make_shared<F1Object>("../3DObjects/cube.obj");    // load a cube instead of the F1 object for faster rendering
    // std::unique_ptr<F1Object> car = std::make_unique<F1Object>("../3DObjects/F1_2026.obj");
    car->shaderPaths.emplace_back("../shaders/f1CarShader.vs.glsl", GL_VERTEX_SHADER);
    car->shaderPaths.emplace_back("../shaders/f1CarShader.fs.glsl", GL_FRAGMENT_SHADER);
    car->init();
    resources.addObject(car);

    std::shared_ptr<CloudParticuleObject> particuleCloud = std::make_shared<CloudParticuleObject>("../3DObjects/cube.obj", NUM_PARTICULES, CLOUD_WIDTH, CLOUD_HEIGHT, CLOUD_SPEED);
    particuleCloud->shaderPaths.emplace_back("../shaders/particule.vs.glsl", GL_VERTEX_SHADER);
    particuleCloud->shaderPaths.emplace_back("../shaders/particule.fs.glsl", GL_FRAGMENT_SHADER);
    particuleCloud->init();
    resources.addObject(particuleCloud);

    std::shared_ptr<FloorObject> floor = std::make_shared<FloorObject>("../3DObjects/cube.obj");
    floor->shaderPaths.emplace_back("../shaders/floor.vs.glsl", GL_VERTEX_SHADER);
    floor->shaderPaths.emplace_back("../shaders/floor.fs.glsl", GL_FRAGMENT_SHADER);
    floor->init();
    resources.addObject(floor);

    Scene scene(resources);

    bool isRunning = true;

    std::cout << "Drawing..." << std::endl;

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);   // Gray
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    const float force[3] = { 0.0f, 0.0f, -GRAVITY_ACCELERATION_RATE };      // Gravity is only applied vertically and towards the bottom
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
