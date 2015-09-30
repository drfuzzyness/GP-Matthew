//
//  Ball.cpp
//  NYUCodebase
//
//  Created by Matthew Conto on 9/30/15.
//  Copyright © 2015 Ivan Safrin. All rights reserved.
//

#include "Ball.hpp"

Ball::Ball( float velocityX, float velocityY, float speed, GLuint textureID, float x, float y, float rotation, float width, float height ) : velocityX(velocityX), velocityY(velocityY), speed(speed), Entity(textureID, x, y, rotation, width, height ) {}