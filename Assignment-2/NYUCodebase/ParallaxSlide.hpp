//
//  ParallaxSlide.hpp
//  NYUCodebase
//
//  Created by Matthew Conto on 9/21/15.
//  Copyright © 2015 Ivan Safrin. All rights reserved.
//

#ifndef ParallaxSlide_hpp
#define ParallaxSlide_hpp

#include <stdio.h>

class ParallaxSlide {
public:
    GLuint texture;
    float originXPos;
    float xPos;
    float maxXPos;
    float velocity;
    ParallaxSlide( float originPos, float startPos, float maxPos, float velocity, GLuint textureID );
    void move();
};

#endif /* ParallaxSlide_hpp */
