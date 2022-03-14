#include "EditorGameState.h"
#include <fstream>
#include "Log.h"
#include "glm/gtc/matrix_transform.hpp"

std::ofstream &operator<<(std::ofstream &ofs, glm::vec2 const &v)
{
	ofs << v.x << " ";
	ofs << v.y << '\n';

	return ofs;
}

void EditorGameState::SaveArea()
{
	std::ofstream ofs;
	ofs.open("data/walk area.dat", std::ios_base::out | std::ios_base::binary);

	if (!ofs)
		LogError("cannot open file for writing");

	// serialize walk area points
	ofs << mWalkArea.Size() << '\n';

	for (auto &point : mWalkArea)
	{
		// preprocess point (scale/translate)
		glm::vec2 outPoint;
		outPoint.x = point.x / 2.0f;
		outPoint.y = point.y / 2.0f;

		ofs << outPoint;
	}

	ofs << "\n\n";

	for (auto &&hole : mHoles)
	{
		if (hole.Size() == 0)
			continue;

		// serialize hole points
		ofs << hole.Size() << '\n';

		for (auto &point : hole)
		{
			// preprocess point (scale/translate)
			glm::vec2 outPoint;
			outPoint.x = point.x / 2.0f;
			outPoint.y = point.y / 2.0f;

			ofs << outPoint;
		}

		ofs << "\n\n";
	}
}

Vector<glm::vec2> LoadArea()
{
	// deserialize walk area points
	return {};
}

void LoadRoom()
{
	Entity *roomEntity = Scene::GetInstance().AddEntity();

	roomEntity->AddComponent<SpriteComponent>("room.png", 1, 1);
	TransformComponent *bgTransformComponent = roomEntity->AddComponent<TransformComponent>();
	bgTransformComponent->SetPosition(0.0f, 0.0f, 0.0f);
	bgTransformComponent->SetScale(3.0f, 2.0f, 0.0f);
}

#include "InputSystem.h"

