#ifndef SHADER_H
#define SHADER_H

#include "../data structures/vector.hpp"
#include <initializer_list>
#include <string>
#include <map>  
#include "glm/glm.hpp"

class Shader
{
public:
	Shader(const std::string &vertexShaderFile, const std::string &fragmentShaderFile);
	~Shader();
	void Use() const;
	void SetUniform(const std::string &uniformName, float v0);
	void SetUniform(const std::string &uniformName, float v0, float v1);
	void SetUniform(const std::string &uniformName, float v0, float v1, float v2);
	void SetUniform(const std::string &uniformName, float v0, float v1, float v2, float v3);
	void SetUniform(const std::string &uniformName, int v0);
	void SetUniform(const std::string &uniformName, int v0, int v1);
	void SetUniform(const std::string &uniformName, int v0, int v1, int v2);
	void SetUniform(const std::string &uniformName, int v0, int v1, int v2, int v3);
	void SetUniform(const std::string &uniformName, unsigned int v0);
	void SetUniform(const std::string &uniformName, unsigned int v0, unsigned int v1);
	void SetUniform(const std::string &uniformName, unsigned int v0, unsigned int v1, unsigned int v2);
	void SetUniform(const std::string &uniformName, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3);
	void SetUniform(const std::string &uniformName, unsigned int numElements, float *v);
	void SetUniform(const std::string &uniformName, unsigned int numElements, int *v);
	void SetUniform(const std::string &uniformName, unsigned int numElements, unsigned int *v);
	void SetUniform(const std::string &uniformName, const glm::mat2x2 &matrix, bool rowMajor = false);
	void SetUniform(const std::string &uniformName, const glm::mat3x3 &matrix, bool rowMajor = false);
	void SetUniform(const std::string &uniformName, const glm::mat4x4 &matrix, bool rowMajor = false);
	void SetUniformBlock(const std::string &uniformBlockName);
private:
	unsigned int mShaderProgram;
	int GetVertexAttributeLocation(std::string &vertexAttributeName);
	int GetUniformLocation(std::string const &uniformName);
	unsigned int GetUniformBlockIndex(std::string const &uniformBlockName);
	std::map<std::string, int> mVertexAttributeLocations;
	std::map<std::string, int> mUniformLocations;
};


#endif  // SHADER_H