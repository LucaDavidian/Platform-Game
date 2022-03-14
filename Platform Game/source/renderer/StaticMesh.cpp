#include "StaticMesh.h"

StaticMesh::StaticMesh()
{
	/******** VAO - vertex array objects and VBO - vertex buffer objects - vertex specification ********/

	//glCreateVertexArrays(1, &mVAO);   // generate and initialize VAO
	//glGenVertexArrays(1, &mVAO);      // or generate VAO

	//glBindVertexArray(mVAO);          // bind current VAO

	//VBOs.Resize(2);                   

	//glCreateBuffers(2, &VBOs[0]);               // generate and initialize VBOs
	//glGenBuffers(2, &VBOs[0]);                  // or generate VBOs

	//glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);     // bind current VBO (array buffer target)

	//struct Vertex  // vertex data
	//{
	//	float position[3];
	//	float color[3];
	//} vertexData[] = { { {0.5f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f} }, { {-0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f} }, { { -0.5f, -0.5f, 0.0f }, {0.0f, 0.0f, 1.0f} }, { { 0.5f, -0.5f, 0.0f }, {1.0f, 1.0f, 1.0f} } };
	
	//glBufferStorage(GL_ARRAY_BUFFER, sizeof(Vertex) * sizeof vertexData / sizeof(Vertex), vertexData, 0);   // create and initialize data store for buffer currently bound to givent target
	//glNamedBufferStorage(mVBOs[0], sizeof(Vertex) * sizeof vertexData / sizeof(Vertex), vertexData, 0);     // create and initialize data store for given buffer

	/**** use currently bound vertex buffer to specify vertex attributes of currently bound VAO (format + source of vertex data) ****/

	//glEnableVertexAttribArray(0);               // enable vertex attribute 0 for currently bound VAO
	//glEnableVertexArrayAttrib(mVAO, 0);         // or enable vertex attribute 0 for specified VAO

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(baseOffset + 0));    // bind currently bound VBO to vertex attribute 0 (mapped to floats in GLSL)
	
	//glEnableVertexAttribArray(1);               // enable vertex attribute 1 for currently bound VAO
	//glEnableVertexArrayAttrib(mVAO, 1);         // or enable vertex attribute 1 for specified VAO

	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(baseOffset + offsetof(Vertex, color)));    // bind currently bound VBO to vertex attribute 1 (mapped to floats in GLSL)
	
	/**** separate vertex data format specification and source of vertex data ****/

	//glEnableVertexAttribArray(0);               // enable vertex attribute 0 for currently bound VAO
	//glEnableVertexArrayAttrib(mVAO, 0);         // or enable vertex attribute 0 for specified VAO

	//glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);                           // specify VERTEX ATTRIBUTE 0 data format
	//glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, color));     // specify VERTEX ATTRIBUTE 1 data format

	//glBindVertexBuffer(0, mVBO[0], baseOffset, sizeof(Vertex));   // bind source of data to BINDING POINT 0

	//glVertexAttribBinding(0, 0);    // link vertex data binding point 0 to vertex attribute 0
	//glVertexAttribBinding(1, 0);    // link vertex data binding point 0 to vertex attribute 1


	/******** EBO - element buffer objects/index buffers  ********/

	//unsigned int indices[] = { 0, 1, 2, 2, 0, 3 };

	//EBOs.Resize(1);

	//glGenBuffers(1, &mEBO);
	//glCreateBuffers(1, &mEBO);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);      // bind current EBO (element array buffer target)

	//glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * 6, indices, 0);

	//glVertexArrayElementBuffer(VAOs[0], EBOs[0]);   // named VAO



	glCreateVertexArrays(1, &mVAO);    // generate and initialize VAO - vertex array object
	glBindVertexArray(mVAO);
	
	//glGenVertexArrays(1, &mVAO);      // generate VAO 
	//glBindVertexArray(mVAO);         
}

StaticMesh::~StaticMesh()
{
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(VBOs.Size(), VBOs.Data());
	//glDeleteBuffers(1, &EBO);
}