void EditorGameState::OnEntry(Game *game)
{
	GUIPanel *buttonPanel = new GUIPanel(glm::vec2(), glm::vec2(0.98f, 0.05f));
	buttonPanel->SetAchorPoint(GUIComponent::AnchorPoint::TOP_LEFT, GUIComponent::AnchorPoint::TOP_LEFT);

	const unsigned int numButtons = 4;
	const float buttonHeight = 1.0f;

	GUIToggleButton *addButton = new GUIToggleButton(glm::vec2(), glm::vec2());
	addButton->SetText("add");
	GUIToggleButton *removeButton = new GUIToggleButton(glm::vec2(), glm::vec2());
	removeButton->SetText("remove");
	GUIToggleButton *moveButton = new GUIToggleButton(glm::vec2(), glm::vec2());
	moveButton->SetText("move");

	GUIButtonGroup *buttonGroup = new GUIButtonGroup(glm::vec2(0 * 1.0 / numButtons, 0.0f), glm::vec2(1.0f / numButtons, buttonHeight));

	buttonGroup->AddButton(addButton);     // button 0 
	buttonGroup->AddButton(removeButton);  // button 1
	buttonGroup->AddButton(moveButton);    // button 2

	buttonGroup->AddButtonSelectedListener([&](unsigned i, bool b)
	{
		switch (i)
		{
			case 0:
				if (b)
				{
					switch (mState)
					{
						case REMOVE:
							mState = mBeforeRemove;

							break;
						case IDLE:
							mState = PLACE_AREA;

							if (mWalkArea.Empty())
							{
								mWalkAreaMesh = new StaticMesh;
								mWalkAreaMesh->LoadVertexAttribute("POSITION", NULL, 100 * sizeof(float), 3, true);
							}

							break;

						case AREA_PLACED:
							mState = PLACE_HOLE;

							if (mHoles.Size() <= mCurrentHole)
								mHoles.InsertLast(Vector<glm::vec2>());

							if (mHoles[mCurrentHole].Empty())
							{
								mHoleMeshes.InsertLast(new StaticMesh());
								mHoleMeshes[mCurrentHole]->LoadVertexAttribute("POSITION", NULL, 100 * sizeof(float), 3, true);
							}

							break;
					}

				}
				else
					switch (mState)
					{
						case PLACE_AREA:
							mState = IDLE;
							break;
						case PLACE_HOLE:
							mState = AREA_PLACED;
							break;
					}
				break;
			case 1:
				if (b)
				{
					mBeforeRemove = mState;
					mState = REMOVE;
				}
				else
				{
					mState = mBeforeRemove;

					switch (mState)
					{
						case IDLE:
						case AREA_PLACED:
							break;
						case PLACE_AREA:
							mState = IDLE;
							break;
						case PLACE_HOLE:
							mState = AREA_PLACED;
							break;
					}


				}
				break;
			case 2:

				// implement

				break;
		}
	});

	mButtonGroup = buttonGroup;  // keep a reference to the GUI component (#TODO_UGLY)

	GUIButton *clearPathButton = new GUIButton(glm::vec2(1 * 1.0 / numButtons, 0.0f), glm::vec2(1.0f / numButtons, buttonHeight));
	clearPathButton->SetAchorPoint(GUIComponent::AnchorPoint::TOP_LEFT, GUIComponent::AnchorPoint::TOP_LEFT);
	clearPathButton->SetText("clear");
	clearPathButton->AddButtonClickedListener([&](const std::string &) { Clear(); });

	GUIButton *loadRoomButton = new GUIButton(glm::vec2(2 * 1.0 / numButtons, 0.0f), glm::vec2(1.0f / numButtons, buttonHeight));
	loadRoomButton->SetAchorPoint(GUIComponent::AnchorPoint::TOP_LEFT, GUIComponent::AnchorPoint::TOP_LEFT);
	loadRoomButton->SetText("load room");
	loadRoomButton->AddButtonClickedListener([&](const std::string &) { LoadRoom(); });

	GUIButton *savePathButton = new GUIButton(glm::vec2(3 * 1.0 / numButtons, 0.0f), glm::vec2(1.0f / numButtons, buttonHeight));
	savePathButton->SetAchorPoint(GUIComponent::AnchorPoint::TOP_LEFT, GUIComponent::AnchorPoint::TOP_LEFT);
	savePathButton->SetText("save area");
	savePathButton->AddButtonClickedListener([&](const std::string &) { SaveArea(); });

	buttonPanel->AddGuiComponent(buttonGroup);
	buttonPanel->AddGuiComponent(clearPathButton);
	buttonPanel->AddGuiComponent(loadRoomButton);
	buttonPanel->AddGuiComponent(savePathButton);

	GUIManager::GetInstance().AddGUIPanel(buttonPanel, true);

	mGUIComponents.InsertLast(buttonPanel);

	mConnections.InsertLast(InputManager::GetInstance().RegisterMouseMoveEvent(*this, &EditorGameState::OnMouseMove));
	mConnections.InsertLast(InputManager::GetInstance().RegisterMousePressEvent(*this, &EditorGameState::OnMousePress));
	mConnections.InsertLast(InputManager::GetInstance().RegisterMouseReleaseEvent(*this, &EditorGameState::OnMouseRelease));
	mConnections.InsertLast(InputManager::GetInstance().RegisterKeyPressEvent([game](unsigned int key) { if (key == SDL_GetKeyFromScancode(SDL_SCANCODE_ESCAPE)) game->ChangeState(&GameState::mMenuGameState); }));

	// editor camera 
	Entity *editorCamera = Scene::GetInstance().AddEntity();
	editorCamera->AddComponent<TransformComponent>();
	editorCamera->AddComponent<OrthographicCameraComponent, CameraComponent>(3.0f / 2.0f, 2.0f, 0.0f, 0.1f);

	Scene::GetInstance().SetCamera(editorCamera);

	// window sliders
	GUISlider *horizontalSlider = new GUISlider(glm::vec2(0.0f), glm::vec2(0.98f, 0.02f), GUISlider::HORIZONTAL);
	horizontalSlider->SetCursorDimension(1.0f);
	horizontalSlider->AddCursorMoveEventListener([editorCamera](float dp)
	{
		auto t = editorCamera->GetComponent<TransformComponent>();
		t->SetPosition(t->GetPosition() + t->GetAxisX() * dp);
	});

	GUISlider *verticalSlider = new GUISlider(glm::vec2(0.0f), glm::vec2(0.02f, 0.98f), GUISlider::VERTICAL);
	verticalSlider->SetAchorPoint(GUIComponent::AnchorPoint::TOP_RIGHT, GUIComponent::AnchorPoint::TOP_RIGHT);
	verticalSlider->SetCursorDimension(1.0f);
	verticalSlider->AddCursorMoveEventListener([editorCamera](float dp)
	{
		auto t = editorCamera->GetComponent<TransformComponent>();
		t->SetPosition(t->GetPosition() + t->GetAxisY() * dp);
	});

	mGUIComponents.InsertLast(horizontalSlider);
	mGUIComponents.InsertLast(verticalSlider);

	//GUISystem::GetInstance().AddGUIPanel(horizontalSlider);  // #TODO_GUI add sliders to same panel with no opacity (GUI overlay lets input through)
	//GUISystem::GetInstance().AddGUIPanel(verticalSlider);
}

