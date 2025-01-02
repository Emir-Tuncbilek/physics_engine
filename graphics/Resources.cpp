//
// Created by Emir Tuncbilek on 8/29/24.
//

#include "Resources.h"


Resources::Resources() : objects() {}

Resources::~Resources() = default;

void Resources::addObject(const std::shared_ptr<RenderObject>& object) { this->objects.push_back(object); }

void Resources::reset() { for (auto && object : this->objects) object->reset(); }