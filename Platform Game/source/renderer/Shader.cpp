#include "Shader.h"
#include <fstream>
#include <glew/GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include "../logger/Logger.h"

Shader::Shader(const std::string &vertexShaderFile, const std::string &fragmentShaderFile)
{
	std::string vertexShaderFilePath = "shaders\\" + vertexShaderFile;
	std::ifstream ifs(vertexShaderFilePath);

	if (!ifs)
		Logger::Instance().Log("cannot open file" + vertexShaderFilePath, Logger::Level::LOG_ERROR);

	// load vertex shader source 
	std::string vertexShaderSource;
	std::string vertexShaderLine;
	while (!ifs.eof())
	{
		std::getline(ifs, vertexShaderLine);
		vertexShaderSource.append(vertexShaderLine);
		vertexShaderSource.append("\n");
	}

	// create vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// compile vertex shader
	const char *vertexShaderSourceC = vertexShaderSource.c_str();
	glShaderSource(vertexShader, 1, &vertexShaderSourceC, NULL);
	glCompileShader(vertexShader);

	// get compilation errors
	int  success;
	char infoLog[512] = {};
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		Logger::Instance().Log("VERTEX SHADER COMPILATION_FAILED\n" + std::string(infoLog), Logger::Level::LOG_ERROR);
	}

	ifs.close();

	std::string fragmentShaderFilePath = "shaders\\" + fragmentShaderFile;
	ifs.open(fragmentShaderFilePath);

	if (!ifs)
		Logger::Instance().Log("cannot open file" + fragmentShaderFilePath, Logger::Level::LOG_ERROR);

	// load fragment shader source
	std::string fragmentShaderSource;
	std::string fragmentShaderLine;
	while (!ifs.eof())
	{
		std::getline(ifs, fragmentShaderLine);
		fragmentShaderSource.append(fragmentShaderLine);
		fragmentShaderSource.append("\n");
	}

	// create fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// compile fragment shader
	const char *fragmentShaderSourceC = fragmentShaderSource.c_str();
	glShaderSource(fragmentShader, 1, &fragmentShaderSourceC, NULL);
	glCompileShader(fragmentShader);

	// get compilation errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		Logger::Instance().Log("FRAGMENT SHADER COMPILATION_FAILED\n" + std::string(infoLog), Logger::Level::LOG_ERROR);
	}

	// create shader program 
	mShaderProgram = glCreateProgram();

	// attach shaders
	glAttachShader(mShaderProgram, vertexShader);
	glAttachShader(mShaderProgram, fragmentShader);

	// link program
	glLinkProgram(mShaderProgram);

	// get linking errors
	glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(mShaderProgram, 512, NULL, infoLog);
		Logger::Instance().Log("PROGRAM SHADER COMPILATION_FAILED\n" + std::string(infoLog), Logger::Level::LOG_ERROR);
	}

	// delete shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
	glDeleteProgram(mShaderProgram);
}

void Shader::Use() const
{
	glUseProgram(mShaderProgram);
}

int Shader::GetVertexAttributeLocation(std::string &vertexAttributeName)
{ 
	auto it = mVertexAttributeLocations.find(vertexAttributeName);

	if (it == mVertexAttributeLocations.end())
	{
		GLint vertexAttributeLocation = glGetAttribLocation(mShaderProgram, vertexAttributeName.c_str());

		if (vertexAttributeLocation != -1)
			mVertexAttributeLocations[vertexAttributeName] = vertexAttributeLocation;
		else
			return -1;
	}

	return mVertexAttributeLocations[vertexAttributeName];
}

int Shader::GetUniformLocation(std::string const &uniformName) 
{
	auto it = mUniformLocations.find(uniformName);

	if (it == mUniformLocations.end())
	{
		GLint uniformLocation = glGetUniformLocation(mShaderProgram, uniformName.c_str());

		if (uniformLocation != -1)
			mUniformLocations[uniformName] = uniformLocation;
		else
			return -1;
	}

	return mUniformLocations[uniformName];
}

unsigned int Shader::GetUniformBlockIndex(std::string const &uniformBlockName)
{
	return glGetUniformBlockIndex(mShaderProgram, uniformBlockName.c_str());  // returns GL_INVALID_INDEX if not found
}

void Shader::SetUniform(const std::string &uniformName, float v0)
{
	GLint uniformLocation = GetUniformLocation(uniformName);

	if (uniformLocation != -1)
		glUniform1f(uniformLocation, v0);
}

void Shader::SetUniform(const std::string &uniformName, float v0, float v1)
{
	GLint uniformLocation = GetUniformLocation(uniformName);

	if (uniformLocation != -1)
		glUniform2f(uniformLocation, v0, v1);
}

void Shader::SetUniform(const std::string &uniformName, float v0, float v1, float v2)
{
	GLint uniformLocation = GetUniformLocation(uniformName);

	if (uniformLocation != -1)
		glUniform3f(uniformLocation, v0, v1, v2);
}

void Shader::SetUniform(const std::string &uniformName, float v0, float v1, float v2, float v3)
{
	GLint uniformLocation = GetUniformLocation(uniformName);

	if (uniformLocation != -1)
		glUniform4f(uniformLocation, v0, v1, v2, v3);
}