void EditorGameState::OnExit(Game *game)
{
	for (auto &&panel : mGUIComponents)
		GUIManager::GetInstance().RemoveGUIPanel(panel);

	for (auto &&entity : Scene::GetInstance().GetEntities())
		entity->Destroy();

	for (auto &&connection : mConnections)
		connection.Disconnect();
}

void EditorGameState::Render(Game *game)
{
	if (mWalkAreaMesh)
		RenderSystem::GetInstance().DrawConnectedPoints(*mWalkAreaMesh);

	for (auto &&holeMesh : mHoleMeshes)
		RenderSystem::GetInstance().DrawConnectedPoints(*holeMesh);
}

void EditorGameState::OnMouseMove(int x, int y)
{

}

bool EditorGameState::OnMousePress(int x, int y)
{
	return false;
}

bool EditorGameState::OnMouseRelease(int x, int y)
{
	// get mouse position's normalized device coordinates
	float ndcX = -1.0f + 2.0f * x / (float)GraphicsManager::GetInstance().GetViewportWidth();
	float ndcY = -1.0f + 2.0f * y / (float)GraphicsManager::GetInstance().GetViewportHeight();

	switch (mState)
	{
		case PLACE_AREA:  // place point
		{
			mWalkArea.InsertLast(glm::vec2(ndcX, ndcY));

			float point[] = { ndcX, ndcY, 0.0f };
			mWalkAreaMesh->UpdateDynamicVertexAttribute("POSITION", 3 * sizeof(float) * (mWalkArea.Size() - 1), point, 3 * sizeof(float));
			mWalkAreaMesh->SetVertexCount(mWalkArea.Size());

			// if perimeter is closed end place and change state
			const float pointHalfSizeX = 10.0f / GraphicsManager::GetInstance().GetViewportWidth();
			const float pointHalfSizeY = 10.0f / GraphicsManager::GetInstance().GetViewportHeight();

			if (mWalkArea.Size() > 1)
				if (ndcX >= mWalkArea[0].x - pointHalfSizeX && ndcX <= mWalkArea[0].x + pointHalfSizeX && ndcY >= mWalkArea[0].y - pointHalfSizeY && ndcY <= mWalkArea[0].y + pointHalfSizeY)
				{
					mWalkArea.Last() = mWalkArea.First();

					float point[] = { mWalkArea.Last().x, mWalkArea.Last().y, 0.0f };
					mWalkAreaMesh->UpdateDynamicVertexAttribute("POSITION", 3 * sizeof(float) * (mWalkArea.Size() - 1), point, 3 * sizeof(float));

					mState = AREA_PLACED;
					mButtonGroup->ToggleButton(0);
				}
		}
		break;

		case PLACE_HOLE:
		{
			mHoles[mCurrentHole].InsertLast(glm::vec2(ndcX, ndcY));

			float point[] = { ndcX, ndcY, 0.0f };
			mHoleMeshes[mCurrentHole]->UpdateDynamicVertexAttribute("POSITION", 3 * sizeof(float) * (mHoles[mCurrentHole].Size() - 1), point, 3 * sizeof(float));
			mHoleMeshes[mCurrentHole]->SetVertexCount(mHoles[mCurrentHole].Size());

			// if perimeter is closed end place and change state
			const float pointHalfSizeX = 10.0f / GraphicsManager::GetInstance().GetViewportWidth();
			const float pointHalfSizeY = 10.0f / GraphicsManager::GetInstance().GetViewportHeight();

			if (mHoles[mCurrentHole].Size() > 1)
				if (ndcX >= mHoles[mCurrentHole][0].x - pointHalfSizeX && ndcX <= mHoles[mCurrentHole][0].x + pointHalfSizeX && ndcY >= mHoles[mCurrentHole][0].y - pointHalfSizeY && ndcY <= mHoles[mCurrentHole][0].y + pointHalfSizeY)
				{
					mHoles[mCurrentHole].Last() = mHoles[mCurrentHole].First();

					float point[] = { mHoles[mCurrentHole].Last().x, mHoles[mCurrentHole].Last().y, 0.0f };
					mHoleMeshes[mCurrentHole]->UpdateDynamicVertexAttribute("POSITION", 3 * sizeof(float) * (mHoles[mCurrentHole].Size() - 1), point, 3 * sizeof(float));

					mState = AREA_PLACED;
					mButtonGroup->ToggleButton(0);
					mCurrentHole++;
				}

			break;
		}
		case REMOVE:  // remove point
		{
			const float pointHalfSizeX = 10.0f / GraphicsManager::GetInstance().GetViewportWidth();
			const float pointHalfSizeY = 10.0f / GraphicsManager::GetInstance().GetViewportHeight();

			for (auto &&it = mWalkArea.Begin(); it != mWalkArea.End(); ++it)
				if (ndcX >= it->x - pointHalfSizeX && ndcX <= it->x + pointHalfSizeX && ndcY >= it->y - pointHalfSizeY && ndcY <= it->y + pointHalfSizeY)
				{
					mWalkArea.Remove(it);

					// #GFX_API_TODO: make an API to insert/remove vertices from buffers, and to manage vertex buffers in general
					mWalkAreaMesh->Clear();

					float points[100];
					for (int i = 0, j = 0; i < mWalkArea.Size(); i++, j += 3)
					{
						points[j] = mWalkArea[i].x;
						points[j + 1] = mWalkArea[i].y;
						points[j + 2] = 0.0f;
					}

					mWalkAreaMesh->LoadVertexAttribute("POSITION", points, 100 * sizeof(float), 3, true);
					mWalkAreaMesh->SetVertexCount(mWalkArea.Size());

					break;
				}

			for (size_t n = 0; n < mHoles.Size(); n++)
				for (auto &&it = mHoles[n].Begin(); it != mHoles[n].End(); ++it)
					if (ndcX >= it->x - pointHalfSizeX && ndcX <= it->x + pointHalfSizeX && ndcY >= it->y - pointHalfSizeY && ndcY <= it->y + pointHalfSizeY)
					{
						mHoles[n].Remove(it);

						// #GFX_API_TODO: make an API to insert/remove vertices from buffers, and to manage vertex buffers in general
						mHoleMeshes[n]->Clear();

						float points[100];
						for (int i = 0, j = 0; i < mHoles[n].Size(); i++, j += 3)
						{
							points[j] = mHoles[n][i].x;
							points[j + 1] = mHoles[n][i].y;
							points[j + 2] = 0.0f;
						}

						mHoleMeshes[n]->LoadVertexAttribute("POSITION", points, 100 * sizeof(float), 3, true);
						mHoleMeshes[n]->SetVertexCount(mHoles[n].Size());

						break;
					}
		}
		break;

	}

	return true;
}

void EditorGameState::Clear()
{
	mWalkArea.Clear();
	mWalkAreaMesh->Clear();
	mWalkAreaMesh->LoadVertexAttribute("POSITION", NULL, 50 * 3 * sizeof(float), 3, true);

	for (size_t i = 0; i < mHoles.Size(); i++)
	{
		mHoles[i].Clear();
		mHoleMeshes[i]->Clear();
		mHoleMeshes[i]->LoadVertexAttribute("POSITION", NULL, 50 * 3 * sizeof(float), 3, true);
	}

	switch (mState)
	{
		case IDLE:
		case AREA_PLACED:
			break;
		case PLACE_AREA:
			mButtonGroup->ToggleButton(0);
			break;
		case PLACE_HOLE:
			mButtonGroup->ToggleButton(0);
			break;
	}

	mState = IDLE;
}


