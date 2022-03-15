#include "VertexArray.h"
#include "Common.h"
#include <GL/glew.h>
#include <SDL2/SDL.h>

//생성/소멸자
VertexArray::VertexArray(const void* _vertices, GLuint _num_vertices,
	const GLuint* _indices, GLuint _num_indices, bool _is_sprite) :
	mNumVertices(_num_vertices),
	mNumIndices(_num_indices)
{
	//Vertex Array 생성
	glGenVertexArrays(1, &mVertexArray);
	//바인딩 후 ID 생성해서 매개변수에 넘겨줌
	//즉 Vertex Array의 ID 생성!
	glBindVertexArray(mVertexArray);

	//VertexSize 설정
	//GLuint vertex_size = 8 * size_f;
	GLuint vertex_size = 5 * size_f;

	//Vertex Buffer 생성
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	//버퍼 데이터 넣기
	//GL_STATIC_DRAW : 데이터를 한번만 로드 -> 자주 그릴 경우 사용
	glBufferData(
		GL_ARRAY_BUFFER,				//데이터를 쓸 버퍼의 버퍼 타입
		_num_vertices * vertex_size,	//복사할 바이트 크기
		_vertices,						//복사할 데이터(포인터)
		GL_STATIC_DRAW);				//이 데이터 용도

	//Index Buffer 생성
	if (_num_indices != -1) {
		glGenBuffers(1, &mIndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			_num_indices * size_ui,
			_indices,
			GL_STATIC_DRAW);
	}

	//Vertex Shader 속성들 설정 = Vertex Shader Layout 설정
	//0 번째 Vertex 속성 설정 = 위치 좌표
	glEnableVertexAttribArray(0);
	//속성 포멧 설정
	glVertexAttribPointer(
		0,					//속성 인덱스
		3,					//요소의 수(여기선 x,y,z 3개)
		GL_FLOAT,			//요소의 타입
		GL_FALSE,			//(정수형만 사용)
		vertex_size,		//간격(각 버텍스의 크기)
		0					//버텍스의 시작에서 이 속성까지의 오프셋
	);
	//Sprite 스프라이트 설정
	if (_is_sprite) {
		//1 번째 Vertex 속성 설정 = 텍스쳐 좌표
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertex_size,
			reinterpret_cast<void*>(size_f * 3));
	}
	//메쉬 설정
	else {
		//1 번째 Vertex 속성 설정 = 텍스쳐 좌표
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertex_size,
			reinterpret_cast<void*>(size_f * 3));
	}
}
VertexArray::~VertexArray() {
	//모든 버퍼 제거
	glDeleteBuffers(1, &mVertexBuffer);
	if(mNumIndices != -1)
		glDeleteBuffers(1, &mIndexBuffer);
	glDeleteBuffers(1, &mVertexArray);
}

//바인딩
void VertexArray::Active() {
	glBindVertexArray(mVertexArray);
}