void Shader::SetUniform(const std::string &uniformName, int v0)
{
	GLint uniformLocation = GetUniformLocation(uniformName);

	if (uniformLocation != -1)
		glUniform1i(uniformLocation, v0);
}

void Shader::SetUniform(const std::string &uniformName, int v0, int v1)
{
	GLint uniformLocation = GetUniformLocation(uniformName);

	if (uniformLocation != -1)
		glUniform2i(uniformLocation, v0, v1);
}

void Shader::SetUniform(const std::string &uniformName, int v0, int v1, int v2)
{
	GLint uniformLocation = GetUniformLocation(uniformName);

	if (uniformLocation != -1)
		glUniform3i(uniformLocation, v0, v1, v2);
}

void Shader::SetUniform(const std::string &uniformName, int v0, int v1, int v2, int v3)
{
	GLint uniformLocation = GetUniformLocation(uniformName);

	if (uniformLocation != -1)
		glUniform4i(uniformLocation, v0, v1, v2, v3);
}

void Shader::SetUniform(const std::string &uniformName, unsigned int v0)
{
	GLint uniformLocation = GetUniformLocation(uniformName);

	if (uniformLocation != -1)
		glUniform1ui(uniformLocation, v0);
}

void Shader::SetUniform(const std::string &uniformName, unsigned int v0, unsigned int v1)
{
	GLint uniformLocation = GetUniformLocation(uniformName);

	if (uniformLocation != -1)
		glUniform2ui(uniformLocation, v0, v1);
}

void Shader::SetUniform(const std::string &uniformName, unsigned int v0, unsigned int v1, unsigned int v2)
{
	GLint uniformLocation = GetUniformLocation(uniformName);

	if (uniformLocation != -1)
		glUniform3ui(uniformLocation, v0, v1, v2);
}

void Shader::SetUniform(const std::string &uniformName, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3)
{
	GLint uniformLocation = GetUniformLocation(uniformName);

	if (uniformLocation != -1)
		glUniform4ui(uniformLocation, v0, v1, v2, v3);
}

void Shader::SetUniform(const std::string &uniformName, unsigned int numElements, float *v)
{
	GLint uniformLocation = GetUniformLocation(uniformName);

	if (uniformLocation != -1)
		switch (numElements)
		{
			case 1:
				glUniform1fv(uniformLocation, numElements, v);
				break;
			case 2:
				glUniform2fv(uniformLocation, numElements, v);
				break;
			case 3:
				glUniform3fv(uniformLocation, numElements, v);
				break;
			case 4:
				glUniform4fv(uniformLocation, numElements, v);
				break;
		}
}

void Shader::SetUniform(const std::string &uniformName, unsigned int numElements, int *v)
{
	GLint uniformLocation = GetUniformLocation(uniformName);

	if (uniformLocation != -1)
		switch (numElements)
		{
			case 1:
				glUniform1iv(uniformLocation, numElements, v);
				break;
			case 2:
				glUniform2iv(uniformLocation, numElements, v);
				break;
			case 3:
				glUniform3iv(uniformLocation, numElements, v);
				break;
			case 4:
				glUniform4iv(uniformLocation, numElements, v);
				break;
		}
}

void Shader::SetUniform(const std::string &uniformName, unsigned int numElements, unsigned int *v)
{
	GLint uniformLocation = GetUniformLocation(uniformName);

	if (uniformLocation != -1)
		switch (numElements)
		{
			case 1:
				glUniform1uiv(uniformLocation, numElements, v);
				break;
			case 2:
				glUniform2uiv(uniformLocation, numElements, v);
				break;
			case 3:
				glUniform3uiv(uniformLocation, numElements, v);
				break;
			case 4:
				glUniform4uiv(uniformLocation, numElements, v);
				break;
		}
}

void Shader::SetUniform(const std::string &uniformName, const glm::mat2x2 &matrix, bool rowMajor)
{
	GLint uniformLocation = GetUniformLocation(uniformName);

	if (uniformLocation != -1)
		glUniformMatrix2fv(uniformLocation, 1, rowMajor, glm::value_ptr(matrix));
}

void Shader::SetUniform(const std::string &uniformName, const glm::mat3x3 &matrix, bool rowMajor)
{
	GLint uniformLocation = GetUniformLocation(uniformName);

	if (uniformLocation != -1)
		glUniformMatrix3fv(uniformLocation, 1, rowMajor, glm::value_ptr(matrix));
}

void Shader::SetUniform(const std::string &uniformName, const glm::mat4x4 &matrix, bool rowMajor)
{
	GLint uniformLocation = GetUniformLocation(uniformName);

	if (uniformLocation != -1)
		glUniformMatrix4fv(uniformLocation, 1, rowMajor, glm::value_ptr(matrix));
}

/**** TODO: uniform block ****/
void Shader::SetUniformBlock(const std::string &name )
{
	/* get uniform block index */
	int uniformBlockIndex = glGetUniformBlockIndex(mShaderProgram, name.c_str());

	/* get uniform block name */
	char nameBuf[100];
	GLsizei nameLength;
	glGetActiveUniformBlockName(mShaderProgram, uniformBlockIndex, sizeof nameBuf, &nameLength, nameBuf);

	/* get info about uniforms in uniform block */
	GLint uniformIndices[10];
	glGetActiveUniformBlockiv(mShaderProgram, uniformBlockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, uniformIndices);
}

