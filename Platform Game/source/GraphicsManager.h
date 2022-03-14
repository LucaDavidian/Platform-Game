#ifndef GRAPHICS_SYSTEM_H
#define GRAPHICS_SYSTEM_H

#include <string>
#include <glew/GL/glew.h>
#include <SDL2/SDL.h>

class GraphicsManager
{
public:
	static GraphicsManager& GetInstance() { static GraphicsManager instance; return instance; }
	~GraphicsManager();

	void Initialize();

	void Clear() const;
	void Present() const { SDL_GL_SwapWindow(mWindow); }

	struct SDL_Window* GetWindow() const { return mWindow; }
	void SetWindowsTitle(std::string const &title) { SDL_SetWindowTitle(mWindow, title.c_str()); }
	std::string GetWindowTitle() const { return SDL_GetWindowTitle(mWindow); }
	unsigned GetWindowWidth() const { return mWindowWidth; }
	unsigned GetWindowHeight() const { return mWindowHeight; }

	void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height, float near, float far);
	unsigned GetViewportX() const { return mViewportX; }
	unsigned GetViewportY() const { return mViewportY; }
	unsigned GetViewportWidth() const { return mViewportWidth; }
	unsigned GetViewportHeight() const { return mViewportHeight; }

	void EnableBlending() const { glEnable(GL_BLEND); }
	void DisableBlending() const { glDisable(GL_BLEND); }
private:
	GraphicsManager() = default;
	SDL_Window* mWindow;
	SDL_GLContext mGLContext;
	unsigned int mWindowWidth = 1024;
	unsigned int mWindowHeight = 768;
	unsigned int mViewportX;
	unsigned int mViewportY;
	unsigned int mViewportWidth;
	unsigned int mViewportHeight;
};

#endif  // GRAPHICS_SYSTEM