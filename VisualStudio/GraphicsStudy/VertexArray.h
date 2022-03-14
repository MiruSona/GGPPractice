#pragma once
#include <GL/glew.h>

class VertexArray {
private:
	/*
	Vertex : 점
	indices : index의 복수형 = 색인들
	mNumVerts : Vertex Buffer에 들어온 vertex 수
	mNumIndices : Index Buffer에 들어온 index 수
	mVertexBuffer : Vertex Buffer의 OpenGL ID
	mIndexBuffer : Index Buffer의 OpenGL ID
	mVertexArray : Vertex Array 객체의 OpenGL ID
	*/
	GLuint mNumVertices, mNumIndices;
	GLuint mVertexBuffer, mIndexBuffer;
	GLuint mVertexArray;

public:
	//생성/소멸자
	VertexArray(const void* _vertices, GLuint _num_vertices,
		const GLuint* _indices, GLuint _num_indices, bool _is_sprite = false);
	~VertexArray();

	//활성화(바인딩)
	void Active();

	//Get
	unsigned int GetNumIndices() const { return mNumIndices; }
	unsigned int GetNumVertices() const { return mNumVertices; }
};