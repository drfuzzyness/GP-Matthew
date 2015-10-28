//
//  Tilemap.h
//  NYUCodebase
//
//  Created by Matthew Conto
//

#pragma once
#include <vector>

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "ShaderProgram.h"

#define LEVEL_HEIGHT 23
#define LEVEL_WIDTH 40
#define TILE_SIZE 1.0f

using namespace std;

class Tilemap {
public:
    Tilemap();
    Tilemap( string& path, GLuint textureID, ShaderProgram* program );
    unsigned char levelData[LEVEL_HEIGHT][LEVEL_WIDTH];
    vector<char> airTiles;
    int SPRITE_COUNT_X = 16;
    int SPRITE_COUNT_Y = 16;
    float playerX = -6;
    float playerY = 15;
    
    void ReadFromFile( const std::string& path );
    void PrepareLevel();
    void Render();
private:
    GLuint textureID;
    ShaderProgram* program;
    bool readLayerData(ifstream &stream);
    bool readEntityData(ifstream &stream);
    vector<float> vertexData;
    vector<float> texCoordData;
    int numVerts;
};
