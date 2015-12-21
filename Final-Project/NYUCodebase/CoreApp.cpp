//
//  CoreApp.cpp
//  NYUCodebase
//
//  Created by Matthew Conto
//

#include "CoreApp.h"

CoreApp::~CoreApp() {
    delete program;
}

CoreApp::CoreApp() {
    lastFrameTicks = 0.0f;
    deltaTime = 0.0f;
    done = false;
    
    Setup();
}

GLuint CoreApp::LoadTexture(const char *image_path) {
    SDL_Surface *surface = IMG_Load(image_path);
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_BGRA,
                 GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    SDL_FreeSurface(surface);
    return textureID;
}

void CoreApp::DrawText( int fontTexture, string text, float size, float spacing, float x, float y ) {
    float texture_size = 1.0/16.0f;
    vector<float> vertexData;
    vector<float> texCoordData;
    
    modelMatrix.identity();
    
    modelMatrix.Translate(x, y, 0.0f);
    program->setModelMatrix( modelMatrix );
    for(int i=0; i < text.size(); i++) {
        float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
        float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
        vertexData.insert(vertexData.end(), {
            ((size+spacing) * i) + (-0.5f * size), 0.5f * size,
            ((size+spacing) * i) + (-0.5f * size), -0.5f * size,
            ((size+spacing) * i) + (0.5f * size), 0.5f * size,
            ((size+spacing) * i) + (0.5f * size), -0.5f * size,
            ((size+spacing) * i) + (0.5f * size), 0.5f * size,
            ((size+spacing) * i) + (-0.5f * size), -0.5f * size,
        });
        texCoordData.insert(texCoordData.end(), {
            texture_x, texture_y,
            texture_x, texture_y + texture_size,
            texture_x + texture_size, texture_y,
            texture_x + texture_size, texture_y + texture_size,
            texture_x + texture_size, texture_y,
            texture_x, texture_y + texture_size,
        }); }
    glUseProgram(program->programID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
    glEnableVertexAttribArray(program->texCoordAttribute);
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glDrawArrays(GL_TRIANGLES, 0, text.size() * 6);
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);

}

