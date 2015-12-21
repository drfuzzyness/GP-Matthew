//
//  Entity.cpp
//  NYUCodebase
//
//  Created by Matthew Conto
//

#include "Entity.h"
#include "Tilemap.h"

Entity::Entity() :
    active(true),
    x(0.0f),
    y(0.0f),
    visualOffsetY(0.0f),
    width(1.0f),
    height(1.0f),
    hitboxHeight(1.0f),
    hitboxWidth(1.0f),
    velocityX(0.0f),
    velocityY(0.0f),
    type(Crab)
    {
}

void Entity::Render( ShaderProgram* program ) {
    if( active ) {
        Matrix modelMatrix;
        modelMatrix.Translate(x, y + visualOffsetY, 0.0f);
        program->setModelMatrix( modelMatrix );
        
        sprite.Draw( program );
    }
}

void Entity::ResolveCollision( float tile_x, float tile_y ) {
    if( y - hitboxHeight/2 < -TILE_SIZE * tile_y ) {
            // TOP OF TILE
        
    }
    bool collision = !( x - hitboxWidth/2 > (TILE_SIZE * tile_x) + TILE_SIZE ||
                       x + hitboxWidth/2 < (TILE_SIZE * tile_x)              ||
                       y - hitboxHeight/2 > -TILE_SIZE * tile_y              ||
                       y + hitboxHeight/2 < (-TILE_SIZE * tile_y) - TILE_SIZE
                      );
    
}

void ResolveCollision( Tilemap& other ) {
    
}

bool Entity::IsCollidingWith(Entity& other) {
    if ( active && other.active ) {
        if(
           x - hitboxWidth/2 > other.x + other.hitboxWidth/2 ||
           x + hitboxWidth/2 < other.x - other.hitboxWidth/2 ||
           y - hitboxHeight/2 > other.y + other.hitboxHeight/2 ||
           y + hitboxHeight/2 < other.y - other.hitboxHeight/2 ) {
            return false;
        }
        return true;
    }
    return false;
}