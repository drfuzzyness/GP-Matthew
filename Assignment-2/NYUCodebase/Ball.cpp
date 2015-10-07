//
//  Ball.cpp
//  NYUCodebase
//
//  Created by Matthew Conto on 9/30/15.
//

#include "Ball.h"

Ball::Ball( float velocityX, float velocityY, float speed, GLuint textureID, float x, float y, float rotation, float width, float height ) : velocityX(velocityX), velocityY(velocityY), speed(speed), Entity(textureID, x, y, rotation, width, height ) {
}
