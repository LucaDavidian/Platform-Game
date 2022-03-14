#ifndef TEXT_H
#define TEXT_H

#include <string>
#include "../renderer/StaticMesh.h"
#include "../asset manager/Font.h"

class Text
{
public:
	Text(const std::string &fontName, unsigned int fontSize, unsigned int bufferCharSize);
	Text(const std::string &text, const std::string &fontName, unsigned int fontSize); 

	void SetFont(const std::string &fontName, unsigned int fontSize); 
	void SetSize(unsigned int fontSize);

	void Set(const std::string &text);
	void Append(char c);
	void Append(const std::string &text);
	
	const StaticMesh &GetMesh() const { return mMesh; } 
	const Font &GetFont() const { return *mFont; }

	glm::vec2 GetNormalizedDimensions() const { return glm::vec2(mTextBoxNormalizedWidth, mTextBoxNormalizedHeight); }
	glm::vec2 GetNormalizedOffset() const { return glm::vec2(mTextBoxNormalizedOffsetX, mTextBoxNormalizedOffsetY); }

	std::string const &GetText() const { return mText; }

	static glm::vec2 GetTextBoundingBox(const std::string &text, const std::string &fontName, unsigned int fontSize);
private:
	struct Vertex
	{
		float mPosition[2];
		float mTextureCoordinates[2];
	};
	Vector<Vertex> mVertices;

	void AllocateBuffer(unsigned int bufferCharSize);
	void AppendText(const std::string &text); 
	void Clear();
	void CalculateTextBox();

	static const unsigned int NUM_VERTEX_PER_CHAR = 6;   // a character is made of 6 vertices
	unsigned int mBufferCharSize;                        // buffer size (number of characters)
	StaticMesh mMesh;
	
	Font *mFont;
	
	std::string mText;
	
	unsigned int mCursorPosition = 0;

	float mTextBoxNormalizedWidth;
	float mTextBoxNormalizedHeight;

	float mTextBoxNormalizedOffsetX;   // offset of bottom left corner of text box in normalized device coordinates
	float mTextBoxNormalizedOffsetY;
};

#endif  // TEXT_H
