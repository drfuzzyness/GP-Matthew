//
//  Ball.hpp
//  NYUCodebase
//
//  Created by Matthew Conto on 9/30/15.
//  Copyright © 2015 Ivan Safrin. All rights reserved.
//

#pragma once
#include "CoreApp.hpp"

class Ball : public Entity {
public:
    float velocityX;
    float velocityY;
    float speed;
    Ball( float velocityX, float velocityY, float speed, GLuint textureID, float x, float y, float rotation, float width, float height );
private:
};