// #GFX_API_TODO: load different data types
void StaticMesh::LoadVertexAttribute(const std::string &attributeName, float *data, unsigned int size, unsigned int elementSize, bool dynamic)
{
	mVBOs.InsertLast(0);        

	/**** create vertex buffer object ****/
	glCreateBuffers(1, &mVBOs.Last());                                          
	//glGenBuffers(1, &mVBOs.Last()):
	
	/**** bind vertex buffer object ****/
	//glBindBuffer(GL_ARRAY_BUFFER, mVBOs.Last());

	/**** load data into currently bound vertex buffer object ****/
	//GLbitfield flags = dynamic ? GL_DYNAMIC_STORAGE_BIT : 0;                    // GL_DYNAMIC_STORAGE_BIT - GL_MAP_READ_BIT - GL_MAP_WRITE_BIT etc...
	//glBufferStorage(GL_ARRAY_BUFFER, size * sizeof(float), data, flags);        // create immutable data store for vertex object bound to target
	
	//GLbitfield usage = dynamic ? GL_DYNAMIC_DRAW : 0;                           // GL_ STREAM/STATIC/DYNAMIC _ DRAW/READ/COPY
	//glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data, usage);           // create data store for vertex object bound to target
	
	//glBufferSubData(GL_ARRAY_BUFFER, 0, size * sizeof(float), data);            // update a subset of a buffer object's data store
	
	//glClearBufferData(GL_ARRAY_BUFFER, GL_RGBA, GL_RGBA, GL_FLOAT, 0);
	//glClearBufferSubData(GL_ARRAY_BUFFER, GL_RGBA, GL_RGBA, size * sizeof(float), GL_RGBA, GL_FLOAT, 0);

	/**** load data into vertex buffer object - DSA ****/
	GLbitfield flags = dynamic ? GL_DYNAMIC_STORAGE_BIT : 0;
	glNamedBufferStorage(mVBOs.Last(), size * sizeof(float), data, flags);       // create immutable data store for vertex object bound to target

	//GLbitfield usage = dynamic ? GL_DYNAMIC_DRAW : 0;                          // GL_ STREAM/STATIC/DYNAMIC _ DRAW/READ/COPY
	//glNamedBufferData(mVBOs.Last(), size * sizeof(float), data, usage);        // create data store for a given vertex object - DSA
	
	//glNamedBufferSubData(mVBOs.Last(), 0, size * sizeof(float), data);         // updates a subset of a buffer object's data store - DSA
	
	//glClearNamedBufferData(mVBOs.Last(), GL_RGBA, GL_RGBA, GL_FLOAT, 0);
	//glClearNamedBufferSubData(mVBOs.Last(), GL_RGBA, GL_RGBA, size * sizeof(float), GL_RGBA, GL_FLOAT, 0);


	/**** bind vertex array ****/
	glBindVertexArray(mVAO);

	/**** set currently bound VAO's vertex attribute format and source ****/
	//glBindBuffer(GL_ARRAY_BUFFER, mVBOs.Last());
	//glVertexAttribPointer(mVBOs.Size() - 1, elementSize, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));

	/**** set vertex attribute format ****/
	unsigned int vertexAttributeIndex = mVBOs.Size() - 1;

	glVertexAttribFormat(vertexAttributeIndex, elementSize, GL_FLOAT, GL_FALSE, 0);                    // set vertex attribute data format for currently bound VAO
	//glVertexArrayAttribFormat(mVAO, vertexAttributeIndex, elementSize, GL_FLOAT, GL_FALSE, 0);       // set vertex attribute data format - DSA


	/**** associate source vertex buffer with binding point ****/
	unsigned int bindingPointIndex = mVBOs.Size() - 1;

	glBindVertexBuffer(bindingPointIndex, mVBOs.Last(), 0, elementSize * sizeof(float));                 // bind vertex buffer to currently bound VAO at binding point
	//glVertexArrayVertexBuffer(mVAO, bindingPointIndex, mVBOs.Last(), 0, elementSize * sizeof(float));  // bind vertex buffer to VAO at binding point - DSA


	/**** associate vertex attribute to binding point ****/
	glVertexAttribBinding(vertexAttributeIndex, bindingPointIndex);                                    // map vertex buffer to currently bound VAO's vertex attribute                                                                           
	//glVertexArrayAttribBinding(mVAO, vertexAttributeIndex, bindingPointIndex);                       // map vertex buffer to VAO's vertex attribute - DSA



	mAttributeMap[attributeName] = mVBOs.Size() - 1;  // index inside VBO array
}

