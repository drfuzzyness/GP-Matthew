//
//  Entity.cpp
//  NYUCodebase
//
//  Created by Matthew Conto on 9/23/15.
//

#include "Entity.hpp"

Entity::Entity( GLuint textureID, float x, float y, float rotation, float width, float height ) : x(x), y(y), rotation(rotation), width(width), height(height), textureID(textureID), hitboxHeight(height), hitboxWidth(width) {
    
    verticies = {-width, -height, width, height, -width, height, width, height, -width, -height, width, -height};
    texCoords = {0.0, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0, 0.0, 0.0, 1.0, 1.0, 1.0};
    
    core = CoreApp::Instance();
}

void Entity::Draw() {

    core->modelMatrix.identity();
    core->modelMatrix.Translate( x, y, 0);
    core->program->setModelMatrix(core->modelMatrix);
    
    glVertexAttribPointer(core->program->positionAttribute, 2, GL_FLOAT, false, 0, &verticies);
    glEnableVertexAttribArray(core->program->positionAttribute);
    
    glVertexAttribPointer(core->program->texCoordAttribute, 2, GL_FLOAT, false, 0, &texCoords);
    glEnableVertexAttribArray(core->program->texCoordAttribute);
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(core->program->positionAttribute);
    glDisableVertexAttribArray(core->program->texCoordAttribute);
}
