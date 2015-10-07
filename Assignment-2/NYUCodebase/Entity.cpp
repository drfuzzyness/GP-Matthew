//
//  Entity.cpp
//  NYUCodebase
//
//  Created by Matthew Conto on 9/23/15.
//

#include "Entity.h"

Entity::Entity() :
    x(0.0f),
    y(0.0f),
    width(0.0f),
    height(0.0f),
    textureID(0),
    hitboxHeight(0.0f),
    hitboxWidth(0.0f),
    velocityX(0.0f),
    velocityY(0.0f)
    {
        RefreshCoords();
}

void Entity::RefreshCoords() {
    verticies = {
        -width, -height,
        width, height,
        -width, height,
        width, height,
        -width, -height,
        width, -height};
    texCoords = {
        0.0, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        1.0, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f};
}

void Entity::Draw() {
//    core->modelMatrix.identity();
//    core->modelMatrix.Translate( x, y, 0);
//    core->program->setModelMatrix(core->modelMatrix);
//    
//    glVertexAttribPointer(core->program->positionAttribute, 2, GL_FLOAT, false, 0, &verticies);
//    glEnableVertexAttribArray(core->program->positionAttribute);
//    
//    glVertexAttribPointer(core->program->texCoordAttribute, 2, GL_FLOAT, false, 0, &texCoords);
//    glEnableVertexAttribArray(core->program->texCoordAttribute);
//    
//    glBindTexture(GL_TEXTURE_2D, textureID);
//    glDrawArrays(GL_TRIANGLES, 0, 6);
//    
//    glDisableVertexAttribArray(core->program->positionAttribute);
//    glDisableVertexAttribArray(core->program->texCoordAttribute);
}

void Entity::Render( ShaderProgram* program ) {
    Matrix modelMatrix;
    modelMatrix.Translate(x, y, 0.0f);
    program->setModelMatrix( modelMatrix );
    
}