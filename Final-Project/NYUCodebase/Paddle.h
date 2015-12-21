//
//  Paddle.hpp
//  NYUCodebase
//
//  Created by Matthew Conto on 9/28/15.
//

#pragma once
//#ifndef PADDLE_H
//#define PADDLE_H
//#include "Entity.hpp"
#include "CoreApp.h"
//#include "Entity.h"

class Paddle : public Entity {
public:
    float maxFromCenter;
    float maxVelocity;
    int score;
    Paddle( float maxFromCenter, float maxVelocity, GLuint textureID, float x, float y, float rotation, float width, float height );
    void Draw();
    
    
};

//#endif