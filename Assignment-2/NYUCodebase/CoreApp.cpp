//
//  CoreApp.cpp
//  NYUCodebase
//
//  Created by Matthew Conto on 9/23/15.
//

#include "CoreApp.hpp"

CoreApp::~CoreApp() {
    delete program;
}

CoreApp::CoreApp() {
    lastFrameTicks = 0.0f;
    deltaTime = 0.0f;
    done = false;
    
    
    Setup();
}

void CoreApp::Setup() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Forest", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 569, 320, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    #ifdef _WINDOWS
        glewInit();
    #endif
    
        // EXTRA SETUP GOES HERE
    
    glViewport(0, 0, 569, 320);
    
    program = new ShaderProgram( RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl" );
    
    projectionMatrix.setOrthoProjection(-3.55, 3.55, -2.0, 2.0, -1.0, 1.0);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void CoreApp::UpdateAndRender() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
            // Checks all the SDL event flags
    }
    
    float ticks = (float)SDL_GetTicks()/1000.0;
    deltaTime = ticks - lastFrameTicks;
    lastFrameTicks= ticks;
    
    glClearColor( .2f, .2f, .2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program->programID);
    
    program->setModelMatrix(modelMatrix);
    program->setViewMatrix(viewMatrix);
    program->setProjectionMatrix(projectionMatrix);
    
    Update();
    
    SDL_GL_SwapWindow(displayWindow);
}

void CoreApp::Update() {
    
}

