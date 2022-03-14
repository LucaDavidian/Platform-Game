#ifndef GUI_SLIDER_H
#define GUI_SLIDER_H

#include "GUIComponent.h"
#include "../delegates/signal.hpp"
#include <string>

class GUISlider : public GUIComponent
{
public:
	SIGNAL_ONE_PARAM(CursorMoveEvent, float dp);

	enum Orientation { HORIZONTAL, VERTICAL };
	GUISlider(const glm::vec2 &position, const glm::vec2 &dimensions, Orientation orientation = HORIZONTAL) 
		: GUIComponent(position, dimensions), mCursorPosition(0.0f), mCursorDimension(0.2f), mOrientation(orientation) {}

	void Draw() override;

	bool OnMouseMove(int x, int y, int dx, int dy) override;
	bool OnMousePress(int x, int y) override;
	bool OnMouseRelease(int x, int y) override;

	float GetCursorPosition() const { return mCursorPosition; }
	float GetCursorDimension() const { return mCursorDimension;}

	void SetCursorDimension(float dimension) { mCursorDimension = dimension; }
	void SetCursorPosition(float position) { mCursorPosition = position; }
	
	Orientation GetOrientation() const { return mOrientation; }

	bool IsOver() const { return mIsOver; }

	template <typename T>
	void AddCursorMoveEventListener(T &&object) { mCursorMoveEvent.Bind(std::forward<T>(object)); }
private:
	CursorMoveEvent mCursorMoveEvent;

	float mCursorPosition;     // cursor relative position 0.0f --> 1.0f
	float mCursorDimension;    // cursor relative dimension 0.0f --> 1.0f
	Orientation mOrientation;

	glm::vec3 mColor = { 1.0f, 1.0f, 1.0f };
	glm::vec3 mCursorColor = { 0.0f, 0.0f, 1.0f };

	bool mIsOver = false;
};

#endif  // GUI_SLIDER_H

