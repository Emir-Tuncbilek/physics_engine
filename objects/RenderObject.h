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
#include <cmath>

#include "../graphics/models.h"
#include "../graphics/textures.h"
#include "../graphics/shaderProgram.h"
#include "../graphics/sceneContext.h"
#include "../physics/physicsState.h"
#include "./collision-detection/CollisionMesh.h"

#define OFFSET 8

class RenderObject {
protected:
    std::unique_ptr<CollisionMesh> collisionMesh;
    std::shared_ptr<SceneContext> context;
public:
    RenderObject();

    RenderObject(const std::string& modelPath);

    RenderObject(const RenderObject& other);

    ~RenderObject() = default;

    void computeCollisions(std::shared_ptr<RenderObject>& o);
    void addBoundingVolume(const std::shared_ptr<BoundingVolume>& volume);
    bool isIntersecting(const std::shared_ptr<RenderObject>& o) const;
    virtual void init();
    virtual void translateCollisionMeshToState();
    virtual void rotateCollisionMeshToState();
    virtual void setContextFromScene(const std::shared_ptr<SceneContext>& sceneContext);
    virtual void renderCollisionMesh(glm::mat4 &view, glm::mat4 &projPersp, const PhysicsState& oldState);

    virtual std::vector<std::shared_ptr<PhysicsState>> getPhysicState();

    virtual std::vector<std::shared_ptr<RenderObject>> getObjects() const = 0;
    virtual std::shared_ptr<RenderObject> clone() const = 0;
    virtual void render(glm::mat4 &view, glm::mat4 &projPersp, const float& delta_t) = 0;

public:
    std::vector<std::pair<std::string, GLenum>> shaderPaths;
    std::shared_ptr<ShaderProgram> modelShaderProgram;
    std::shared_ptr<ShaderProgram> transformShaderProgram;
    std::shared_ptr<Texture2D> modelTexture;
    std::shared_ptr<Model> model3D;
    std::shared_ptr<PhysicsState> physics;
    GLint mvpModelLocation;
};



#endif //PHYSICS_ENGINE_RENDEROBJECT_H
