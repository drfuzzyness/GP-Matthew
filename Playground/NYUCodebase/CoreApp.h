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

    // Gridspace Boundaries for fixed camera setup
#define GRD_LEFT -8.0
#define GRD_RIGHT -8.0
#define GRD_TOP 8.0
#define GRD_BOT -8.0

    // Enemy Movement
#define ENM_BASE_MOVEMENT_TICK_TIME 1.0
#define ENM_MOVEMENT_TICK_MAX .1
#define ENM_MOVEMENT_AMOUNT .5

    // Player Movement
#define PLR_SPEED 2.0


#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include <vector>

#include "ShaderProgram.h"
#include "Matrix.h"

#include "Entity.h"

using namespace std;

enum GameState { MENU, GAME, GAMEOVER };

class SpriteSheet {
public: // Fix
    SpriteSheet( GLuint textureSheet, float right, float top, float left, float bot, float scale );
};

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
    
    Entity player;
    vector<Entity> enemies;
    Entity enemyBullet;
    Entity playerBullet;
    
    int enmCurrentDirection; // x pos or x neg
    int enmNextDirection;
    
    static CoreApp* Instance();
    
    ShaderProgram* program;
    Matrix projectionMatrix;
    Matrix modelMatrix;
    
    void DrawSpriteSheetSprite(int index, int spriteCountX, int spriteCountY);
    
    GameState state;
private:
    static CoreApp* pInstance;
    SDL_Window* displayWindow;
    Matrix viewMatrix;
    

    
    void GotoMainMenu();
    void GotoGame();
    void GotoGameOver();
    
        // RIP Readability. I'm seriously tired of linking errors.
            // Why does C++ have to have such fickle handling of headers...
    void UpdatePlayer(Entity& player);
    void UpdateEnemy(Entity& enemy);
    void UpdatePlayerBullet(Entity& pBullet);
    void UpdateEnemyBullet(Entity& eBullet);
    
    void PlayerShoot();
    
    float lastFrameTicks;
    bool restartGame;
};

//#endif