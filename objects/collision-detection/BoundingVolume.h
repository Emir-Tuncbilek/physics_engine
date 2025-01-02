//
// Created by Emir Tuncbilek on 12/26/24.
//

#ifndef PHYSICS_ENGINE_BOUNDINGVOLUME_H
#define PHYSICS_ENGINE_BOUNDINGVOLUME_H

#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <memory>
#include <algorithm>
#include <cassert>
#include <array>

#include "./SAT_collisions.h"

#define DIMENSIONS 3
#define BOUNDING_VOLUME_FRAGMENT_SHADER_PATH "../shaders/boundingVolume.fs.glsl"
#define BOUNDING_VOLUME_VERTEX_SHADER_PATH   "../shaders/boundingVolume.vs.glsl"

class RenderObject;

class BoundingVolume {
protected:
    std::vector<float> center;
    std::vector<float> orientation;
    std::vector<std::vector<float>> rotationMatrix;
    std::shared_ptr<RenderObject> renderObject;
public:
    BoundingVolume(std::vector<float> geometricCenter);
    BoundingVolume(const BoundingVolume& boundingVolume);
    ~BoundingVolume();

    const std::vector<float>& getGeomCenter() { return this->center; }
    void translate(const std::vector<float>& newPos);
    void applyRotationMatrix(const std::vector<float>& vec, std::vector<float>& result) const;

    void setRotationMatrix(const std::vector<std::vector<float>>& rotMatrix);
    void resize(const float &x, const float &y, const float &z);

    virtual std::shared_ptr<BoundingVolume> clone() = 0;
    virtual std::pair<bool, std::vector<float>> isCollidingWith(std::shared_ptr<BoundingVolume> boundingVolume) = 0;
    virtual std::pair<bool, std::vector<float>> testNormals(std::shared_ptr<BoundingVolume> boundingVolume) = 0;
    virtual float getMin(const std::vector<float>& axis) const = 0;
    virtual float getMax(const std::vector<float>& axis) const = 0;
    virtual void render(glm::mat4 &view, glm::mat4 &projPersp) const = 0;
};


#endif //PHYSICS_ENGINE_BOUNDINGVOLUME_H