void CoreApp::Setup() {
    windowSizeX = windowSizeY = 512;
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Long Walks on the Beach Simulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowSizeX, windowSizeY, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    #ifdef _WINDOWS
        glewInit();
    #endif
    glViewport(0, 0, windowSizeX, windowSizeY);
    program = new ShaderProgram( RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl" );
    projectionMatrix.setOrthoProjection(GRD_LEFT, GRD_RIGHT, GRD_BOT, GRD_TOP, -1.0, 1.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
        // im so lazy that imma spend more time embedding individual textures rather than making a single spritesheet
        // they're also all not pow2, so no mobile game here!
        // also sdl documentation is scary
    beachTextures = LoadTexture(RESOURCE_FOLDER"beach-bg-loop.png");
    charactersTexture = LoadTexture(RESOURCE_FOLDER"compiled-beachbods.png");
    coinTexture = LoadTexture(RESOURCE_FOLDER"coinGold.png");
    towelTexture = LoadTexture(RESOURCE_FOLDER"towel.png");
    crabTexture = LoadTexture(RESOURCE_FOLDER"crabcropped.png");
    fontTexture = LoadTexture(RESOURCE_FOLDER"pixel_font.png");
    shadowTexture = LoadTexture(RESOURCE_FOLDER"playershadow.png");
    titleTexture = LoadTexture(RESOURCE_FOLDER"lwotb-title.png");
    scoreTexture = LoadTexture(RESOURCE_FOLDER"lwotb-score.png");
    gameoverTexture = LoadTexture(RESOURCE_FOLDER"lwotb-gameover.png");
    footprintTexture = LoadTexture(RESOURCE_FOLDER"footprint.png");
    
    
    coinCollectSFX = Mix_LoadWAV(RESOURCE_FOLDER"coin-sfx.wav");
    jumpLandSFX = Mix_LoadWAV(RESOURCE_FOLDER"jumpland-sfx.wav");
    jumpStartSFX = Mix_LoadWAV(RESOURCE_FOLDER"jumpstart-sfx.wav");
    crabSFX = Mix_LoadWAV(RESOURCE_FOLDER"hurt1-sfx.wav");
    hitChairSFX = Mix_LoadWAV(RESOURCE_FOLDER"hurt2-sfx.wav");
    music = Mix_LoadMUS("ChariotsOfFireLoop.ogg");
    
     Mix_PlayMusic(music, -1);
    
    
        // EXTRA SETUP GOES HERE
    clearColorBit = true;
    
    titleObj = Entity();
    titleObj.sprite = SpriteSheet( titleTexture, 0, 0, 1, 1, 14 );
    
    scoreObj = Entity();
    scoreObj.sprite = SpriteSheet( scoreTexture, 0, 0, 1, 1, 3 );
    
    gameoverObj = Entity();
    gameoverObj.sprite = SpriteSheet( gameoverTexture, 0, 0, 1, 1, 10 );
    
    SetupSceneRendering();
    DoMainMenu();
}

void CoreApp::SetupSceneRendering() {
    distanceTraveled = 0;
    beachBG = Entity();
    beachBG.x = 0;
    beachBG.y = 0;
    beachBG.width = fabs( GRD_TOP) + fabs(GRD_BOT);
    beachBG.height = fabs( GRD_TOP) + fabs(GRD_BOT);
    beachBG.sprite = SpriteSheet( beachTextures, 0, 0, 1, .5f, fabs( GRD_TOP) + fabs(GRD_BOT) ); // change to true height ratios
    
    crabs = vector<Entity>();
    for( int i = 0; i < GEN_MAX_CRABS; i++ ) {
        crabs.push_back( Entity() );
        crabs[i].active = false;
        crabs[i].type = Crab;
        crabs[i].hitboxHeight = .5f;
        crabs[i].hitboxWidth = .5f;
        crabs[i].sprite = SpriteSheet( crabTexture, 0, 0, 1, 1, .7f );
    }
    lastCrab = 0;
    coins = vector<Entity>();
    for( int i = 0; i < GEN_MAX_COINS; i++ ) {
        coins.push_back( Entity() );
        coins[i].active = false;
        coins[i].type = Coin;
        coins[i].sprite = SpriteSheet( coinTexture, 0, 0, 1, 1, 1 );
    }
    lastCoin = 0;
    towels = vector<Entity>();
    for( int i = 0; i < GEN_MAX_TOWELS; i++ ) {
        towels.push_back( Entity() );
        towels[i].active = false;
        towels[i].type = Beachtowel;
        towels[i].sprite = SpriteSheet( towelTexture, 0, 0, 1, 1, 1 );
    }
    lastTowel = 0;
    for( int i = 0; i < GEN_MAX_FOOTPRINTS; i++ ) {
        footsteps.push_back( Entity() );
        footsteps[i].active = false;
        footsteps[i].type = Phantom;
        footsteps[i].sprite = SpriteSheet( footprintTexture, 0, 0, 1, 1, .25f );
        
    }
    lastStep = 0;
    
}

void CoreApp::StartGame() {
    
    insideObj = false;
    player = Entity();
    player.x = PS_ROWX_1;
    player.y = 6;
    player.width = 1.3f;
    player.height = 1.3f;
//    player.sprite = SpriteSheet(charactersTexture, 0, 0, 1, 1, 1.0f);
    player.sprite = SpriteSheet(charactersTexture, 0, 0, 1.0f/9.0f, 1, 1.3f);
    playerRow = 1;
    
    playerShadow = Entity();
    playerShadow.hitboxHeight = .15f;
    playerShadow.hitboxWidth = .5f;
    playerShadow.sprite = SpriteSheet( shadowTexture, 0, 0, 1, 1, .5f );
    
    SetupSceneRendering();
    distanceTraveled = 0;
    timeLeftWalkAnim = 0;
    timeleftAir = 0;
    playerFrame = 0;
    
    generationTimer = 2;
    timeBetweenGenerations = GEN_BASE_DELAY;
    
    coinChance = GEN_COIN_CHANCE;
    towelChance = GEN_TOWEL_CHANCE;
    crabChance = GEN_CRAB_CHANCE;
    anyChance = GEN_BASE_CHANCE;
    
    coinsCollected = 0;
    score = 0;
    lives = PLR_LIVES;
    
    nextTrail = vector<EntityType>();
}



void CoreApp::UpdateAndRender() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if( event.type == SDL_KEYDOWN) {
            switch (gameStage) {
                case MainMenu:
                    if( event.key.keysym.scancode == SDL_SCANCODE_SPACE || event.key.keysym.scancode == SDL_SCANCODE_RETURN || event.key.keysym.scancode == SDL_SCANCODE_RETURN2 ) {
                        DoStartGame();
                    }
                    break;
                case Game:
                    if( event.key.keysym.scancode == SDL_SCANCODE_A || event.key.keysym.scancode == SDL_SCANCODE_LEFT ) {
                        if( playerRow >= 1 ) {
                            playerRow--;
                        }
                    }
                    if( event.key.keysym.scancode == SDL_SCANCODE_D || event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
                            // player switch right
                        if( playerRow <= 1 ) {
                            playerRow++;
                        }
                    }
                    
                    if( playerRow == 0 ) {
                        player.x = PS_ROWX_0;
                    } else if ( playerRow == 1 ) {
                        player.x = PS_ROWX_1;
                    } else {
                        player.x = PS_ROWX_2;
                    }
                    
                    if( event.key.keysym.scancode == SDL_SCANCODE_SPACE && timeleftAir <= 0 ) {
                        player.visualOffsetY = .5f;
                        timeleftAir = PLR_JUMP_DUR;
                        Mix_PlayChannel( -1, jumpStartSFX, 0);
                    }
                    break;
                case Over:
                    if( event.key.keysym.scancode == SDL_SCANCODE_SPACE || event.key.keysym.scancode == SDL_SCANCODE_RETURN || event.key.keysym.scancode == SDL_SCANCODE_RETURN2 ) {
                        DoMainMenu();
                    }
                    break;
            }
            
            
        } 
    }
    
    float ticks = (float)SDL_GetTicks()/1000.0;
    deltaTime = ticks - lastFrameTicks;
    lastFrameTicks= ticks;
    
    if( gameStage == Game )
        Update();
    
    glClearColor( 0, 0, 0, 1.0f);
    if( clearColorBit )
        glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program->programID);
    
    program->setModelMatrix(modelMatrix);
    program->setViewMatrix(viewMatrix);
    program->setProjectionMatrix(projectionMatrix);
    
    
     // Update and render tiles
    
    
