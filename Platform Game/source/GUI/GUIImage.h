#ifndef GUI_IMAGE_H
#define GUI_IMAGE_H

#include "GUIComponent.h"
#include <string>
#include "../delegates/signal.hpp"

class Texture;

class GUIImage : public GUIComponent
{
public:
	GUIImage(const glm::vec2 &position, const glm::vec2 &dimensions, const std::string &image = "");

	void Draw() override;

	bool OnMouseMove(int x, int y, int dx, int dy) override;
	bool OnMousePress(int x, int y) override;

	void SetImage(const std::string &image);
	std::string const &GetImage() const { return mImage; }
	void RemoveImage();

	void SetColor(glm::vec3 color) { mColor = color; }
	glm::vec3 GetColor() const { return mColor; }

	Texture *GetTexture() const { return mTexture; }
	
	bool IsOver() { return mIsOver; } bool mIsOver = false;   

	template <typename T>
	Connection AddImageHoverListener(T &listener, void (T::*PtrToMemFun)(const std::string&)) { return mImageHover.Bind(listener, PtrToMemFun); }

	template <typename T>
	Connection AddImageClickedListener(T &listener, void (T::*PtrToMemFun)(const std::string&)) { return mImageClicked.Bind(listener, PtrToMemFun); }
private:
	SIGNAL_ONE_PARAM(ImageHover, const std::string&);
	ImageHover mImageHover;

	SIGNAL_ONE_PARAM(ImageClicked, const std::string&);
	ImageClicked mImageClicked;

	std::string mImage;
	Texture *mTexture;
	glm::vec3 mColor{ 0.3f, 0.5f, 0.3f };
};

#endif // GUI_IMAGE_H