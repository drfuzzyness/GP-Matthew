//
//  CoreApp.cpp
//  NYUCodebase
//
//  Created by Matthew Conto on 9/23/15.
//

#include "CoreApp.h"

CoreApp::~CoreApp() {
    delete program;
}

CoreApp::CoreApp() {
    lastFrameTicks = 0.0f;
    deltaTime = 0.0f;
    done = false;
    pInstance = this;
    
    Setup();
}

void CoreApp::DrawSpriteSheetSprite(int index, int spriteCountX, int spriteCountY) {
    float u = (float)(((int)index) % spriteCountX) / (float) spriteCountX;
    float v = (float)(((int)index) / spriteCountX) / (float) spriteCountY;
    float spriteWidth = 1.0/(float)spriteCountX;
    float spriteHeight = 1.0/(float)spriteCountY;
    GLfloat texCoords[] = {
        u, v+spriteHeight,
        u+spriteWidth, v,
        u, v,
        u+spriteWidth, v,
        u, v+spriteHeight,
        u+spriteWidth, v+spriteHeight
    };
    float vertices[] = {
        -0.5f, -0.5f,
        0.5f, 0.5f,
        -0.5f, 0.5f,
        0.5f, 0.5f,
        -0.5f, -0.5f,
        0.5f, -0.5f };
        // our regular sprite drawing
}

GLuint CoreApp::LoadTexture(const char *image_path) {
    SDL_Surface *surface = IMG_Load(image_path);
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    SDL_FreeSurface(surface);
    return textureID;
}

void CoreApp::Setup() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    #ifdef _WINDOWS
        glewInit();
    #endif
    
        // EXTRA SETUP GOES HERE
    restartGame = true;
    
    
    glViewport(0, 0, 600, 600);
    
    program = new ShaderProgram( RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl" );
    
    projectionMatrix.setOrthoProjection(-6.0, 6.0, -6.0, 6.0, -6.0, 6.0);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    GLuint whiteTexture = LoadTexture(RESOURCE_FOLDER"white.jpg");
    
    ball = Entity();
    ball.x = 0.0f;
    ball.y = 0.0f;
    playerOne = Entity();
    playerOne.x = 0.0f;
    playerOne.y = 0.0f;
    playerOne.width = 0.5f;
    playerOne.height = 2.0f;
    playerTwo = Entity();
    
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
    
        // draw objects
    ball.Render( program );
    playerOne.Render( program );
    playerTwo.Render( program );
    
    SDL_GL_SwapWindow(displayWindow);
}

void CoreApp::Update() {
        // check if apply restart
        // move ball
    
        // move paddles
        // check if ball should bounce or if gameover
    
}

CoreApp* CoreApp::pInstance = NULL;

CoreApp* CoreApp::Instance() {
    if( !pInstance ) {
        pInstance = new CoreApp();
    }
    
    return pInstance;
}