//    viewMatrix.identity();
//    viewMatrix.Translate(player.x, player.y, 0 );
    switch (gameStage) {
        case MainMenu:
            RenderMainMenu();
            break;
        case Game:
            RenderGame();
            break;
        case Over:
            RenderGameover();
            break;
    }
    
    
    SDL_GL_SwapWindow(displayWindow);
}

void CoreApp::RenderMainMenu() {
    RenderScenery();
    titleObj.Render(program);
}

void CoreApp::RenderGame() {
    RenderScenery();
    RenderScoreOverlay();
    
    for( Entity& crab : crabs ){
        if( crab.active )
            crab.Render(program);
    }
    for( Entity& towel : towels ){
        if(towel.active)
            towel.Render(program);
    }
    for( Entity& coin : coins ){
        if( coin.active )
            coin.Render(program);
    }
    
    for( Entity& step : footsteps ){
        if( step.active )
            step.Render(program);
    }
    
    playerShadow.x = player.x;
    playerShadow.y = player.y - .6;
    playerShadow.Render(program);
    player.Render(program);
}

void CoreApp::RenderGameover() {
    RenderScenery();
    RenderScoreOverlay();
    
    gameoverObj.Render(program);
}

void CoreApp::RenderScenery() {
    beachBG.sprite.v += BCH_SCROLLPERSEC * deltaTime;
    beachBG.Render(program);
}

