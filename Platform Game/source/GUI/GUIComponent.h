#ifndef GUI_COMPONENT_H
#define GUI_COMPONENT_H

#include <glm/glm.hpp>
#include "../data structures/vector.hpp"
#include "../data structures/stack.hpp"

//class GUIVisitor
//{
//public:
//	virtual void Visit(class GUIPanel *component) {}
//	virtual void Visit(class GUIButton *component) {}
//	virtual void Visit(class GUIToggleButton *component) {}
//	virtual void Visit(class GUIButtonGroup *component) {}
//	virtual void Visit(class GUITextBox *component) {}
//	virtual void Visit(class GUILabel *component) {}
//	virtual void Visit(class GUIImage *component) {}
//	virtual void Visit(class GUIInventory *component) {}
//	virtual void Visit(class GUISlider *component) {}
//};

class GUIComponent
{
public:
	virtual ~GUIComponent() = default;

	void SetParent(GUIComponent *parent) { mParent = parent; }

	enum class AnchorPoint { TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT, CENTER };     // default anchor point is bottom left for this component and for component's parent
	void SetAchorPoint(AnchorPoint anchorPoint = AnchorPoint::BOTTOM_LEFT, AnchorPoint parentAnchorPoint = AnchorPoint::BOTTOM_LEFT);

	void SetRelativePosition(float x, float y) { SetRelativePosition(glm::vec2(x, y)); }   // set position of current anchor point of quad relative to parent's anchor point (percent)
	void SetRelativePosition(const glm::vec2 position);         
	
	void SetRelativeDimensions(float width, float height) { SetRelativeDimensions(glm::vec2(width, height)); }   // set width and height of quad relative to parent's (percent)
	void SetRelativeDimensions(const glm::vec2 dimensions);    
	
	void AddScreenPositionOffset(int dx, int dy);

	glm::vec2 GetRelativePositionBL() const { return mRelativePosition; }
	glm::vec2 GetRelativePositionBR() const { return glm::vec2(mRelativePosition.x + mRelativeDimensions.x, mRelativePosition.y); }
	glm::vec2 GetRelativePositionTL() const { return glm::vec2(mRelativePosition.x, mRelativePosition.y + mRelativeDimensions.y); }
	glm::vec2 GetRelativePositionTR() const { return glm::vec2(mRelativePosition.x + mRelativeDimensions.x, mRelativePosition.y + mRelativeDimensions.y); }
	glm::vec2 GetRelativePositionCenter() const { return glm::vec2(mRelativePosition.x + mRelativeDimensions.x / 2.0f, mRelativePosition.y + mRelativeDimensions.y / 2.0f); }

	glm::vec2 GetNormalizedPositionBL() const { return mNormalizedPosition; }
	glm::vec2 GetNormalizedPositionBR() const { return glm::vec2(mNormalizedPosition.x + mNormalizedDimensions.x, mNormalizedPosition.y); }
	glm::vec2 GetNormalizedPositionTL() const { return glm::vec2(mNormalizedPosition.x, mNormalizedPosition.y + mNormalizedDimensions.y); }
	glm::vec2 GetNormalizedPositionTR() const { return glm::vec2(mNormalizedPosition.x + mNormalizedDimensions.x, mNormalizedPosition.y + mNormalizedDimensions.y); }
	glm::vec2 GetNormalizedPositionCenter() const { return glm::vec2(mNormalizedPosition.x + mNormalizedDimensions.x / 2.0f, mNormalizedPosition.y + mNormalizedDimensions.y / 2.0f); }

	glm::vec2 GetScreenPositionBL() const { return mScreenPosition; }
	glm::vec2 GetScreenPositionBR() const { return glm::vec2(mScreenPosition.x + mScreenDimensions.x, mScreenPosition.y); }
	glm::vec2 GetScreenPositionTL() const { return glm::vec2(mScreenPosition.x, mScreenPosition.y + mScreenDimensions.y); }
	glm::vec2 GetScreenPositionTR() const { return glm::vec2(mScreenPosition.x + mScreenDimensions.x, mScreenPosition.y + mScreenDimensions.y); }
	glm::vec2 GetScreenPositionCenter() const { return glm::vec2(mScreenPosition.x + mScreenDimensions.x / 2.0f, mScreenPosition.y + mScreenDimensions.y / 2.0f); }
	
	glm::vec2 GetRelativeDimensions() const { return mRelativeDimensions; }
	glm::vec2 GetNormalizedDimensions() const { return mNormalizedDimensions; }
	glm::vec2 GetScreenDimensions() const { return mScreenDimensions; }

	virtual bool IsInside(int x, int y) const { return (x >= mScreenPosition.x && x <= mScreenPosition.x + mScreenDimensions.x && y >= mScreenPosition.y && y <= mScreenPosition.y + mScreenDimensions.y); }
	virtual void Update() { UpdateComponent(); }
	virtual void Draw() = 0;
	
	void SetVisible(bool visible) { mIsVisible = visible; }
	bool IsVisible() const { return mIsVisible; }

	// GUI input events, dispatched by GUI system (InputSystem --> GUISystem --> GUIComponent). If a GUI component consumes an event it returns true otherwise it returns false (default implementation)
	virtual bool OnMouseMove(int x, int y, int dx, int dy) { return false; };
	virtual bool OnMousePress(int x, int y) { return false; };
	virtual bool OnMouseRelease(int x, int y) { return false; };
	virtual bool OnKeyPress(unsigned int key) { return false; }
private:
	void UpdateComponent();  // this member function is called from constructor (so it can't be virtual) and Update()
protected:
	GUIComponent(const glm::vec2 &position = glm::vec2(0.0f, 0.0f), const glm::vec2 &dimensions = glm::vec2(1.0f, 1.0f));

	GUIComponent *mParent;               // parent is viewport if null

	glm::vec2 mRelativePosition;         // position of bottom left corner of quad relative to bottom left corner of parent (percent)
	glm::vec2 mRelativeDimensions;       // width and height of quad relative to parent's (percent)

	glm::vec2 mNormalizedPosition;       // position of bottom left corner of quad in normalized device coordinates
	glm::vec2 mNormalizedDimensions;     // width and height of quad in normalized device coordinates

	glm::ivec2 mScreenPosition;          // position of bottom left corner of quad in screen coordinates (origin bottom left)
	glm::ivec2 mScreenDimensions;        // width and height of quad in screen coordinates

	AnchorPoint mAnchorPoint;
	AnchorPoint mParentAnchorPoint;

	bool mIsFocused = false;
	bool mIsSelected = false;

	bool mIsVisible = true;
};

#endif  // GUI_COMPONENT_H