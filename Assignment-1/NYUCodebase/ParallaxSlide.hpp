//
//  ParallaxSlide.hpp
//  NYUCodebase
//
//  Created by Matthew Conto on 9/21/15.
//

#pragma once

#include <stdio.h>

class ParallaxSlide {
public:
    GLuint texture;
    float originXPos;
    float xPos;
    float maxXPos;
    float velocity;
    ParallaxSlide( float originPos, float startPos, float maxPos, float velocity_, GLuint textureID );
    void move();
};