void CoreApp::RenderScoreOverlay() {
    scoreObj.x = -6;
    scoreObj.y = -6;
    scoreObj.Render(program);
    DrawText(fontTexture, to_string(score), .4, 0, -4.4, -5.4);
    DrawText(fontTexture, to_string(lives), .4, 0, -4.4, -6.1);
}

void CoreApp::Update() {
    distanceTraveled += GRD_SCROLLPERSEC * deltaTime;

    for( int i = 0; i < crabs.size(); i++ ) {
            // entities is a pool system
        if( crabs[i].active ) {
                // move with scene
            MoveEntityWithScene(crabs[i]);
                // move crab independent
                // check if colliding with player
            if( playerShadow.IsCollidingWith(crabs[i]))
                CrabCollideWithPlayer(crabs[i]);
        }
    }
    
    for( int i = 0; i < coins.size(); i++ ) {
            // entities is a pool system
        if( coins[i].active ) {
                // move with scene
            MoveEntityWithScene(coins[i]);
                // check if colliding with player
            if( playerShadow.IsCollidingWith(coins[i]))
                CoinCollideWithPlayer(coins[i]);
        }
    }
    
    for( int i = 0; i < footsteps.size(); i++ ) {
            // entities is a pool system
        if( footsteps[i].active ) {
                // move with scene
            MoveEntityWithScene(footsteps[i]);
                // check if colliding with player
        }
    }
    bool anycol = false;
    for( int i = 0; i < towels.size(); i++ ) {
            // entities is a pool system
        if( towels[i].active ) {
            MoveEntityWithScene(towels[i]);
            if( playerShadow.IsCollidingWith(towels[i])) {
                anycol = true;
                TowelCollideWithPlayer(towels[i]);
            }
            
        }
    }
    if( !anycol ) {
        insideObj = false;
    }
    generationTimer -= deltaTime;
    if( generationTimer <= 0 ) {
        generationTimer = timeBetweenGenerations;
        GenerateTrail();
    }
    
    UpdatePlayer();
}

void CoreApp::MoveEntityWithScene(Entity &ent) {
    ent.y += GRD_SCROLLPERSEC * deltaTime;

        // disable if offscene
    if( ent.y >= PS_CUTOFFY ) {
        ent.active = false;
    }
}

void CoreApp::GenerateTrail() {
    int choice = rand() % 100;
    if( choice < anyChance ) {
        anyChance += 7;
        choice = rand() % 100;
        if( choice < 15 ) {
                // 3
            int numCrabs = 0;
            for( int row = 0; row < 3; row++ ) {
                EntityType type = ChooseObject();
                    // failsafe for unbeatable section
                if( type == Crab ) {
                    numCrabs++;
                    if( numCrabs == 3) {
                        type = Beachtowel;
                    }
                }
                MakeObject( type, row);
            }
        } else if( choice < 5 ) {
                // 2
            choice = rand() % 3;
            if( choice == 0 ) {
                for( int row = 0; row < 2; row++ ) {
                    EntityType type = ChooseObject();
                    MakeObject( type, row);
                }
            } else if( choice == 1 ){
                for( int row = 1; row < 3; row++ ) {
                    EntityType type = ChooseObject();
                    MakeObject( type, row);
                }
            } else {
                EntityType type = ChooseObject();
                MakeObject( type, 0);
                type = ChooseObject();
                MakeObject( type, 2);
            }
        } else {
                // 1
            choice = rand() % 3;
            EntityType type = ChooseObject();
            MakeObject( type, choice);
        }
    }
}

EntityType CoreApp::ChooseObject() {
    int choice = rand() % (coinChance + crabChance + towelChance );
    if( choice < coinChance ) {
        return Coin;
    } else if (choice < coinChance + crabChance ) {
        return Crab;
    } else {
        return Beachtowel;
    }
}

