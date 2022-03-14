#include "TextureManager.h"

Texture &TextureManager::GetTexture(const std::string &textureFilePath)
{
	std::map<std::string, Texture>::iterator it = mLoadedTextures.find(textureFilePath);

	if (it == mLoadedTextures.end())
		mLoadedTextures.insert(std::pair<std::string, Texture>(textureFilePath, Texture(textureFilePath)));

	return mLoadedTextures.at(textureFilePath);
}