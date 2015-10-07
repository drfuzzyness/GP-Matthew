//
//  Ball.hpp
//  NYUCodebase
//
//  Created by Matthew Conto on 9/30/15.
//

//#ifndef BALL_H
//#define BALL_H
#pragma once
#include "CoreApp.h"

class Ball : public Entity {
public:
    float velocityX;
    float velocityY;
    float speed;
    Ball( float velocityX, float velocityY, float speed, GLuint textureID, float x, float y, float rotation, float width, float height );
    void Draw();
};
//#endif // BALL_H