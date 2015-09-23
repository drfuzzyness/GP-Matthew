#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "ShaderProgram.h"
#include "Matrix.h"
#include "ParallaxSlide.hpp"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;

GLuint LoadTexture(const char *image_path) {
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

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Forest", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 569, 320, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	#ifdef _WINDOWS
		glewInit();
	#endif
    
    GLuint backTreesID = LoadTexture(RESOURCE_FOLDER"parallax-forest-back-trees.png");
    GLuint midTreesID = LoadTexture(RESOURCE_FOLDER"parallax-forest-middle-trees.png");
    GLuint frontTreesID = LoadTexture(RESOURCE_FOLDER"parallax-forest-front-trees.png");
    
        // ParallaxSlide( float originPos, float startPos, float maxPos, float velocity, GLuint textureID );
    ParallaxSlide slides[] = { ParallaxSlide( -7.1f, 0.0f, 7.1f, .05f, backTreesID ),
        ParallaxSlide( -7.1f, -7.1f, 7.1f, .05f, backTreesID ),
        ParallaxSlide( -7.1f, 0.0f, 7.1f, .10f, midTreesID ),
        ParallaxSlide( -7.1f, -7.1f, 7.1f, .10f, midTreesID ),
        ParallaxSlide( -7.1f, 0.0f, 7.1f, .20f, frontTreesID ),
        ParallaxSlide( -7.1f, -7.1f, 7.1f, .20f, frontTreesID ) };
    
    // Setup goes here
    glViewport(0, 0, 569, 320);
    
    ShaderProgram program = ShaderProgram( RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl" );
    
    Matrix modelMatrix;
    Matrix viewMatrix;
    Matrix projectionMatrix;
    
    projectionMatrix.setOrthoProjection(-3.55, 3.55, -2.0, 2.0, -1.0, 1.0);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SDL_Event event;
	bool done = false;
    float lastFrameTicks = 0;
	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
                // Checks all the SDL event flags
		}
        
        float ticks = (float)SDL_GetTicks()/1000.0;
        float deltaTime = ticks - lastFrameTicks;
        lastFrameTicks= ticks;
        
        glClearColor( .2f, .2f, .2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        program.setModelMatrix(modelMatrix);
        program.setViewMatrix(viewMatrix);
        program.setProjectionMatrix(projectionMatrix);
        
        
//        modelMatrix.identity();
////        modelMatrix.Translate(-1, 0, 0);
//        program.setModelMatrix(modelMatrix);

        glUseProgram(program.programID);
        
        
        float vertices[] = {-3.55f, -2.0f, 3.55f, 2.0f, -3.55f, 2.0f, 3.55f, 2.0f, -3.55f, -2.0f, 3.55f, -2.0f};
        float texCoords[] = {0.0, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0, 0.0, 0.0, 1.0, 1.0, 1.0};
        
//        std::cout << "About to renders";
        
        for( size_t i = 0; i < 6; i++ ) {
//            std::cout << "trying to move";
            slides[i].xPos += slides[i].velocity * deltaTime;
//            std::cout << deltaTime;
            if( slides[i].xPos > slides[i].maxXPos ) {
                slides[i].xPos = slides[i].originXPos;
//                std::cout << "Slide position reset";
            }
            modelMatrix.identity();
            modelMatrix.Translate( slides[i].xPos , 0, 0);
            program.setModelMatrix(modelMatrix);
            
            glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
            glEnableVertexAttribArray(program.positionAttribute);
            
            glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
            glEnableVertexAttribArray(program.texCoordAttribute);
            
            glBindTexture(GL_TEXTURE_2D, slides[i].texture);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            
            glDisableVertexAttribArray(program.positionAttribute);
            glDisableVertexAttribArray(program.texCoordAttribute);

            
        }
        
        std::cout << slides[0].xPos;
        
		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}
