#include "GUIComponent.h"
#include "../GraphicsManager.h"

GUIComponent::GUIComponent(const glm::vec2 &position, const glm::vec2 &dimensions) 
	: mParent(nullptr), mRelativePosition(position), mRelativeDimensions(dimensions), mNormalizedPosition(0.0f, 0.0f), mNormalizedDimensions(0.0f, 0.0f), mAnchorPoint(AnchorPoint::BOTTOM_LEFT), mParentAnchorPoint(AnchorPoint::BOTTOM_LEFT)
{
	UpdateComponent();
}

void GUIComponent::UpdateComponent()
{
	if (!mParent) // parent is viewport
	{
		mNormalizedPosition.x = -1.0f + 2.0f * mRelativePosition.x;  
		mNormalizedPosition.y = -1.0f + 2.0f * mRelativePosition.y;
		
		mNormalizedDimensions.x = 2.0f * mRelativeDimensions.x;  
		mNormalizedDimensions.y = 2.0f * mRelativeDimensions.y;  

		mScreenPosition.x = GraphicsManager::GetInstance().GetViewportX() + (mNormalizedPosition.x + 1.0f) * GraphicsManager::GetInstance().GetViewportWidth() / 2.0f;     // mScreenPosition.x = viewportX + viewportWidth * mRelativePosition.x;
		mScreenPosition.y = GraphicsManager::GetInstance().GetViewportY() + (mNormalizedPosition.y + 1.0f) * GraphicsManager::GetInstance().GetViewportHeight() / 2.0f;    // mScreenPosition.y = viewportY + viewportHeight * mRelativePosition.y;
		
		mScreenDimensions.x = mNormalizedDimensions.x / 2.0f * GraphicsManager::GetInstance().GetViewportWidth();                                                         // mScreenDimensions.x = viewportWidth * mRelativeDimensions.x;
		mScreenDimensions.y = mNormalizedDimensions.y / 2.0f * GraphicsManager::GetInstance().GetViewportHeight();                                                        // mScreenDimensions.y = viewportHeight * mRelativeDimensions.y;
	}
	else  // parent is GUI component
	{
		mNormalizedPosition.x = mParent->mNormalizedPosition.x + mParent->mNormalizedDimensions.x * mRelativePosition.x;
		mNormalizedPosition.y = mParent->mNormalizedPosition.y + mParent->mNormalizedDimensions.y * mRelativePosition.y;
		
		mNormalizedDimensions.x = mParent->mNormalizedDimensions.x * mRelativeDimensions.x;
		mNormalizedDimensions.y = mParent->mNormalizedDimensions.y * mRelativeDimensions.y;

		mScreenPosition.x = mParent->mScreenPosition.x + mParent->mScreenDimensions.x * mRelativePosition.x;
		mScreenPosition.y = mParent->mScreenPosition.y + mParent->mScreenDimensions.y * mRelativePosition.y;
		
		mScreenDimensions.x = mParent->mScreenDimensions.x * mRelativeDimensions.x;
		mScreenDimensions.y = mParent->mScreenDimensions.y * mRelativeDimensions.y;
	}
}

void GUIComponent::SetAchorPoint(AnchorPoint anchorPoint /*= AnchorPoint::BOTTOM_LEFT*/, AnchorPoint parentAnchorPoint /*= AnchorPoint::BOTTOM_LEFT*/)
{
	mAnchorPoint = anchorPoint; 
	mParentAnchorPoint = parentAnchorPoint; 

	SetRelativePosition(mRelativePosition);
}

