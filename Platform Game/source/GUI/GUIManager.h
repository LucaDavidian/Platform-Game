#ifndef GUI_SYSTEM_H
#define GUI_SYSTEM_H

#include "../data structures/queue.hpp"
#include "../data structures/vector.hpp"
#include "../delegates/connection.hpp"

class GUIComponent;

class GUIManager
{
public:
	static GUIManager &GetInstance() 
	{ 
		static GUIManager instance; 
		return instance; 
	}

	~GUIManager() 
	{ 
		for (auto &connection : mConnections) 
			connection.Disconnect(); 
	}

	void AddGUIPanel(GUIComponent *panel, bool alwaysActive = false) 
	{ 
		mGUIPanels.InsertLast(panel); 
		mActivePanel = panel; 
		mAlwaysActive = alwaysActive; 
	}
	
	void RemoveGUIPanel(GUIComponent *panel) 
	{ 
		for (auto it = mGUIPanels.Begin(), end = mGUIPanels.End(); it != end; ++it) 
			if (*it == panel) 
			{ 
				mGUIPanels.Remove(it); 
				return; 
			} 
	}

	Vector<GUIComponent*> &GetGUIPanels() 
	{ 
		return mGUIPanels; 
	}
private:
	GUIManager();

	void OnMouseMove(int x, int y);
	bool OnMousePress(int x, int y);
	bool OnMouseRelease(int x, int y);
	void OnKeyPress(unsigned int key);

	Vector<GUIComponent*> mGUIPanels;
	GUIComponent *mActivePanel;  // GUI system dispatches input events to active panel (if any)

	bool mAlwaysActive = false;

	Vector<Connection> mConnections;
};

#endif  // GUI_SYSTEM_H