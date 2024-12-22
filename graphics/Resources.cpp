//
// Created by Emir Tuncbilek on 8/29/24.
//

#include "Resources.h"


Resources::Resources() : objects() {}

Resources::~Resources() = default;

void Resources::addObject(std::unique_ptr<RenderObject> object) { this->objects.push_back(std::move(object)); }
