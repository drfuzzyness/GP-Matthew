//
//  Paddle.cpp
//  NYUCodebase
//
//  Created by Matthew Conto on 9/28/15.
//

#include "Paddle.h"

Paddle::Paddle( float maxFromCenter, float maxVelocity, GLuint textureID, float x, float y, float rotation, float width, float height ) : maxFromCenter(maxFromCenter), maxVelocity(maxVelocity), score(0), Entity(textureID, x, y, rotation, width, height) {}