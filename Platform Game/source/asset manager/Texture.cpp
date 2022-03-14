#include "Texture.h"
#include <glew/GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb image/stb_image.h>
#include "../logger/Logger.h"

Texture::Texture(const std::string &name) : mName(name)
{
	stbi_set_flip_vertically_on_load(true);
	unsigned char *imageData = stbi_load(("assets/textures/" + name).c_str(), &mWidth, &mHeight, &mNumChannels, 0);

	if (!imageData)
		Logger::Instance().Log("cannot load image: " + name, Logger::Level::LOG_ERROR);
	else
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &mTextureID);   // texture created as GL_TEXTURE_2D, from now on only bind to GL_TEXTURE_2D target

		switch (mNumChannels)
		{
			case 1U:
				glTextureStorage2D(mTextureID, 1, GL_R8, mWidth, mHeight);                                           // immutable storage (allocate storage)
				glTextureSubImage2D(mTextureID, 0, 0, 0, mWidth, mHeight, GL_RED, GL_UNSIGNED_BYTE, imageData);      // put data into storage
				break;
			case 2U:
				glTextureStorage2D(mTextureID, 1, GL_RG8, mWidth, mHeight);
				glTextureSubImage2D(mTextureID, 0, 0, 0, mWidth, mHeight, GL_RG, GL_UNSIGNED_BYTE, imageData);
				break;
			case 3U:
				glTextureStorage2D(mTextureID, 1, GL_RGB8, mWidth, mHeight);
				glTextureSubImage2D(mTextureID, 0, 0, 0, mWidth, mHeight, GL_RGB, GL_UNSIGNED_BYTE, imageData);
				break;
			case 4U:
				glTextureStorage2D(mTextureID, 1, GL_RGBA8, mWidth, mHeight);
				glTextureSubImage2D(mTextureID, 0, 0, 0, mWidth, mHeight, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
				break;
			default:
				Logger::Instance().Log("loading image: " + name + ", invalid number of texture channels", Logger::Level::LOG_ERROR);
		}

		glGenerateTextureMipmap(mTextureID);

		glTextureParameteri(mTextureID, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);                              // embedded sampler object (default sampling parameters for the texture) - direct state access
		glTextureParameteri(mTextureID, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTextureParameteri(mTextureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST/*GL_LINEAR*/);
		glTextureParameteri(mTextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST/*GL_LINEAR*/);

		//glGenTextures(1, &mTexture);
		//glBindTexture(GL_TEXTURE_2D, mTexture);                                                            // from now on only bind to GL_TEXTURE_2D target
		//glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, mWidth, mHeight);                                       // immutable storage (allocate storage)
		//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mWidth, mHeight, GL_RGBA, GL_UNSIGNED_BYTE, imageData);    // put data into storage
		//glGenerateMipmap(GL_TEXTURE_2D);

		//glGenTextures(1, &mTexture);
		//glBindTexture(GL_TEXTURE_2D, mTexture);                                                             // from now on only bind to GL_TEXTURE_2D target
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);  // mutable storage  (allocate storage and specify data)

		//glTexParameteri(mTexture, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);                                   // embedded sampler object (default sampling parameters for the texture) 
		//glTexParameteri(mTexture, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		//glTexParameteri(mTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(mTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//glCreateSamplers(1, &mSampler);                                                              // create sampler object
		//glSamplerParameteri(mSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);              // set sampling parameters
		//glSamplerParameteri(mSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		stbi_image_free(imageData);  // free loaded data
	}
}

Texture::Texture(int width, int height, unsigned int numChannels) : mWidth(width), mHeight(height), mNumChannels(numChannels)
{ 
	glCreateTextures(GL_TEXTURE_2D, 1, &mTextureID);                            
	
	switch (mNumChannels)
	{
		case 1U:
			glTextureStorage2D(mTextureID, 1, GL_R8, width, height);     // immutable storage (allocate storage)
			break;
		case 2U:
			glTextureStorage2D(mTextureID, 1, GL_RG8, width, height);
			break;
		case 3U:
			glTextureStorage2D(mTextureID, 1, GL_RGB8, width, height);
			break;
		case 4U:
			glTextureStorage2D(mTextureID, 1, GL_RGBA8, width, height);
			break;
		default:
			Logger::Instance().Log("invalid number of texture channels specified when creating empty texture", Logger::Level::LOG_ERROR);
	}

	glGenerateTextureMipmap(mTextureID);

	glTextureParameteri(mTextureID, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);     // embedded sampler object (default sampling parameters for the texture) - direct state access
	glTextureParameteri(mTextureID, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTextureParameteri(mTextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(mTextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glGenTextures(1, &mTexture);
	//glBindTexture(GL_TEXTURE_2D, mTexture);
	//glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA, width, height);             

	//glGenTextures(1, &mTexture);
	//glBindTexture(GL_TEXTURE_2D, mTexture);                                                           
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);   
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	//glTexParameteri(mTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(mTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//glCreateSamplers(1, &mSampler);                                                                
	//glSamplerParameteri(mSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);                
	//glSamplerParameteri(mSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

//Texture::Texture(const Texture &other)
//{
//
//}

Texture::Texture(Texture &&other) : mWidth(other.mWidth), mHeight(other.mHeight), mNumChannels(other.mNumChannels), mTextureID(other.mTextureID), mSampler(other.mSampler), mTextureUnit(other.mTextureUnit)
{
	other.mTextureID = 0;
}

Texture::~Texture() 
{ 
	glDeleteTextures(1, &mTextureID);  
}


//Texture &Texture::operator=(const Texture &other)
//{
//
//}

Texture &Texture::operator=(Texture &&other)
{
	Texture temp(std::move(other));
	std::swap(*this, temp);

	return *this;
}

void Texture::Update(int xOffset, int yOffset, int width, int height, void *imageData)
{
	switch (mNumChannels)
	{
		case 1U:
			glTextureSubImage2D(mTextureID, 0, xOffset, yOffset, width, height, GL_RED, GL_UNSIGNED_BYTE, imageData);      // put data into storage
			break;
		case 2U:
			glTextureSubImage2D(mTextureID, 0, xOffset, yOffset, width, height, GL_RG, GL_UNSIGNED_BYTE, imageData);
			break;
		case 3U:
			glTextureSubImage2D(mTextureID, 0, xOffset, yOffset, width, height, GL_RGB, GL_UNSIGNED_BYTE, imageData);
			break;
		case 4U:
			glTextureSubImage2D(mTextureID, 0, xOffset, yOffset, width, height, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
			break;
	}

	glGenerateTextureMipmap(mTextureID);

	//glBindTexture(GL_TEXTURE_2D, mTexture);
	//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	//glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::Bind(unsigned int textureUnit) const
{
	// each texture unit has several texture targets

	glBindTextureUnit(textureUnit, mTextureID);     // bind texture to the target it was created with (GL_TEXTURE_2D) in specified texture unit

	//glActiveTexture(GL_TEXTURE0 + textureUnit);   // select current texture unit
	//glBindTexture(GL_TEXTURE_2D, mTexture);       // bind texture to target in currently active texture unit 

	//glBindSampler(textureUnit, mSampler);         // sampler and texture are bound to a texture unit

	mTextureUnit = textureUnit;
}

void Texture::Unbind() const
{
	glBindTextureUnit(mTextureUnit, 0);             // unbind texture from texture target of current texture unit

	//glActiveTexture(GL_TEXTURE0 + mTextureUnit);
	//glBindTexture(GL_TEXTURE_2D, 0);

	//glBindSampler(textureUnit, 0);                // unbind sampler 

	mTextureUnit = -1;
}