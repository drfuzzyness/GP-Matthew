//
//  Paddle.hpp
//  NYUCodebase
//
//  Created by Matthew Conto on 9/28/15.
//

#pragma once
#include "CoreApp.hpp"

class Paddle : public Entity {
public:
    float maxFromCenter;
    float maxVelocity;
    int score;
    Paddle( float maxFromCenter, float maxVelocity, GLuint textureID, float x, float y, float rotation, float width, float height );
    
    
};