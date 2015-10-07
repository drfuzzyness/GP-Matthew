//
//  Entity.hpp
//  NYUCodebase
//
//  Created by Matthew Conto on 9/23/15.
//

#ifndef ENTITY_H
#define ENTITY_H
#include "CoreApp.hpp"

//using namespace std;

//class CoreApp;

class Entity {
public:
    void Draw();
    
    float x;
    float y;
    float rotation;
    
    GLuint textureID;
    
    float width;
    float height;
    
    float hitboxWidth;
    float hitboxHeight;
    
    Entity( GLuint textureID, float x, float y, float rotation, float width, float height );

private:
    std::vector<float> verticies;
    std::vector<float> texCoords;
    CoreApp* core;
};
#endif