//
//  Entity.hpp
//  NYUCodebase
//
//  Created by Matthew Conto on 9/23/15.
//

#pragma once

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include "CoreApp.hpp"

using namespace std;

class Entity {
public:
    void Draw();
    
    float x;
    float y;
    float rotation;
    
    GLuint textureID;
    
    float width;
    float height;
    
    float hitboxWidth;
    float hitboxHeight;
    
    Entity( GLuint textureID, float x, float y, float rotation, float width, float height );

private:
    vector<float> verticies;
    vector<float> texCoords;
    CoreApp* core;
};