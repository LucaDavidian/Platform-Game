#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class Texture
{
public:
	Texture(const std::string &name);
	Texture(int width, int height, unsigned int numChannels = 4U);

	Texture(const Texture &other) = delete;
	Texture(Texture &&other);

	~Texture();

	Texture &operator=(const Texture &other) = delete;
	Texture &operator=(Texture &&other);

	unsigned int GetWidth() const { return mWidth; }
	unsigned int GetHeight() const { return mHeight; }

	void Bind(unsigned int textureUnit) const;
	void Unbind() const;
	void Update(int xOffset, int yOffset, int width, int height, void *imageData);
private:
	std::string mName;
	int mWidth;
	int mHeight;
	int mNumChannels;
	unsigned int mTextureID;
	unsigned int mSampler;
	mutable unsigned int mTextureUnit = -1;
};

#endif // TEXTURE_H