// set relative position of selected corner of component relative to parent's selected corner:
// - mRelativePosition is always the position of bottom left corner of component relative to the parent's bottom left corner 
// - this function sets the position of this component's anchor point relative to parent's anchor point
// - relative position of this component's anchor point is indicated in parent's anchor point relative coordinates (axis pointing inside the parent's component):
//
// TL --->      <--- TR 
//  |       ^        |
//  v       |        v
//          C-->
//  ^                ^
//  |                |
// BL --->      <--- BR
void GUIComponent::SetRelativePosition(const glm::vec2 position)
{
	switch (mAnchorPoint)
	{
		case AnchorPoint::BOTTOM_LEFT:
			switch (mParentAnchorPoint)
			{
				case AnchorPoint::BOTTOM_LEFT:
					mRelativePosition = glm::vec2(position.x, position.y);
					break;
				case AnchorPoint::BOTTOM_RIGHT:
					mRelativePosition = glm::vec2(1.0f - position.x, position.y);
					break;
				case AnchorPoint::TOP_LEFT:
					mRelativePosition = glm::vec2(position.x, 1.0f - position.y);
					break;
				case AnchorPoint::TOP_RIGHT:
					mRelativePosition = glm::vec2(1.0f - position.x, 1.0f - position.y);
					break;
				case AnchorPoint::CENTER:
					mRelativePosition = glm::vec2(0.5f + position.x, 0.5f + position.y);
					break;
			}
			break;
		case AnchorPoint::BOTTOM_RIGHT:
			switch (mParentAnchorPoint)
			{
				case AnchorPoint::BOTTOM_LEFT:
					mRelativePosition = glm::vec2(position.x - mRelativeDimensions.x, position.y);
					break;
				case AnchorPoint::BOTTOM_RIGHT:
					mRelativePosition = glm::vec2(1.0f - position.x - mRelativeDimensions.x, position.y);
					break;
				case AnchorPoint::TOP_LEFT:
					mRelativePosition = glm::vec2(position.x - mRelativeDimensions.x, 1.0f - position.y);
					break;
				case AnchorPoint::TOP_RIGHT:
					mRelativePosition = glm::vec2(1.0f - position.x - mRelativeDimensions.x, 1.0f - position.y);
					break;
				case AnchorPoint::CENTER:
					mRelativePosition = glm::vec2(0.5f + position.x - mRelativeDimensions.x, 0.5f + position.y);
					break;
			}
			break;
		case AnchorPoint::TOP_LEFT:
			switch (mParentAnchorPoint)
			{
				case AnchorPoint::BOTTOM_LEFT:
					mRelativePosition = glm::vec2(position.x, position.y - mRelativeDimensions.y);
					break;
				case AnchorPoint::BOTTOM_RIGHT:
					mRelativePosition = glm::vec2(1.0f - position.x, position.y - mRelativeDimensions.y);
					break;
				case AnchorPoint::TOP_LEFT:
					mRelativePosition = glm::vec2(position.x, 1.0f - position.y - mRelativeDimensions.y);
					break;
				case AnchorPoint::TOP_RIGHT:
					mRelativePosition = glm::vec2(1.0f - position.x, 1.0f - position.y - mRelativeDimensions.y);
					break;
				case AnchorPoint::CENTER:
					mRelativePosition = glm::vec2(0.5f + position.x, 0.5f + position.y - mRelativeDimensions.y);
					break;
			}		
			break;
		case AnchorPoint::TOP_RIGHT:
			switch (mParentAnchorPoint)
			{
				case AnchorPoint::BOTTOM_LEFT:
					mRelativePosition = glm::vec2(position.x - mRelativeDimensions.x, position.y - mRelativeDimensions.y);
					break;
				case AnchorPoint::BOTTOM_RIGHT:
					mRelativePosition = glm::vec2(1.0f - position.x - mRelativeDimensions.x, position.y - mRelativeDimensions.y);
					break;
				case AnchorPoint::TOP_LEFT:
					mRelativePosition = glm::vec2(position.x - mRelativeDimensions.x, 1.0f - position.y - mRelativeDimensions.y);
					break;
				case AnchorPoint::TOP_RIGHT:
					mRelativePosition = glm::vec2(1.0f - position.x - mRelativeDimensions.x, 1.0f - position.y - mRelativeDimensions.y);
					break;
				case AnchorPoint::CENTER:
					mRelativePosition = glm::vec2(0.5f + position.x - mRelativeDimensions.x, 0.5f + position.y - mRelativeDimensions.y);
					break;
			}
			break;
		case AnchorPoint::CENTER:
			switch (mParentAnchorPoint)
			{
				case AnchorPoint::BOTTOM_LEFT:
					mRelativePosition = glm::vec2(position.x - mRelativeDimensions.x / 2.0f, position.y - mRelativeDimensions.y / 2.0f);
					break;
				case AnchorPoint::BOTTOM_RIGHT:
					mRelativePosition = glm::vec2(1.0f - position.x - mRelativeDimensions.x / 2.0f, position.y - mRelativeDimensions.y / 2.0f);
					break;
				case AnchorPoint::TOP_LEFT:
					mRelativePosition = glm::vec2(position.x - mRelativeDimensions.x / 2.0f, 1.0f - position.y - mRelativeDimensions.y / 2.0f);
					break;
				case AnchorPoint::TOP_RIGHT:
					mRelativePosition = glm::vec2(1.0f - position.x - mRelativeDimensions.x / 2.0f, 1.0f - position.y - mRelativeDimensions.y / 2.0f);
					break;
				case AnchorPoint::CENTER:
					mRelativePosition = glm::vec2(0.5f + position.x - mRelativeDimensions.x / 2.0f, 0.5f + position.y - mRelativeDimensions.y / 2.0f);
					break;
			}
			break;
	}

	Update();
}

void GUIComponent::SetRelativeDimensions(const glm::vec2 dimensions)
{
	switch (mAnchorPoint)
	{
		case AnchorPoint::TOP_LEFT:
			mRelativePosition.y += mRelativeDimensions.y - dimensions.y;
			break;
		case AnchorPoint::TOP_RIGHT:
			mRelativePosition += mRelativeDimensions - dimensions;
			break;
		case AnchorPoint::BOTTOM_LEFT:
			break;
		case AnchorPoint::BOTTOM_RIGHT:
			mRelativePosition.x += mRelativeDimensions.x - dimensions.x;
			break;
		case AnchorPoint::CENTER:
			mRelativePosition += (mRelativeDimensions - dimensions) / 2.0f;
			break;
	}

	mRelativeDimensions = dimensions;

	Update();
}

void GUIComponent::AddScreenPositionOffset(int dx, int dy)
{
	if (!mParent)
	{
		mRelativePosition.x += (float)dx / GraphicsManager::GetInstance().GetViewportWidth();
		mRelativePosition.y += (float)dy / GraphicsManager::GetInstance().GetViewportHeight();
	}
	else
	{
		mRelativePosition.x += (float)dx / mParent->mScreenDimensions.x;
		mRelativePosition.y += (float)dy / mParent->mScreenDimensions.y;
	}

	Update();
}

