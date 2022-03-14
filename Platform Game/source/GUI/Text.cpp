#include "Text.h"
#include "../GraphicsManager.h"
#include "../asset manager/FontManager.h"

Text::Text(const std::string &fontName, unsigned int fontSize, unsigned int bufferCharSize)
	: mBufferCharSize(bufferCharSize), mFont(&FontManager::GetInstance().GetFont(fontName, fontSize))
{
	mMesh.LoadVertexAttribute("POSITIONS", nullptr, mBufferCharSize * NUM_VERTEX_PER_CHAR * 2 * sizeof(float), 2, true);
	mMesh.LoadVertexAttribute("TEX_COORDS", nullptr, mBufferCharSize * NUM_VERTEX_PER_CHAR * 2 * sizeof(float), 2, true);
}

Text::Text(const std::string &text, const std::string &fontName, unsigned int fontSize) 
	: mText(text), mBufferCharSize(text.size()), mFont(&FontManager::GetInstance().GetFont(fontName, fontSize))
{
	mMesh.LoadVertexAttribute("POSITIONS", nullptr, mBufferCharSize * NUM_VERTEX_PER_CHAR * 2 * sizeof(float), 2, true);
	mMesh.LoadVertexAttribute("TEX_COORDS", nullptr, mBufferCharSize * NUM_VERTEX_PER_CHAR * 2 * sizeof(float), 2, true);

	AppendText(mText);
}

void Text::Clear()
{
	mVertices.Clear();
	mMesh.Clear();
	mCursorPosition = 0;
}

void Text::SetFont(const std::string &fontName, unsigned int fontSize)
{
	mFont = &FontManager::GetInstance().GetFont(fontName, fontSize);
	
	if (mText.size())
	{
		Clear();

		mMesh.LoadVertexAttribute("POSITIONS", nullptr, mBufferCharSize * NUM_VERTEX_PER_CHAR * 2 * sizeof(float), 2, true);
		mMesh.LoadVertexAttribute("TEX_COORDS", nullptr, mBufferCharSize * NUM_VERTEX_PER_CHAR * 2 * sizeof(float), 2, true);

		AppendText(mText);
	}
}

void Text::SetSize(unsigned int fontSize)
{
	mFont = &FontManager::GetInstance().GetFont(mFont->GetName(), fontSize);
	
	if (mText.size())
	{
		Clear();

		mMesh.LoadVertexAttribute("POSITIONS", nullptr, mBufferCharSize * NUM_VERTEX_PER_CHAR * 2 * sizeof(float), 2, true);
		mMesh.LoadVertexAttribute("TEX_COORDS", nullptr, mBufferCharSize * NUM_VERTEX_PER_CHAR * 2 * sizeof(float), 2, true);

		AppendText(mText);
	}
}

void Text::Set(const std::string &text)
{
	Clear();
	
	mText = text;	
	mBufferCharSize = text.size();

	mMesh.LoadVertexAttribute("POSITIONS", nullptr, mBufferCharSize * NUM_VERTEX_PER_CHAR * 2 * sizeof(float), 2, true);
	mMesh.LoadVertexAttribute("TEX_COORDS", nullptr, mBufferCharSize * NUM_VERTEX_PER_CHAR * 2 * sizeof(float), 2, true);
	
	AppendText(mText);
}

void Text::Append(char c)
{
	Append(std::string(1, c));
}

void Text::Append(const std::string &text)
{
	if (mText.size() + text.size() >= mBufferCharSize)
		AllocateBuffer((mText.size() + text.size()) * 1.5f);   // allocate new buffer and copy text

	mText += text;

	AppendText(text);
}

void Text::AppendText(const std::string &text)
{
	float screenWidthSizePerPixel = 2.0f / GraphicsManager::GetInstance().GetViewportWidth();
	float screenHeightSizePerPixel = 2.0f / GraphicsManager::GetInstance().GetViewportHeight();

	// append text to end of string
	for (char character : text)
	{
		Character c = mFont->GetCharacter(character);

		float quadOffsetX = c.bearingX * screenWidthSizePerPixel;
		float quadOffsetY = c.bearingY * screenHeightSizePerPixel;
		float quadWidth = c.width * screenWidthSizePerPixel;
		float quadHeight = c.height * screenHeightSizePerPixel;
		float quadAdvance = mCursorPosition * screenWidthSizePerPixel;

		float charTextureX = c.textureOffsetX;
		float charTextureY = c.textureOffsetY;
		float charTextureWidth = c.textureWidth;
		float charTextureHeight = c.textureHeight;

		// first triangle
		mVertices.InsertLast(Vertex{{quadAdvance + quadOffsetX, quadOffsetY}, {charTextureX, charTextureY}});
		mVertices.InsertLast(Vertex{{quadAdvance + quadOffsetX + quadWidth, quadOffsetY}, {charTextureX + charTextureWidth, charTextureY}});
		mVertices.InsertLast(Vertex{{quadAdvance + quadOffsetX, + quadOffsetY - quadHeight}, {charTextureX, charTextureY + charTextureHeight}});

		// second triangle
		mVertices.InsertLast(Vertex{{quadAdvance + quadOffsetX, quadOffsetY - quadHeight}, {charTextureX, charTextureY + charTextureHeight}});
		mVertices.InsertLast(Vertex{{quadAdvance + quadOffsetX + quadWidth, quadOffsetY}, {charTextureX + charTextureWidth, charTextureY}});
		mVertices.InsertLast(Vertex{{quadAdvance + quadOffsetX + quadWidth, quadOffsetY - quadHeight}, {charTextureX + charTextureWidth, charTextureY + charTextureHeight}});

		mCursorPosition += c.advanceX;
	}

	// update vertex buffer - TODO: ugly, implement static mesh loading
	Vector<float> positions_;
	Vector<float> textureCoordinates_;
	for (unsigned int i = (mText.size() - text.size()) * NUM_VERTEX_PER_CHAR; i < mVertices.Size(); i++)
	{
		positions_.InsertLast(mVertices[i].mPosition[0]);
		positions_.InsertLast(mVertices[i].mPosition[1]);

		textureCoordinates_.InsertLast(mVertices[i].mTextureCoordinates[0]);
		textureCoordinates_.InsertLast(mVertices[i].mTextureCoordinates[1]);
	}

	unsigned int offset = (mText.size() - text.size()) * NUM_VERTEX_PER_CHAR * 2 * sizeof(float);

	mMesh.UpdateDynamicVertexAttribute("POSITIONS", offset, positions_.Data(), positions_.Size());
	mMesh.UpdateDynamicVertexAttribute("TEX_COORDS", offset, textureCoordinates_.Data(), textureCoordinates_.Size());
	mMesh.SetVertexCount(mText.size() * NUM_VERTEX_PER_CHAR);

	CalculateTextBox();
}

