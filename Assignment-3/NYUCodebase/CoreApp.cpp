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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA,
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
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("INVADERS FROM SPACE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    #ifdef _WINDOWS
        glewInit();
    #endif
    glViewport(0, 0, 512, 512); // divides into 16 16 times
    program = new ShaderProgram( RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl" );
    projectionMatrix.setOrthoProjection(GRD_LEFT, GRD_RIGHT, GRD_BOT, GRD_TOP, -1.0, 1.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    invadersTexture = LoadTexture(RESOURCE_FOLDER"square-invader-sprite-sheet.png");
    fontTexture = LoadTexture(RESOURCE_FOLDER"pixel_font.png");
    
        // EXTRA SETUP GOES HERE
    clearColorBit = true;
    GotoMainMenu();
}

void CoreApp::GotoMainMenu() {
    state = MENU;
    clearColorBit = true;
    score = 0;
    lives = 3;
    player = Entity();
    player.x = GRD_LEFT + 1.5f;
    player.y = GRD_BOT + .5f;
    player.sprite = SpriteSheet(invadersTexture, 16.0f/80.0f, 16.0f/48.0f, 16.0f/80.0f, 16.0f/48.0f, 1.0f);
        // Initialize the object list to all we need
}

void CoreApp::GotoGame() {
    state = GAME;
    
    clearColorBit = true;
    
    enmTickCount = 0;
    enmTickWait = ENM_MOVEMENT_BASE_TICK_TIME;
    enmCurrentDirection = 1.0f;
    enmMoveDown = false;
    enmHitWall = false;
    
    player = Entity();
    player.x = GRD_LEFT + 1.5f;
    player.y = GRD_BOT + .5f;
    player.sprite = SpriteSheet(invadersTexture, 16.0f/80.0f, 16.0f/48.0f, 16.0f/80.0f, 16.0f/48.0f, 1.0f);
    
    enemies.clear();
        // Setup enemies
    for( int row = 0; row < 6; row++ ) {
        for( int column = 0; column < 6; column++ ) {
            
            Entity newEnemy = Entity();
            newEnemy.x = column + -7.0f + .8 * column; // Can't use compiler vars here.
            newEnemy.y = -row + 6.0f;
            if( row <= 1 ) {
                newEnemy.sprite = SpriteSheet(invadersTexture, 0.0f/80.0f, 0.0f/48.0f, 16.0f/80.0f, 16.0f/48.0f, 1.0f);
            } else if( row <= 3 ) {
                newEnemy.sprite = SpriteSheet(invadersTexture, 16.0f/80.0f, 0.0f/48.0f, 16.0f/80.0f, 16.0f/48.0f, 1.0f);
            } else {
                newEnemy.sprite = SpriteSheet(invadersTexture, 0.0f/80.0f, 16.0f/48.0f, 16.0f/80.0f, 16.0f/48.0f, 1.0f);
            }
            enemies.push_back(newEnemy);
            
        }
    }
    
    enmRemaining = ENM_GRID_NUM_ENEMIES;
    
    playerBullet = Entity();
    playerBullet.active = false;
    playerBullet.sprite = SpriteSheet(invadersTexture, 0.0f/80.0f, 32.0f/48.0f, 16.0f/80.0f, 16.0f/48.0f, 1.0f);
    playerBullet.hitboxHeight = .5f;
    playerBullet.hitboxWidth = .2f;
    enemyBullet = Entity();
    enemyBullet.active = false;
    enemyBullet.sprite = SpriteSheet(invadersTexture, 0.0f/80.0f, 32.0f/48.0f, 16.0f/80.0f, 16.0f/48.0f, 1.0f);
}

void CoreApp::GotoGameOver() {
    state = GAMEOVER;
    clearColorBit = true;
}

void CoreApp::UpdateAndRender() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if( event.type == SDL_KEYDOWN) {
            if( event.key.keysym.scancode == SDL_SCANCODE_SPACE && state == GAME ) {
                PlayerShoot();
            } else if( event.key.keysym.scancode == SDL_SCANCODE_RETURN ) {
                if( state == GAMEOVER ) {
                    GotoMainMenu();
                } else if ( state == MENU ) {
                    GotoGame();
                } else {
//                    GotoGameOver();
                }
            }
        }
    }
    
    float ticks = (float)SDL_GetTicks()/1000.0;
    deltaTime = ticks - lastFrameTicks;
    lastFrameTicks= ticks;
    
    
    Update();
    
    glClearColor( .1f, .1f, .1f, 1.0f);
    if( clearColorBit )
        glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program->programID);
    
    program->setModelMatrix(modelMatrix);
    program->setViewMatrix(viewMatrix);
    program->setProjectionMatrix(projectionMatrix);

    switch (state) {
        case MENU:
            DrawText(fontTexture, "INVADERS FROM", 1.0, 0, -7.0f, 4.0f);
            DrawText(fontTexture, "SPACE", 2.0, 0, -6.5f, 2.5f);
            DrawText(fontTexture, "CREATIVE COMMONS EDITION ", 0.3, 0, -7.0f, 1.0f);
            DrawText(fontTexture, "SPACE TO FIRE", 0.3, 0, -7.0f, -5.5f);
            DrawText(fontTexture, "ENTER TO BEGIN", 0.5, 0, -7.0f, -6.0f);
            player.Render( program );
            break;
        case GAME:
            player.Render(program);
            for( Entity& thisEnemy : enemies ) {
                thisEnemy.Render(program);
            }
            playerBullet.Render(program);
            enemyBullet.Render(program);
            DrawText(fontTexture, "SCORE: " + to_string(score), 0.5, 0, -4.0f, 7.0f);
            break;
            
        case GAMEOVER:
            for( Entity& thisEnemy : enemies ) {
                thisEnemy.Render(program);
            }
            DrawText(fontTexture, "GAMEOVER", 1.0, 0, -4.0f, 4.0f);
            DrawText(fontTexture, "SCORE: " + to_string(score), 0.5, 0, -4.0f, 3.0f);
            break;
    }
        // draw objects
    
    
    SDL_GL_SwapWindow(displayWindow);
}



