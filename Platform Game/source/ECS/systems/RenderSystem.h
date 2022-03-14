#ifndef RENDERING_SYSTEM_H
#define RENDERING_SYSTEM_H

#include "../../renderer/Shader.h"
#include "../../renderer/StaticMesh.h"
#include "../../data structures/vector.hpp"

class RenderSystem
{
public:
	static RenderSystem &GetInstance() 
	{ 
		static RenderSystem instance; 
		return instance; 
	}

	void Render();

	void RenderDebug();

	void DrawConnectedPoints(StaticMesh &mesh)
	{
		SetShader(mPointShader);

		glPointSize(10.0f);
		mPointShader.SetUniform("color", 1.0f, 1.0f, 0.0f);

		mCurrentShader->SetUniform("viewProjectionMatrix", glm::mat4(1.0f));

		mesh.Bind();

		mesh.DrawPoints();
		mesh.DrawLines();
	}

	Shader &GetCurrentShader() { return *mCurrentShader; }
	void SetShader(Shader &shader) { mCurrentShader = &shader; mCurrentShader->Use(); }

	StaticMesh &GetQuadMesh() { return mQuadMesh; }

	Shader mSpriteShader;
	Shader mImageShader;
	Shader mPointShader;
	Shader mTextShader;
	Shader mGUIShader;

private:
	RenderSystem();

	Shader *mCurrentShader;

	StaticMesh mQuadMesh;
	StaticMesh mFlippedXQuadMesh;
	StaticMesh mFlippedYQuadMesh;
	StaticMesh mFlippedXYQuadMesh;
};

#endif  // RENDERING_SYSTEM_H