void Text::AllocateBuffer(unsigned int bufferCharSize)
{
	Clear();

	mBufferCharSize = bufferCharSize;

	mMesh.LoadVertexAttribute("POSITIONS", nullptr, mBufferCharSize * NUM_VERTEX_PER_CHAR * 2 * sizeof(float), 2, true); // WRONG: buffer not freed
	mMesh.LoadVertexAttribute("TEX_COORDS", nullptr, mBufferCharSize * NUM_VERTEX_PER_CHAR * 2 * sizeof(float), 2, true);

	AppendText(mText);
}

void Text::CalculateTextBox()
{
	float minX = mVertices.First().mPosition[0];  // text box left bound
	float maxX = mVertices.Last().mPosition[0];   // text box right bound
	float minY = 0.0f, maxY = 0.0f;

	for (Vertex const &vertex : mVertices)
	{
		if (vertex.mPosition[1] > maxY)
			maxY = vertex.mPosition[1];       // text box upper bound
		                   
		if (vertex.mPosition[1] < minY)
			minY = vertex.mPosition[1];       // text box lower bound
	}

	// calculate text box dimensions
	mTextBoxNormalizedWidth = maxX - minX;
	mTextBoxNormalizedHeight = maxY - minY;

	// calculate normalized device coordinates of bottom left corner of text box
	mTextBoxNormalizedOffsetX = minX;
	mTextBoxNormalizedOffsetY = minY;
}
	
// static utility/helper function to get a text (normalized) dimensions for a given font and font size 
glm::vec2 Text::GetTextBoundingBox(const std::string &text, const std::string &fontName, unsigned int fontSize)
{
	float screenWidthSizePerPixel = 2.0f / GraphicsManager::GetInstance().GetViewportWidth();
	float screenHeightSizePerPixel = 2.0f / GraphicsManager::GetInstance().GetViewportHeight();

	Font &font = FontManager::GetInstance().GetFont(fontName, fontSize);
	unsigned int cursorPosition = 0;

	Vector<Text::Vertex> vertices;

	// create text mesh
	for (char character : text)
	{
		Character c = font.GetCharacter(character);

		float quadOffsetX = c.bearingX * screenWidthSizePerPixel;
		float quadOffsetY = c.bearingY * screenHeightSizePerPixel;
		float quadWidth = c.width * screenWidthSizePerPixel;
		float quadHeight = c.height * screenHeightSizePerPixel;
		float quadAdvance = cursorPosition * screenWidthSizePerPixel;

		float charTextureX = c.textureOffsetX;
		float charTextureY = c.textureOffsetY;
		float charTextureWidth = c.textureWidth;
		float charTextureHeight = c.textureHeight;

		// first triangle
		vertices.InsertLast(Vertex{ {quadAdvance + quadOffsetX, quadOffsetY}, {charTextureX, charTextureY} });
		vertices.InsertLast(Vertex{ {quadAdvance + quadOffsetX + quadWidth, quadOffsetY}, {charTextureX + charTextureWidth, charTextureY} });
		vertices.InsertLast(Vertex{ {quadAdvance + quadOffsetX, +quadOffsetY - quadHeight}, {charTextureX, charTextureY + charTextureHeight} });

		// second triangle
		vertices.InsertLast(Vertex{ {quadAdvance + quadOffsetX, quadOffsetY - quadHeight}, {charTextureX, charTextureY + charTextureHeight} });
		vertices.InsertLast(Vertex{ {quadAdvance + quadOffsetX + quadWidth, quadOffsetY}, {charTextureX + charTextureWidth, charTextureY} });
		vertices.InsertLast(Vertex{ {quadAdvance + quadOffsetX + quadWidth, quadOffsetY - quadHeight}, {charTextureX + charTextureWidth, charTextureY + charTextureHeight} });

		cursorPosition += c.advanceX;
	}

	// calculate text box
	float minX = vertices.First().mPosition[0];  // text box left bound
	float maxX = vertices.Last().mPosition[0];   // text box right bound
	float minY = 0.0f, maxY = 0.0f;

	for (Vertex const &vertex : vertices)
	{
		if (vertex.mPosition[1] > maxY)
			maxY = vertex.mPosition[1];       // text box upper bound

		if (vertex.mPosition[1] < minY)
			minY = vertex.mPosition[1];       // text box lower bound
	}

	// calculate text box dimensions
	float textBoxNormalizedWidth = maxX - minX;
	float textBoxNormalizedHeight = maxY - minY;

	return glm::vec2(textBoxNormalizedWidth, textBoxNormalizedHeight);
}