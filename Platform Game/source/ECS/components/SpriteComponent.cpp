#include "SpriteComponent.h"
#include "../../asset manager/TextureManager.h"
#include "../../asset manager/Texture.h"
#include "../../Lua bindings/LuaVM.h"
#include "../Entity.h"

namespace 
{
	SpriteComponent SpriteComponentTableCtor(LuaObject componentTable)
	{
		LuaObject name = componentTable.Get("file_name");
		LuaObject numRows = componentTable.Get("num_rows");
		LuaObject numCols = componentTable.Get("num_cols");
		LuaObject zOrder = componentTable.Get("z_order");

		return SpriteComponent(name, numRows, numCols, zOrder ? zOrder : (unsigned)0);
	}

	void SpriteComponentAddToEntity(SpriteComponent const &spriteComponent, Entity &entity)
	{
		entity.AddComponent<SpriteComponent>(spriteComponent);
	}
}

const bool SpriteComponent::mRegistered = []() {
	Reflect::Reflect<SpriteComponent>("sprite")
		.AddConstructor(SpriteComponentTableCtor)
		.AddMemberFunction(SpriteComponentAddToEntity, "add_to_entity");

	return true;
}();

SpriteComponent::SpriteComponent(const std::string &textureName, unsigned int numRows, unsigned int numColumns, unsigned int zOrder)
	: mTextureAtlas(&TextureManager::GetInstance().GetTexture(textureName)), mNumRows(numRows), mNumColumns(numColumns), mZorder(zOrder)
{
}

void SpriteComponent::SetTexture(const std::string &textureName, unsigned int numRows, unsigned int numColumns)
{
	mTextureAtlas = &TextureManager::GetInstance().GetTexture(textureName);
	mNumRows = numRows;
	mNumColumns = numColumns;
}