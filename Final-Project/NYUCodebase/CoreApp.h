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

    // Generation Rules
#define GEN_MAX_CRABS 20
#define GEN_MAX_COINS 20
#define GEN_MAX_TOWELS 20
#define GEN_MAX_FOOTPRINTS 60
#define GEN_BASE_DELAY 1
#define GEN_COIN_CHANCE 40
#define GEN_TOWEL_CHANCE 30
#define GEN_CRAB_CHANCE 30
#define GEN_BASE_CHANCE 30

    // Playspace Setup
#define PS_ROWX_0 -1.0f
#define PS_ROWX_1 1.0f
#define PS_ROWX_2 3.0f
#define PS_CUTOFFY 10.0f
#define PS_STARTY -10.0f


    // Player
#define PLR_SPEED_SEC 6.0
#define PLR_LIVES 5
#define PLR_JUMP_DUR 1
#define BCH_SCROLLPERSEC .04f
#define GRD_SCROLLPERSEC 1.28f
#define PLR_ANIM_DELAY .1f
#define PLR_ANIM_FRAME_COUNT 9;

    //


#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <vector>
#include <string>

#include "ShaderProgram.h"
#include "Matrix.h"
#include "Tilemap.h"
#include "Entity.h"


using namespace std;

enum Stage { MainMenu, Game, Over };

class CoreApp {
public:
    CoreApp();
    void Setup();
    void Update();
    void UpdateAndRender();
    void SetupSceneRendering();
    void RenderScenery();
    GLuint LoadTexture(const char *image_path);
    void DrawText( int fontTexture, string text, float size, float spacing, float x, float y);
    ~CoreApp();
    bool done;
    
    float deltaTime;
    int windowSizeX;
    int windowSizeY;
    
    Entity player;
    Entity playerShadow;
    int playerRow;
    int lives;
    float timeleftAir;
    float timeLeftWalkAnim;
    int playerFrame;
    bool insideObj;
    
    float timeBetweenGenerations;
    float generationTimer;
    int anyChance;
    int coinChance;
    int crabChance;
    int towelChance;
    
    vector<Entity> crabs;
    int lastCrab;
    vector<Entity> coins;
    int lastCoin;
    vector<Entity> towels;
    int lastTowel;
    vector<Entity> footsteps;
    int lastStep;
    Entity beachBG;
    
    bool clearColorBit;
    float distanceTraveled;
    int coinsCollected;
    
    
    
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
    
    void UpdatePlayer();
    
    void MoveEntityWithScene( Entity& ent );
    
    
    void CrabCollideWithPlayer( Entity& crab );
    void TowelCollideWithPlayer( Entity& towel );
    void CoinCollideWithPlayer( Entity& coin );
    
    
    void GenerateTrail();
    EntityType ChooseObject();
    void MakeObject(EntityType& type, int row);
    float RowToX( int row );
    
    vector<EntityType> nextTrail;
    
    GLuint beachTextures;
    GLuint charactersTexture;
    GLuint fontTexture;
    GLuint coinTexture;
    GLuint towelTexture;
    GLuint crabTexture;
    GLuint shadowTexture;
    GLuint titleTexture;
    GLuint scoreTexture;
    GLuint gameoverTexture;
    GLuint footprintTexture;
    
    Mix_Chunk* coinCollectSFX;
    Mix_Chunk* crabSFX;
    Mix_Chunk* hitChairSFX;
    Mix_Chunk* jumpStartSFX;
    Mix_Chunk* jumpLandSFX;
    Mix_Music* music;
    
    Entity titleObj;
    Entity scoreObj;
    Entity gameoverObj;
    
    float lastFrameTicks;
    bool restartGame;
    
    
    Stage gameStage;
    
    void RenderMainMenu();
    void RenderGame();
    void RenderGameover();
    void RenderScoreOverlay();
    
    void DoGameover();
    void DoMainMenu();
    void DoStartGame();
};

//#endif