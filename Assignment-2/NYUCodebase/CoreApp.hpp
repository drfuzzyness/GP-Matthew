//
//  CoreApp.hpp
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

#include "ShaderProgram.h"
#include "Matrix.h"
#include "ParallaxSlide.hpp"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

class CoreApp {
public:
    CoreApp();
    void Setup();
    void Update();
    void UpdateAndRender();
    ~CoreApp();
    bool done;
    float deltaTime;
private:
    ShaderProgram* program;
    SDL_Window* displayWindow;
    
    Matrix projectionMatrix;
    Matrix modelMatrix;
    Matrix viewMatrix;
    
    float lastFrameTicks;
    
    
};