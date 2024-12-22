//
// Created by Emir Tuncbilek on 8/29/24.
//

#ifndef PHYSICS_ENGINE_RESOURCES_H
#define PHYSICS_ENGINE_RESOURCES_H

#pragma once
#include <memory>

#include "../objects/RenderObject.h"

class Resources {
public:
    std::vector<std::unique_ptr<RenderObject>> objects;
public:
    Resources();
    ~Resources();
    void addObject(std::unique_ptr<RenderObject> object);
};

#endif //PHYSICS_ENGINE_RESOURCES_H
