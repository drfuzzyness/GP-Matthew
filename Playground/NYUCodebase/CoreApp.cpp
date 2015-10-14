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
    displayWindow = SDL_CreateWindow("INVADERS FROM SPACE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    #ifdef _WINDOWS
        glewInit();
    #endif
    glViewport(0, 0, 512, 512); // divides into 16 16 times
    program = new ShaderProgram( RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl" );
    projectionMatrix.setOrthoProjection(GRD_LEFT, GRD_TOP, -1.0, GRD_RIGHT, GRD_BOT, 1.0); // We have a 16x16 grid of 16x16s from -8 to 8, defined in header
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
        // EXTRA SETUP GOES HERE
    state = MENU;
}

void CoreApp::GotoMainMenu() {
    state = MENU;
        // Initialize the object list to all we need
}

void CoreApp::UpdateAndRender() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if( event.type == SDL_KEYDOWN) {
            if( event.key.keysym.scancode == SDL_SCANCODE_SPACE ) {
                PlayerShoot();
            } else if( event.key.keysym.scancode == SDL_SCANCODE_RETURN ) {
                if( state != GAME ) {
                        // change state
                }
            }
        }
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
    player.Render(program);
    for( Entity& thisEnemy : enemies ) {
        thisEnemy.Render(program);
    }
    
    SDL_GL_SwapWindow(displayWindow);
}

void CoreApp::Update() {
    for( Entity& thisEnemy : enemies ) {
        UpdateEnemy(thisEnemy);
    }
    
}

void CoreApp::UpdatePlayer(Entity& player){
        // Handles: Player control
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if( keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A] ) {
        player.x -= PLR_SPEED * deltaTime;
        if( player.x < GRD_RIGHT + .5f) {
            player.x = GRD_RIGHT + .5f;
        }
    } // Not else if so that if both keys pressed, players stays still
    if ( keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D] ) {
        player.x += PLR_SPEED * deltaTime;
        if( player.x > GRD_LEFT - .5f) {
            player.x = GRD_LEFT - .5f;
        }
    }
}

void CoreApp::UpdateEnemy(Entity& enemy){
            // Called during fixedupdate
        // If on cue, move enemy
    
}

void CoreApp::UpdatePlayerBullet(Entity& pBullet) {
            // Called during fixedupdate
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

void DrawMap() {
    vector<float> vertexData;
    vector<float>texCoordData;
//    for, for
    float u = (float))(((int)levelData[y][x]) % SPRITE_COUNT_X ) / (float) SPRITE_COUNT_X; // about
    // Y for V
    vertexData.insert( vertexData.end(), {
            // triangles
    });
    
    texCoordData.insert(texCoordData.end(), {
            // uv tris
    });
// endfor endfor
//    render it
// ...
// glDrawArrays(GL_TRIANGLES, 0, vertexData.size()/2);
}

void CoreApp::PlayerShoot() {
    
}

CoreApp* CoreApp::pInstance = NULL;

CoreApp* CoreApp::Instance() {
    if( !pInstance ) {
        pInstance = new CoreApp();
    }
    
    return pInstance;
}

