//
// Created by Emir Tuncbilek on 9/6/24.
//

#ifndef PHYSICS_ENGINE_RENDEROBJECT_H
#define PHYSICS_ENGINE_RENDEROBJECT_H

#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <functional>
#include <fstream>
#include <sstream>
#include <memory>

#include "../graphics/models.h"
#include "../graphics/textures.h"
#include "../graphics/shaderProgram.h"
#include "../physics/physicsState.h"

class RenderObject {
public:
    RenderObject();

    RenderObject(const std::string& modelPath);

    RenderObject(const RenderObject& other);

    ~RenderObject() = default;

    std::vector<std::pair<std::string, GLenum>> shaderPaths;
    std::shared_ptr<ShaderProgram> modelShaderProgram;
    std::shared_ptr<Texture2D> modelTexture;
    std::shared_ptr<Model> model3D;
    std::shared_ptr<PhysicsState> physics;
    GLint mvpModelLocation;

    void computeCollisions(std::unique_ptr<RenderObject>& o);

    virtual void init();
    virtual void render(glm::mat4 &view, glm::mat4 &projPersp, const float& delta_t) = 0;
    virtual std::vector<std::shared_ptr<PhysicsState>> getPhysicState();
};



#endif //PHYSICS_ENGINE_RENDEROBJECT_H
