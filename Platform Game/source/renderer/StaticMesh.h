#ifndef STATIC_MESH_H
#define STATIC_MESH_H

#include "../data structures/vector.hpp"
#include "glm/glm.hpp"
#include <glew/GL/glew.h>
#include <string>
#include <map>

class StaticMesh
{
public:
	StaticMesh();
	~StaticMesh();
	void LoadVertexAttribute(const std::string &attributeName, float *data, unsigned int size, unsigned int elementSize, bool dynamic = false);
	void UpdateDynamicVertexAttribute(const std::string &attributeName, unsigned int offset, float *data, unsigned int size);
	void RemoveVertexAttribute(const std::string &attributeName);
    void Clear();
	void LoadIndices(unsigned int *indices, unsigned int size);
	void Bind() const;
	void BindVertexAttribute();
	void DrawTriangles() const;
	void DrawPoints() const;
	void DrawLines(bool strip = true) const;
	void SetVertexCount(unsigned count) { mVertexCount = count; }
private:
	unsigned int mVAO;
	Vector<unsigned int> mVBOs;
	std::map<std::string,unsigned> mAttributeMap;
	unsigned int mEBO;
	bool mIndexed = false;
	unsigned int mIndexCount;
	unsigned int mVertexCount;
};

#endif  // STATIC_MESH