void StaticMesh::UpdateDynamicVertexAttribute(const std::string &attributeName, unsigned int offset, float *data, unsigned int size)
{
	/**** dynamically modify buffer data - data in system memory (buffer must have been initialized with GL_DYNAMIC_STORAGE_BIT flag) ****/
	glNamedBufferSubData(mVBOs[mAttributeMap[attributeName]], offset, size * sizeof(float), data);
	//glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);    

	/**** map the buffer - get a pointer to the buffer (performance-heavy) ****/
	//unsigned int access = GL_WRITE_ONLY;   // GL_READ_ONLY/GL_WRITE_ONLY/GL_READ_WRITE
	//void *bufferPtr = glMapNamedBuffer(VBOs[mAttributeMap[attributeName]], access);  // if (!bufferPtr) Error
	////void *bufferPtr = glMapBuffer(GL_ARRAY_BUFFER, access); 
	//
	//for (int i = 0; i < size; i++)
	//	*((float*)bufferPtr + i) = data[i];

	/**** map the buffer - get a pointer to the buffer (with offset and range) ****/
	//unsigned int accessBitField = GL_MAP_WRITE_BIT;
	//void *bufferPtr = glMapNamedBufferRange(VBOs[mAttributeMap[attributeName]], offset, size, accessBitField);
	//void *bufferPtr = glMapBufferRange(GL_ARRAY_BUFFER, offset, size, access bit field);
	// .... memcpy data

	//glUnmapNamedBuffer(VBOs[mAttributeMap[attributeName]]);  // returns GL_TRUE/GL_FALSE
	//glUnmapBuffer(GL_ARRAY_BUFFER);
}

void StaticMesh::LoadIndices(unsigned int *indices, unsigned int size)
{
	glCreateBuffers(1, &mEBO);                                 // create element buffer object
	//glGenBuffers(1, &mEBO);

	glNamedBufferStorage(mEBO, size * sizeof(unsigned int), (const void*)0, GL_DYNAMIC_STORAGE_BIT);
	glNamedBufferSubData(mEBO, 0, size * sizeof(unsigned int), indices);

	glVertexArrayElementBuffer(mVAO, mEBO);   // bind EBO to VAO

	//glBindVertexArray(VAO);                                                                           // bind vertex array object
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);                                                       // store EBO in VAO
	//glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), indices, GL_STATIC_DRAW);   // fill buffer's data store
	//glBindVertexArray(0);                                                                             // unbind vertex array object

	mIndexCount = size;

	mIndexed = true;
}

void StaticMesh::Bind() const
{
	for (unsigned int i = 0; i < mVBOs.Size(); i++)
		glEnableVertexArrayAttrib(mVAO, i);    //glDisableVertexArrayAttrib(VAO, i);      

	glBindVertexArray(mVAO);      // bind vertex array object

	//glBindVertexArray(mVAO);      

	//for (unsigned int i = 0; i < VBOs.Size(); i++)
	//	glEnableVertexAttribArray(i);  //glDisableVertexAttribArray(i);
}

void StaticMesh::BindVertexAttribute()
{

}

// #GFX_API_TODO: draw different primitives
void StaticMesh::DrawTriangles() const
{
	if (!mIndexed)
		glDrawArrays(GL_TRIANGLES, 0, mVertexCount);
	else
		glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_INT, 0);
		//glDrawElementsBaseVertex(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 0);
}

void StaticMesh::DrawPoints() const
{
	if (!mIndexed)
		glDrawArrays(GL_POINTS, 0, mVertexCount);
	else
		glDrawElements(GL_POINTS, mIndexCount, GL_UNSIGNED_INT, 0);
		//glDrawElementsBaseVertex(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 0);
}

void StaticMesh::DrawLines(bool strip) const
{
	if (!strip)
	{
		if (!mIndexed)
			glDrawArrays(GL_LINES, 0, mVertexCount);
		else
			glDrawElements(GL_LINES, mIndexCount, GL_UNSIGNED_INT, 0);
	}
	else
		if (!mIndexed)
			glDrawArrays(GL_LINE_STRIP, 0, mVertexCount);
		else
			glDrawElements(GL_LINE_STRIP, mIndexCount, GL_UNSIGNED_INT, 0);
			//glDrawElementsBaseVertex(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 0);
}

void StaticMesh::Clear()
{ 
	glDeleteBuffers(mVBOs.Size(), mVBOs.Data());
	mVBOs.Clear();
	mAttributeMap.clear();

	mVertexCount = 0;
}

void StaticMesh::RemoveVertexAttribute(const std::string &attributeName) 
{ 
	glDeleteBuffers(1, &mAttributeMap[attributeName]); 
	mVBOs.Remove(mAttributeMap[attributeName]); 
	mAttributeMap.erase(attributeName); 
}