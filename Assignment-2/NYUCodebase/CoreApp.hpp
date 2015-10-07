//
//  CoreApp.hpp
//  NYUCodebase
//
//  Created by Matthew Conto on 9/23/15.
//


#ifndef CORE_H
#define CORE_H

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

#include "Entity.hpp"
#include "Ball.hpp"
#include "Paddle.hpp"

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
    
    Ball* ball;
    Paddle* playerOne;
    Paddle* playerTwo;
    
    
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

#endif