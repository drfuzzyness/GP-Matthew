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
#define GRD_LEFT -8.0f
#define GRD_RIGHT 8.0f
#define GRD_TOP 8.0f
#define GRD_BOT -8.0f

    // Enemy Movement
#define ENM_MOVEMENT_BASE_TICK_TIME 1.0
//#define ENM_MOVEMENT_TICK_MAX .1 
#define ENM_MOVEMENT_AMOUNT .5 // stylish as fuck
#define ENM_MOVEMENT_TICK_INCREMENT -.025 // tuned as fuck
                                          // this imitates the fact that the original game's speed increased as the framerate increased.
#define ENM_GAMEOVER_Y -6.0f

    // Enemy Grid
#define ENM_GRID_TOP 6.0f
#define ENM_GRID_LEFT -6.0f
#define ENM_GRID_MARGIN 0.0f
#define ENM_GRID_NUM_ENEMIES 6 * 6

    // Player Movement
#define PLR_SPEED 6.0

    // Bullets
#define BULLET_SPEED 12.0


#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include <vector>

#include "ShaderProgram.h"
#include "Matrix.h"

#include "Entity.h"

using namespace std;

enum GameState { MENU, GAME, GAMEOVER };

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
    Entity enemyBullet;
    Entity playerBullet;
    
    int enmCurrentDirection; // x pos or x neg
    bool enmHitWall;
    bool enmMoveDown;
    float enmTickWait;
    float enmTickCount;
    int enmRemaining;
    
    bool clearColorBit;
    
    static CoreApp* Instance();
    
    ShaderProgram* program;
    Matrix projectionMatrix;
    Matrix modelMatrix;
    
    void DrawSpriteSheetSprite(int index, int spriteCountX, int spriteCountY);
    
    GameState state;
    int lives;
    int score;
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
    
    GLuint invadersTexture;
    GLuint fontTexture;
    
    float lastFrameTicks;
    bool restartGame;
};

//#endif