#ifndef GUI_PANEL_H
#define GUI_PANEL_H

#include "GUIComposite.h"
#include "glm/glm.hpp"

class GUIPanel : public GUIComposite
{
public:
	GUIPanel(const glm::vec2 &position, const glm::vec2 &dimensions, bool draggable = false) : GUIComposite(position, dimensions), mDraggable(draggable) {}

	void Draw() override;

	bool OnMouseMove(int x, int y, int dx, int dy) override;
	bool OnMousePress(int x, int y) override;
	bool OnMouseRelease(int x, int y) override;

	void SetColor(float red, float green, float blue) { mColor = glm::vec3(red, green, blue); }
	void SetColor(const glm::vec3 &color) { mColor = color; }
	glm::vec3 GetColor() const { return mColor; }

	void SetOpacity(float opacity) { mOpacity = opacity; }
	float GetOpacity() const { return mOpacity; }
private:
	GUIComponent *mSelectedComponent = nullptr;
	bool mDraggable;
	glm::vec3 mColor = glm::vec3(0.5f, 0.5f, 0.5f);
	float mOpacity = 1.0f;
};

#endif  // GUI_PANEL_H