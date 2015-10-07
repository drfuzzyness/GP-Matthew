//
//  CoreApp.h
//  NYUCodebase
//
//  Created by Matthew Conto on 9/23/15.
//


//#ifndef CORE_H
//#define CORE_H
#pragma once

#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#ifdef _WINDOWS
#include <GL/glew.h>
#define RESOURCE_FOLDER ""
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include <vector>

#include "ShaderProgram.h"
#include "Matrix.h"

#include "Entity.h"
//#include "Ball.h"
//#include "Paddle.h"

//class Entity;

class CoreApp {
public:
    CoreApp();
    void Setup();
    void Update();
    void UpdateAndRender();
    GLuint LoadTexture(const char *image_path);
    ~CoreApp();
    bool done;
    float deltaTime;
    
    Entity ball;
    Entity playerOne;
    Entity playerTwo;
    
    static CoreApp* Instance();
    
    ShaderProgram* program;
    Matrix projectionMatrix;
    Matrix modelMatrix;
private:
    static CoreApp* pInstance;
    SDL_Window* displayWindow;
    Matrix viewMatrix;
    
    float lastFrameTicks;
    bool restartGame;
};

//#endif