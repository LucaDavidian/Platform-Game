#include "GUIManager.h"
#include "GUIComponent.h"
#include "../Input/InputManager.h"

GUIManager::GUIManager()
{
	mConnections.InsertLast(InputManager::GetInstance().RegisterMouseMoveEvent(*this, &GUIManager::OnMouseMove));
	mConnections.InsertLast(InputManager::GetInstance().RegisterMousePressEvent(*this, &GUIManager::OnMousePress, 0));
	mConnections.InsertLast(InputManager::GetInstance().RegisterMouseReleaseEvent(*this, &GUIManager::OnMouseRelease, 0));
	mConnections.InsertLast(InputManager::GetInstance().RegisterKeyPressEvent(*this, &GUIManager::OnKeyPress));
}

void GUIManager::OnMouseMove(int x, int y)
{
	static int prevX = x, prevY = y;   // save previous mouse position (for deltas)

	if (mActivePanel)
	{
		int deltaX = x - prevX;
		int deltaY = y - prevY;

		mActivePanel->OnMouseMove(x, y, deltaX, deltaY);
	}

	prevX = x;
	prevY = y;
}

bool GUIManager::OnMousePress(int x, int y)
{
	if (mActivePanel && mActivePanel->IsInside(x, y))  // inside current active panel
	{
		mActivePanel->OnMousePress(x, y);

		return true;  // consume input
	}
	else  // no current active panel or outside current active panel
	{
		if (mActivePanel && mAlwaysActive)
			return false;

		mActivePanel = nullptr;

		for (GUIComponent *panel : mGUIPanels)
		{
			if (panel->IsInside(x, y))  // panels are ordered back to front (for rendering)
				mActivePanel = panel;
		}

		if (mActivePanel)  // if new panel is active, move to front
		{
			mGUIPanels.Remove(mGUIPanels.Find(mActivePanel));
			mGUIPanels.InsertLast(mActivePanel);

			if (mActivePanel->OnMousePress(x, y))
				return true;  // consume input
		}
	}

	return false;  // don't consume input if mouse out of GUI
}

bool GUIManager::OnMouseRelease(int x, int y)
{
	if (mActivePanel)
		if (mActivePanel->OnMouseRelease(x, y))
			return true;

	return false;
}

void GUIManager::OnKeyPress(unsigned int key)
{
	if (mActivePanel)
		mActivePanel->OnKeyPress(key);
}