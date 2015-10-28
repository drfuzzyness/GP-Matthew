//
//  Entity.h
//  NYUCodebase
//
//  Created by Matthew Conto
//

#pragma once
//#ifndef ENTITY_H
//#define ENTITY_H
//#include "CoreApp.h"
#include "Matrix.h"
#include "ShaderProgram.h"
#include <vector>

#include "SpriteSheet.h"

using namespace std;

class Entity {
public:
    Entity();
    void Render( ShaderProgram* program );
    bool IsCollidingWith( Entity& otherEntity );
    void ResolveCollision( float tile_x, float tile_y );
    
    bool active;
    float x;
    float y;
    float velocityX;
    float velocityY;
    
    float width;
    float height;
    float hitboxWidth;
    float hitboxHeight;
    
    SpriteSheet sprite;
};