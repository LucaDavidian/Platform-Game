#ifndef GUI_COMPOSITE_H
#define GUI_COMPOSITE_H

#include "GUIComponent.h"
#include "../data structures/vector.hpp"

class GUIComposite : public GUIComponent
{
public:
	~GUIComposite() { for (GUIComponent *component : mComponents) delete component; }

	void AddGuiComponent(GUIComponent *component);
	void RemoveGUIComponent(GUIComponent *component);

	bool IsInside(int x, int y) const override;
	void Update() override;
	void Draw() override;

	// GUI composite dispatches events to its child components and returns true if any of them consumes the input
	bool OnMouseMove(int x, int y, int dx, int dy) override;
	bool OnMousePress(int x, int y) override;
	bool OnMouseRelease(int x, int y) override;
	bool OnKeyPress(unsigned int key) override;
	
	Vector<GUIComponent*> const &GetComponents() const { return mComponents; }
protected:
	GUIComposite(const glm::vec2 &position, const glm::vec2 &dimensions) : GUIComponent(position, dimensions) {}
	Vector<GUIComponent*> mComponents;
};

#endif  // GUI_COMPOSITE_H
