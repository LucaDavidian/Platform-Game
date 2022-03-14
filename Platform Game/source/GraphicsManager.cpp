#include "GraphicsManager.h"
#include <glew/GL/glew.h>

GraphicsManager::~GraphicsManager()
{
	SDL_GL_DeleteContext(mGLContext);
	SDL_DestroyWindow(mWindow);
}

void GraphicsManager::Initialize()
{
	SDL_Init(SDL_INIT_VIDEO);

	mWindow = SDL_CreateWindow("Game - FPS: ", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWindowWidth, mWindowHeight, SDL_WINDOW_OPENGL);
	mGLContext = SDL_GL_CreateContext(mWindow);

	glewInit();

	SetViewport(0, 0, mWindowWidth, mWindowHeight, 0.0f, 100.0f);  // OpengGL origin is bottom left corner
}

void GraphicsManager::Clear() const 
{ 
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
	glClear(GL_COLOR_BUFFER_BIT); 
}

void GraphicsManager::SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height, float near, float far)
{
	mViewportX = x;
	mViewportY = y;
	mViewportWidth = width;
	mViewportHeight = height;

	glViewport(0, 0, width, height);  // lower left corner of viewport

	glDepthRangef(near, far);
}