void CoreApp::MakeObject(EntityType& type, int row) {
    switch (type) {
        case Crab:
            crabs[lastCrab].active = true;
            crabs[lastCrab].x = RowToX(row);
            crabs[lastCrab].y = PS_STARTY;
            lastCrab = ( lastCrab + 1 ) % GEN_MAX_CRABS;
            break;
        case Coin:
            coins[lastCoin].active = true;
            coins[lastCoin].x = RowToX(row);
            coins[lastCoin].y = PS_STARTY;
            lastCoin = ( lastCoin + 1 ) % GEN_MAX_COINS;
            break;
        case Beachtowel:
            towels[lastTowel].active = true;
            towels[lastTowel].x = RowToX(row);
            towels[lastTowel].y = PS_STARTY;
            lastTowel = ( lastTowel + 1 ) % GEN_MAX_TOWELS;
            break;
        default:
            break;
    }
}

float CoreApp::RowToX( int row ) {
    if( row == 0 ) {
        return PS_ROWX_0;
    } else if( row == 1 ) {
        return PS_ROWX_1;
    } else {
        return PS_ROWX_2;
    }
}

void CoreApp::UpdatePlayer(){
        // Handles: Player control
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if( keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W] ) {
            // move player up
        player.y += GRD_SCROLLPERSEC * deltaTime;
        timeLeftWalkAnim = PLR_ANIM_DELAY;
        
    } // Not else if so that if both keys pressed, players stays still
    if ( keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S] ) {
        
            // move down
        player.y -= GRD_SCROLLPERSEC * 1.5f * deltaTime;
    }
    if( timeleftAir > 0 ) {
        timeleftAir -= deltaTime;
        if( timeleftAir <= 0 ) {
            Mix_PlayChannel( -1, jumpLandSFX, 0);
            player.visualOffsetY = 0;
        }
    } else {
        timeLeftWalkAnim -= deltaTime;
    }
    
    
    if( timeLeftWalkAnim < 0 ) {
        playerFrame++;
        playerFrame = playerFrame % PLR_ANIM_FRAME_COUNT;
        player.sprite.u = (float) playerFrame / PLR_ANIM_FRAME_COUNT;

        timeLeftWalkAnim = PLR_ANIM_DELAY;
        
        if( playerFrame == 4) {
            footsteps[lastStep].active = true;
            footsteps[lastStep].x = playerShadow.x + .18f;
            footsteps[lastStep].y = playerShadow.y;
            lastStep = ( lastStep + 1 ) % GEN_MAX_FOOTPRINTS;
            cout << lastStep << endl;
        } else if( playerFrame == 8) {
            footsteps[lastStep].active = true;
            footsteps[lastStep].x = playerShadow.x - .18f;
            footsteps[lastStep].y = playerShadow.y;
            lastStep = ( lastStep + 1 ) % GEN_MAX_FOOTPRINTS;
        }
    }
    
//    cout << player.sprite.u << endl;
    
//    cout<< player.x << " " << player.y << endl;
}

void CoreApp::CrabCollideWithPlayer( Entity& crab  ) {
//    cout << "Crab collided with player\n";
    crab.active = false;
    Mix_PlayChannel( -1, crabSFX, 0);
    lives--;
    if (lives <= 0 ) {
        DoGameover();
    }
}

void CoreApp::CoinCollideWithPlayer( Entity& coin ) {
//    cout << "Coin collided with player\n";
    coin.active = false;
    Mix_PlayChannel( -1, coinCollectSFX, 0);
    coinsCollected++;
    score++;
        // play sound
        // do particles?
}

void CoreApp::TowelCollideWithPlayer( Entity &towel ) {
//    cout << "Towel collided with player\n";
    if( timeleftAir <= 0 ) {
        towel.active = false;
        lives--;
        Mix_PlayChannel( -1, hitChairSFX, 0);
        if (lives <= 0 ) {
            DoGameover();
        }
    } else {
        if( !insideObj ) {
            score++;
            insideObj = true;
            Mix_PlayChannel( -1, coinCollectSFX, 0);
        }
    }

}

void CoreApp::DoGameover() {
    gameStage = Over;
}

void CoreApp::DoMainMenu() {
    gameStage = MainMenu;
}

void CoreApp::DoStartGame() {
    SetupSceneRendering();
    StartGame();
    gameStage = Game;
}