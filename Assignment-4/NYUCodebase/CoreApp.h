//
//  CoreApp.h
//  NYUCodebase
//
//  Created by Matthew Conto
//


//#ifndef CORE_H
//#define CORE_H
#pragma once // Fuck linking errors.

#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#ifdef _WINDOWS
#include <GL/glew.h>
#define RESOURCE_FOLDER ""
#endif

    // Gridspace Boundaries for moving camera setup
#define GRD_LEFT -8.0f
#define GRD_RIGHT 8.0f
#define GRD_TOP 8.0f
#define GRD_BOT -8.0f


    // Player Movement
#define PLR_SPEED 6.0
#define PLR_JUMP_VEL 4.0
#define PLR_FALL_VEL 4.0
#define PLR_FALL_ACCEL 4.0



#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include <vector>

#include "ShaderProgram.h"
#include "Matrix.h"
#include "Tilemap.h"
#include "Entity.h"


using namespace std;

class CoreApp {
public:
    CoreApp();
    void Setup();
    void Update();
    void UpdateAndRender();
    GLuint LoadTexture(const char *image_path);
    void DrawText( int fontTexture, string text, float size, float spacing, float x, float y);
    ~CoreApp();
    bool done;
    float deltaTime;
    
    Entity player;
    vector<Entity> enemies;
    Tilemap tilemap;
    
    bool clearColorBit;
    
    ShaderProgram* program;
    Matrix projectionMatrix;
    Matrix modelMatrix;
    
    void DrawSpriteSheetSprite(int index, int spriteCountX, int spriteCountY);
    
    int score;
private:
    static CoreApp* pInstance;
    SDL_Window* displayWindow;
    Matrix viewMatrix;
    

    
    void StartGame();
    
        // RIP Readability. I'm seriously tired of linking errors.
            // Why does C++ have to have such fickle handling of headers...
    void UpdatePlayer(Entity& player);
    
    void PlayerJump();
    
    GLuint terrainTexture;
    GLuint charactersTexture;
    GLuint fontTexture;
    
    float lastFrameTicks;
    bool restartGame;
};

//#endif