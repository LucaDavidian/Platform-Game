#include "RenderSystem.h"
#include "../../GraphicsManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../../data structures/vector.hpp"
#include "../../asset manager/Texture.h"
#include "../../GUI/GUIManager.h"
#include "../../GUI/GUIComponent.h"

RenderSystem::RenderSystem() 
	: mSpriteShader("sprite vertex shader.glsl", "sprite fragment shader.glsl"), mImageShader("vertex shader.glsl", "image fragment shader.glsl"),
	mPointShader("point vertex shader.glsl", "point fragment shader.glsl"), mTextShader("GUI vertex shader.glsl", "text fragment shader.glsl"),
	mGUIShader("GUI vertex shader.glsl", "GUI fragment shader.glsl")
{
	float positions[] =
	{
		0.5f, 0.5f, 0.0f, 
		-0.5f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f, 
		0.5f, -0.5f, 0.0f
	};
	
	mQuadMesh.LoadVertexAttribute("POSITIONS", positions, sizeof positions, 3);
	mFlippedXQuadMesh.LoadVertexAttribute("POSITIONS", positions, sizeof positions, 3);
	mFlippedYQuadMesh.LoadVertexAttribute("POSITIONS", positions, sizeof positions, 3);
	mFlippedXYQuadMesh.LoadVertexAttribute("POSITIONS", positions, sizeof positions, 3);

	float textureCoordinates[] =
	{
		1.0, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f, 
		1.0f, 0.0f
	};

	float flippedXtextureCoordinates[] =
	{
		0.0, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f
	};

	float flippedYtextureCoordinates[] =
	{
		1.0, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f
	};

	float flippedXYtextureCoordinates[] =
	{
		0.0, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	mQuadMesh.LoadVertexAttribute("TEX_COORDS", textureCoordinates, sizeof textureCoordinates, 2);	
	mFlippedXQuadMesh.LoadVertexAttribute("TEX_COORDS", flippedXtextureCoordinates, sizeof flippedXtextureCoordinates, 2);
	mFlippedYQuadMesh.LoadVertexAttribute("TEX_COORDS", flippedYtextureCoordinates, sizeof flippedYtextureCoordinates, 2);
	mFlippedXYQuadMesh.LoadVertexAttribute("TEX_COORDS", flippedXYtextureCoordinates, sizeof flippedXYtextureCoordinates, 2);

	unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

	mQuadMesh.LoadIndices(indices, 6);
	mFlippedXQuadMesh.LoadIndices(indices, 6);
	mFlippedYQuadMesh.LoadIndices(indices, 6);
	mFlippedXYQuadMesh.LoadIndices(indices, 6);

	//InputManager::GetInstance().RegisterKeyPressEvent(*this, &RenderSystem::EnableRenderPath);
}

#include "../Scene.h"
#include "../components/TransformComponent.h"
#include "../components/SpriteComponent.h"
#include "../components/CameraComponent.h"
//#include "TextComponent.h"

//#include "PathFindingSystem.h"
//#include "data structures/graph.hpp"

#include <algorithm>

void RenderSystem::Render()
{
	//glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f));
	//glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3());
	//glm::mat4 modelMatrix = translationMatrix * scaleMatrix;

	//// draw action text
	//Text const *labelText = TextFSM::GetInstance().GetText();

	//scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
	//translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-0.1f, -0.35f, 0.0f));
	//modelMatrix = translationMatrix * scaleMatrix;

	//mGUIShader.Use();
	//mGUIShader.SetUniform("modelMatrix", modelMatrix);
	//mGUIShader.SetUniform("color", 1.0f, 1.0f, 0.0f);

	//GraphicsSystem::GetInstance().EnableBlending();

	//TextFSM::GetInstance().GetText()->GetFont().GetAtlas().Bind(0);
	//TextFSM::GetInstance().GetText()->GetMesh().Bind();

	//GraphicsSystem::GetInstance().EnableBlending();
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//TextFSM::GetInstance().GetText()->GetMesh().Draw();
	//GraphicsSystem::GetInstance().DisableBlending();

	Entity *camera = Scene::GetInstance().GetCamera();

	if (!camera)
		return;

	CameraComponent *cameraComponent = camera->GetComponent<CameraComponent>();
	glm::mat4 viewMatrix = cameraComponent->GetViewMatrix();
	glm::mat4 projectionMatrix = cameraComponent->GetProjectionMatrix();

	// render entities
	mSpriteShader.Use();

	// #TODO_sort_every_frame_is_wrong
	Vector<Entity*> renderables;

	for (Entity *entity : Scene::GetInstance().GetEntities())
	{
		if (entity->HasComponents<TransformComponent, SpriteComponent>())
			renderables.InsertLast(entity);
	}

	std::sort(renderables.Begin(), renderables.End(), [](Entity *a, Entity *b) { 
		return a->GetComponent<SpriteComponent>()->GetZOrder() < b->GetComponent<SpriteComponent>()->GetZOrder();
	});


	for (Entity *entity : renderables/*Scene::GetInstance().GetEntities()*/)
	{
		if (!entity->IsActive())
			continue;

		if (entity->HasComponents<TransformComponent, SpriteComponent>())
		{
			TransformComponent *transform = entity->GetComponent<TransformComponent>();
			 
			glm::mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * transform->GetScaledWorldMatrix();

			//mSpriteShader.SetUniform("modelMatrix", transform->GetScaledWorldMatrix());
			mSpriteShader.SetUniform("modelViewProjectionMatrix", modelViewProjectionMatrix);

			SpriteComponent *sprite = entity->GetComponent<SpriteComponent>();

			unsigned int tileNumber = sprite->GetTile();

			unsigned int offsetX = tileNumber % sprite->GetNumColumns();
			unsigned int offsetY = sprite->GetNumRows() - 1 - tileNumber / sprite->GetNumColumns();

			float tileHeight = 1.0f / sprite->GetNumRows();
			float tileWidth = 1.0f / sprite->GetNumColumns();

			mSpriteShader.SetUniform("texCoordIndex.tileWidth", tileWidth);
			mSpriteShader.SetUniform("texCoordIndex.tileHeight", tileHeight);
			mSpriteShader.SetUniform("texCoordIndex.offsetX", offsetX);
			mSpriteShader.SetUniform("texCoordIndex.offsetY", offsetY);

			sprite->GetTexture()->Bind(0);

			GraphicsManager::GetInstance().EnableBlending();
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			if (sprite->IsFlipped())
				mFlippedXQuadMesh.Bind();
			else
				mQuadMesh.Bind();		
			
			mQuadMesh.DrawTriangles();
			GraphicsManager::GetInstance().DisableBlending();
		}
	}

	//if (mRenderPath)  // render path finding graph
	//{
	//	// render walk area and holes geometry
	//	auto &meshes = PathFindingSystem::GetInstance().GetMeshes();

	//	mPointShader.Use();

	//	glm::mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;
	//	mPointShader.SetUniform("viewProjectionMatrix", viewProjectionMatrix);

	//	glPointSize(10.0f);
	//	mPointShader.SetUniform("color", 1.0f, 0.0f, 0.0f);

	//	for (auto mesh : meshes)
	//	{
	//		mesh.Bind();

	//		mesh.DrawPoints();
	//		mesh.DrawLines();
	//	}

	//	// draw visibility graph edges
	//	auto &graphEdges = PathFindingSystem::GetInstance().GetGraphEdges();

	//	mPointShader.SetUniform("color", 0.0f, 1.0f, 1.0f);

	//	graphEdges.Bind();
	//	graphEdges.DrawLines(false);

	//	// draw visibility graph nodes
	//	auto &graphNodes = PathFindingSystem::GetInstance().GetGraphNodes();

	//	glPointSize(16.0f);
	//	mPointShader.SetUniform("color", 1.0f, 0.0f, 0.0f);

	//	graphNodes.Bind();
	//	graphNodes.DrawPoints();

	//	// draw path edges
	//	auto &graphEdgesPath = PathFindingSystem::GetInstance().GetPathEdges();

	//	mPointShader.SetUniform("color", 0.0f, 1.0f, 0.0f);

	//	graphEdgesPath.Bind();
	//	graphEdgesPath.DrawLines(false);

	//	// draw path nodes
	//	auto &graphNodesPath = PathFindingSystem::GetInstance().GetPathNodes();

	//	glPointSize(10.0f);
	//	mPointShader.SetUniform("color", 0.0f, 1.0f, 0.0f);

	//	graphNodesPath.Bind();
	//	graphNodesPath.DrawPoints();
	//}

	//// render interaction text
	//InteractionFSM::GetInstance();
	//const Text &interactionText = InteractionFSM::GetInstance().GetText();

	//SetShader(mTextShader);

	//interactionText.GetFont().GetAtlas().Bind(0);
	//interactionText.GetMesh().Bind();

	//glm::vec2 position(0.0f, -0.5f);
	//float offX = position.x - interactionText.GetNormalizedDimensions().x / 2.0f - interactionText.GetNormalizedOffset().x;
	//float offY = position.y - interactionText.GetNormalizedDimensions().y / 2.0f - interactionText.GetNormalizedOffset().y;

	//scaleMatrix = glm::mat4(1.f);
	//translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(offX, offY, 0.0f));
	//modelMatrix = translationMatrix * scaleMatrix;

	//mCurrentShader->SetUniform("modelMatrix", modelMatrix);

	//glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	//mCurrentShader->SetUniform("color", color.r, color.g, color.b);  

	//GraphicsManager::GetInstance().EnableBlending();
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//interactionText.GetMesh().DrawTriangles();
	//GraphicsManager::GetInstance().DisableBlending();

	//for (auto entity : Scene::GetInstance().GetEntities())
	//	if (entity->HasComponent<TextComponent>())
	//	{
	//		Text const &text = entity->GetComponent<TextComponent>()->GetText();
	//		glm::vec3 pos = entity->GetComponent<TransformComponent>()->GetPosition();
	//		glm::vec2 position(pos.x, pos.y);
	//		float offX = position.x - text.GetNormalizedDimensions().x / 2.0f - text.GetNormalizedOffset().x;
	//		float offY = position.y - text.GetNormalizedDimensions().y / 2.0f - text.GetNormalizedOffset().y;

	//		scaleMatrix = glm::mat4(1.f);
	//		translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(offX, offY, 0.0f));
	//		modelMatrix = translationMatrix * scaleMatrix;

	//		mCurrentShader->SetUniform("modelMatrix", modelMatrix);
	//		 
	//		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	//		mCurrentShader->SetUniform("color", color.r, color.g, color.b);

	//		text.GetFont().GetAtlas().Bind(0);
	//		text.GetMesh().Bind();

	//		GraphicsManager::GetInstance().EnableBlending();
	//		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//		text.GetMesh().DrawTriangles();
	//		GraphicsManager::GetInstance().DisableBlending();
	//	}

	for (auto &&gui : GUIManager::GetInstance().GetGUIPanels())
		gui->Draw();
}

#include "../components/CollisionComponent.h"

void RenderSystem::RenderDebug()
{
	Entity *camera = Scene::GetInstance().GetCamera();

	if (!camera)
		return;

	CameraComponent *cameraComponent = camera->GetComponent<CameraComponent>();
	glm::mat4 viewMatrix = cameraComponent->GetViewMatrix();
	glm::mat4 projectionMatrix = cameraComponent->GetProjectionMatrix();

	mPointShader.Use();

	for (Entity *entity : Scene::GetInstance().GetEntities())
	{
		if (entity->HasComponents<TransformComponent, CollisionComponent>())
		{
			auto transformComponent = entity->GetComponent<TransformComponent>();
			auto collisionComponent = entity->GetComponent<CollisionComponent>();

			//glm::vec2 colliderPosition = glm::vec2(transformComponent->GetPosition()) + collisionComponent->GetOffset();
			//glm::vec2 colliderDimensions = glm::vec2(collisionComponent->GetDimensions().x * transformComponent->GetScale().x, collisionComponent->GetDimensions().y * transformComponent->GetScale().y);

			TransformComponent colliderTransform = *transformComponent;
			colliderTransform.SetPosition(colliderTransform.GetPosition() + glm::vec3(collisionComponent->GetOffset(), 0.0f));
			colliderTransform.SetScale(glm::vec3(collisionComponent->GetDimensions().x * colliderTransform.GetScale().x, collisionComponent->GetDimensions().y * colliderTransform.GetScale().y, colliderTransform.GetScale().z));

			glm::mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * colliderTransform.GetScaledWorldMatrix();

			mPointShader.SetUniform("modelViewProjectionMatrix", modelViewProjectionMatrix);
			mPointShader.SetUniform("color", collisionComponent->GetColor().r, collisionComponent->GetColor().g, collisionComponent->GetColor().g);

			// draw rect - TODO: render primitives
			mQuadMesh.Bind();
			mQuadMesh.DrawLines();
		}
	}
}