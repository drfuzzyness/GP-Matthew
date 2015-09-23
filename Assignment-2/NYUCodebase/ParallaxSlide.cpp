//
//  ParallaxSlide.cpp
//  NYUCodebase
//
//  Created by Matthew Conto on 9/21/15.
//  Copyright © 2015 Ivan Safrin. All rights reserved.
//
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "ShaderProgram.h"
#include "Matrix.h"
#include "ParallaxSlide.hpp"


ParallaxSlide::ParallaxSlide( float originPos, float startPos, float maxPos, float velocity, GLuint textureID ) : texture(textureID), xPos( startPos ), maxXPos( maxPos ), originXPos(originPos) {
    }

void ParallaxSlide::move() {
    xPos += velocity;
    if( xPos >= maxXPos ) {
        xPos = originXPos;
        std::cout << "Slide position reset";
    }
    return;
}