//
//  Tilemap.cpp
//  NYUCodebase
//
//  Created by Matthew Conto
//

#include "Tilemap.h"

Tilemap::Tilemap() {}

Tilemap::Tilemap( string& path, GLuint textureID, ShaderProgram* program ) : textureID(textureID), program(program) {
    ReadFromFile(path);
    PrepareLevel();
}

void Tilemap::PrepareLevel() {
    float spriteWidth = 1.0f/(float)SPRITE_COUNT_X;
    float spriteHeight = 1.0f/(float)SPRITE_COUNT_Y;
    for(int y=0; y < LEVEL_HEIGHT; y++) {
        for(int x=0; x < LEVEL_WIDTH; x++) {
            float u = (float)(((int)levelData[y][x]) % SPRITE_COUNT_X) / (float) SPRITE_COUNT_X;
            float v = (float)(((int)levelData[y][x]) / SPRITE_COUNT_X) / (float) SPRITE_COUNT_Y;
            
            vertexData.insert(vertexData.end(), {
                TILE_SIZE * x, -TILE_SIZE * y,
                TILE_SIZE * x, (-TILE_SIZE * y)-TILE_SIZE,
                (TILE_SIZE * x)+TILE_SIZE, (-TILE_SIZE * y)-TILE_SIZE,
                TILE_SIZE * x, -TILE_SIZE * y,
                (TILE_SIZE * x)+TILE_SIZE, (-TILE_SIZE * y)-TILE_SIZE,
                (TILE_SIZE * x)+TILE_SIZE, -TILE_SIZE * y
            });
            
            texCoordData.insert(texCoordData.end(), {
                u, v,
                u, v+(spriteHeight),
                u+spriteWidth, v+(spriteHeight),
                u, v,
                u+spriteWidth, v+(spriteHeight),
                u+spriteWidth, v
            });
        }
    }
    numVerts = vertexData.size() / 2;
}

void Tilemap::ReadFromFile( const std::string &path ) {
        // Do I feel guilty that I'm just ripping code from the slides?
        // A bit
        // Is C++ hard?
        // Yes.
        // Is it worth it to understand how game engines work games?
        // Yes.
        // Do I want to be using Unity right now?
        // Debug.log("Yes.");
    ifstream infile( path );
    string line;
    while (getline(infile, line)) {
        if(line == "[header]") {
            // I assume all my maps are the same size.
            // I do this because I was having issues with the levelData[x][y]
        } else if(line == "[layer]") {
            readLayerData(infile);
        } else if(line == "[Object Layer 1]") {
            readEntityData(infile);
        }
    }
}

bool Tilemap::readLayerData(ifstream &stream) {
    string line;
    while(getline(stream, line)) {
        if(line == "") { break; }
        istringstream sStream(line);
        string key,value;
        getline(sStream, key, '=');
        getline(sStream, value);
        if(key == "data") {
            for(int y=0; y < LEVEL_HEIGHT; y++) {
                getline(stream, line);
                istringstream lineStream(line);
                string tile;
                for(int x=0; x < LEVEL_WIDTH; x++) {
                    getline(lineStream, tile, ',');
                    unsigned char val =  (unsigned char)atoi(tile.c_str());
                    if(val > 0) {
                        levelData[y][x] = val-1;
                    } else {
                        levelData[y][x] = 0;
                    }
                }
            } }
    }
    return true;
}

bool Tilemap::readEntityData(std::ifstream &stream) {
    string line;
    string type;
    while(getline(stream, line)) {
        if(line == "") { break; }
        istringstream sStream(line);
        string key,value;
        getline(sStream, key, '=');
        getline(sStream, value);
        if(key == "type") {
            type = value; // it'll say "start", don't worry, me
        } else if(key == "location") {
            istringstream lineStream(value);
            string xPosition, yPosition;
            getline(lineStream, xPosition, ',');
            getline(lineStream, yPosition, ',');
            
//            playerX = atoi(xPosition.c_str());
//            playerY = atoi(yPosition.c_str());
//            placeEntity(type, placeX, placeY);
            
        }
    }
    return true;
}

void Tilemap::Render() {
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    glDrawArrays(GL_TRIANGLES, 0, numVerts );

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}
