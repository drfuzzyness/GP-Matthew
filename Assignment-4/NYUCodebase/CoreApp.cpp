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
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Prettyland", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    #ifdef _WINDOWS
        glewInit();
    #endif
    glViewport(0, 0, 512, 512);
    program = new ShaderProgram( RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl" );
    projectionMatrix.setOrthoProjection(GRD_LEFT, GRD_RIGHT, GRD_BOT, GRD_TOP, -1.0, 1.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    terrainTexture = LoadTexture(RESOURCE_FOLDER"Tiny Platform Quest Tiles_0.png");
    fontTexture = LoadTexture(RESOURCE_FOLDER"pixel_font.png");
    charactersTexture = LoadTexture(RESOURCE_FOLDER"tinyperson.png");
    
        // EXTRA SETUP GOES HERE
    clearColorBit = true;
    StartGame();
}

void CoreApp::StartGame() {
    string path = RESOURCE_FOLDER"map01-3.txt"; // wat r u even doin cpp
    tilemap = Tilemap( path, terrainTexture, program );
    
    player = Entity();
    player.x = tilemap.playerX;
    player.y = tilemap.playerY;
    player.sprite = SpriteSheet(charactersTexture, 0, 0, 1, 1, 1.0f);
}



void CoreApp::UpdateAndRender() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if( event.type == SDL_KEYDOWN) {
            if( event.key.keysym.scancode == SDL_SCANCODE_SPACE ) {
                PlayerJump();
            }
        }
    }
    
    float ticks = (float)SDL_GetTicks()/1000.0;
    deltaTime = ticks - lastFrameTicks;
    lastFrameTicks= ticks;
    
    
    Update();
    
    glClearColor( 0.37, 0.62, 1, 1.0f);
    if( clearColorBit )
        glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program->programID);
    
    program->setModelMatrix(modelMatrix);
    program->setViewMatrix(viewMatrix);
    program->setProjectionMatrix(projectionMatrix);
//    DrawText(fontTexture, "INVADERS FROM", 1.0, 0, -7.0f, 4.0f);
    tilemap.Render();
    
    viewMatrix.identity();
    viewMatrix.Translate(player.x, player.y, 0 );
    
    player.Render(program);
    
    SDL_GL_SwapWindow(displayWindow);
}



void CoreApp::Update() {
    UpdatePlayer(player);
    
}

void CoreApp::UpdatePlayer(Entity& player){
        // Handles: Player control
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if( keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A] ) {
        player.x += PLR_SPEED * deltaTime;
        
    } // Not else if so that if both keys pressed, players stays still
    if ( keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D] ) {
        player.x -= PLR_SPEED * deltaTime;
        
    }
    
    player.y -= player.velocityY * deltaTime;
    
    if( player.velocityY > -PLR_FALL_VEL ) {
        player.velocityY -= PLR_FALL_ACCEL * deltaTime;
    }
    
//    cout<< player.x << " " << player.y << endl;
}

void CoreApp::PlayerJump() {
    player.velocityY = PLR_JUMP_VEL;
}

