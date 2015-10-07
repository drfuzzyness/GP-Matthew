//
//  Entity.h
//  NYUCodebase
//
//  Created by Matthew Conto on 9/23/15.
//

#pragma once
//#ifndef ENTITY_H
//#define ENTITY_H
//#include "CoreApp.h"
#include "Matrix.h"
#include "ShaderProgram.h"
#include <vector>

using namespace std;

enum EntityType{ ENTITY_PADDLE, ENTITY_BALL };

class Entity {
public:
    void Draw();
    void Render( ShaderProgram* program );
    void RefreshCoords();
    
    float x;
    float y;
    float velocityX;
    float velocityY;
    
    float width;
    float height;
    float hitboxWidth;
    float hitboxHeight;
    
    GLuint textureID;

    EntityType type;
    
    Entity();
private:
    vector<float> verticies;
    vector<float> texCoords;
};
//#endif