void CoreApp::Update() {
    switch (state) {
        case MENU:
            
            break;
            
        case GAME:
            UpdatePlayer(player);
            enmTickCount += deltaTime;
            if( enmTickCount > enmTickWait ) {
                enmTickCount -= enmTickWait;
                
                for( Entity& thisEnemy : enemies ) {
                    UpdateEnemy(thisEnemy);
                }
                enmMoveDown = false;
                if( enmHitWall ) {
                    enmMoveDown = true;
                    enmCurrentDirection *= -1;
                    enmHitWall = false;
                }
            }
            UpdatePlayerBullet(playerBullet);
            UpdateEnemyBullet(enemyBullet);
            break;
            
        case GAMEOVER:
            
            break;
    }
    
    
}

void CoreApp::UpdatePlayer(Entity& player){
        // Handles: Player control
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if( keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A] ) {
        player.x -= PLR_SPEED * deltaTime;
        
        if( player.x < GRD_LEFT + .5f) {
            player.x = GRD_LEFT + .5f;
        }
    } // Not else if so that if both keys pressed, players stays still
    if ( keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D] ) {
        player.x += PLR_SPEED * deltaTime;
        if( player.x > GRD_RIGHT - .5f ) {
            player.x = GRD_RIGHT - .5f;
        }
        
    }
//    cout<< player.x << " " << player.y << endl;
}

void CoreApp::UpdateEnemy(Entity& enemy){
            // Called during fixedupdate
        // If on cue, move enemy
    if( enemy.active ) {
        if( !enmMoveDown ) {
            enemy.x += ENM_MOVEMENT_AMOUNT * enmCurrentDirection;
            if( enemy.x > GRD_RIGHT - .5 - ENM_MOVEMENT_AMOUNT ||
               enemy.x < GRD_LEFT + .5 + ENM_MOVEMENT_AMOUNT ) {
                enmHitWall = true;
            }
        } else {
            enemy.y -= 1.0f;
            if( enemy.y < ENM_GAMEOVER_Y ) {
                GotoGameOver();
            }
        }
    }
}

void CoreApp::UpdatePlayerBullet(Entity& pBullet) {
            // Called during fixedupdate
    if( !pBullet.active )
        return;
    pBullet.y += BULLET_SPEED * deltaTime;
    if( pBullet.y > GRD_TOP ) {
        pBullet.active = false;
    } else {
        for( Entity& thisEnemy : enemies ) {
            if( pBullet.IsCollidingWith(thisEnemy)) {
                thisEnemy.active = false;
                pBullet.active = false;
                score++;
                enmTickWait += ENM_MOVEMENT_TICK_INCREMENT;
                enmRemaining--;
                if( enmRemaining <= 0 ) {
                    GotoGame();
                }
                break;
            }
        }
    }
        // Move
        // Check collisions
            // Destroy enemy
                // Increase enemy tick
                // +Score
                // Check Win
            // OR Reposition when offscreen
    
}

void CoreApp::UpdateEnemyBullet(Entity& eBullet){
            // Called during fixedupdate
        // Move
        // Check collisions
            // Gameover
    
}

void CoreApp::PlayerShoot() {
    
    if( !playerBullet.active ) {
        playerBullet.active = true;
        playerBullet.x = player.x;
        playerBullet.y = player.y;
    }
}

