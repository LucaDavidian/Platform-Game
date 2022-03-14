#include "GUIComposite.h"

void GUIComposite::AddGuiComponent(GUIComponent *component)
{
	component->SetParent(this);
	component->Update();
	mComponents.InsertLast(component);
}

void GUIComposite::RemoveGUIComponent(GUIComponent *component)
{
	for (auto it = mComponents.Begin(); it != mComponents.End(); ++it)
		if (*it == component)
		{
			(*it)->SetParent(nullptr);
			mComponents.Remove(it);
			break;
		}
}

bool GUIComposite::IsInside(int x, int y) const
{
	for (auto &&component : mComponents)
		if (component->IsInside(x, y))
			return true;

	return GUIComponent::IsInside(x, y);
}

void GUIComposite::Update() 
{
	GUIComponent::Update();  

	for (GUIComponent *component : mComponents)
		component->Update();
}

void GUIComposite::Draw()
{
	for (auto &&component : mComponents)
		component->Draw();
}

bool GUIComposite::OnMouseMove(int x, int y, int dx, int dy) 
{
	for (auto GUIComponent : mComponents)
		if (GUIComponent->OnMouseMove(x, y, dx, dy))
			return true;

	return false;
}

bool GUIComposite::OnMousePress(int x, int y) 
{
	for (auto GUIComponent : mComponents)
		if (GUIComponent->OnMousePress(x, y))
			return true;

	return false;
}

bool GUIComposite::OnMouseRelease(int x, int y) 
{
	for (auto GUIComponent : mComponents)
		if (GUIComponent->OnMouseRelease(x, y))
			return true;

	return false;
}

bool GUIComposite::OnKeyPress(unsigned int key) 
{
	for (auto GUIComponent : mComponents)
		if (GUIComponent->OnKeyPress(key))
			return true;

	return false;
}