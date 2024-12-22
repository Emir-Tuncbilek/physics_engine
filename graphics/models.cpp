//
// Created by Emir Tuncbilek on 8/30/24.
//

#include "models.h"

#include "public-libs/obj_loader.h"
#include <iostream>

Model::Model(const char* path) :
m_count(0) {
    std::vector<GLuint> indices;
    loadObj(path, vertexData, indices);
    this->m_shape.setData(&this->vertexData[0], vertexData.size() * sizeof(GLfloat),
                  &indices[0], indices.size() * sizeof(unsigned int));
    // buffer looks like this : [[position X, position Y, position Z, Texture Coord X, Texture Coord Y, Red, Green, Blue], ... ]
    this->m_shape.enableAttribute(0, 3, 8, 0); // position
    this->m_shape.enableAttribute(1, 2, 8, 3); // texture coordinates
    this->m_shape.enableAttribute(2, 3, 8, 5); // color
    this->m_count = indices.size();
}

void Model::loadObj(const char* path, std::vector<GLfloat>& vertexData, std::vector<GLuint>& indices) {
    objl::Loader loader;
    bool loadout = loader.LoadFile(path);
    if (!loadout) {
        std::cout << "Unable to load model " << path << std::endl;
        return;
    }

    for (size_t i = 0; i < loader.LoadedVertices.size(); i++) {
        objl::Vector3 p = loader.LoadedVertices[i].Position;
        vertexData.push_back(p.X);
        vertexData.push_back(p.Y);
        vertexData.push_back(p.Z);

        objl::Vector2 t = loader.LoadedVertices[i].TextureCoordinate;
        vertexData.push_back(t.X);
        vertexData.push_back(t.Y);

        objl::Vector3 n = loader.LoadedVertices[i].Normal;
        vertexData.push_back(n.X);
        vertexData.push_back(n.Y);
        vertexData.push_back(n.Z);
    }
    indices = loader.LoadedIndices;
}

void Model::draw() {
    this->m_shape.draw(GL_TRIANGLES, this->m_count);
}
