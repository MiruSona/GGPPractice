#include "Shader.h"
#include <fstream>
#include <sstream>
#include <SDL2/SDL.h>

Shader::Shader() :
	mProgramID(0)
{

}

Shader::~Shader() {
	//쉐이더 프로그램 삭제
	if (mLoad) glDeleteProgram(mProgramID);
}

//쉐이더 로드 및 컴파일해서 프로그램 생성
bool Shader::Load(const char* _vertex_path, const char* _fragment_path) {
	GLuint vertex_id, fragment_id;

	//쉐이더 컴파일 및 id 받기
	if (!CompileShader(_vertex_path, GL_VERTEX_SHADER, vertex_id))
		return false;
	if (!CompileShader(_fragment_path, GL_FRAGMENT_SHADER, fragment_id))
		return false;

	//프로그램 생성 및 id 받기
	mProgramID = glCreateProgram();
	//Vertex & Fragment 쉐이더 링크
	glAttachShader(mProgramID, vertex_id);
	glAttachShader(mProgramID, fragment_id);
	glLinkProgram(mProgramID);

	//프로그램 링크 확인
	if (!CheckErrors(mProgramID, GL_PROGRAM))
		return false;

	//다 됬으면 쉐이더 삭제
	//프로그램에 링크되어있어서 괜찮음
	glDeleteShader(vertex_id);
	glDeleteShader(fragment_id);

	//다 괜찮으면 true
	mLoad = true;
	return true;
}

//쉐이더 컴파일
bool Shader::CompileShader
(const std::string& _path, GLenum _shader_type, GLuint& _out_id) {

	//읽은 내용 저장용
	std::string code;
	//쉐이더 파일 읽기용
	std::ifstream shader_file;
	//에러 체크 설정
	shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		//파일 열기
		shader_file.open(_path);
		//파일 읽기
		std::stringstream sstream;
		sstream << shader_file.rdbuf();
		shader_file.close();
		//코드 저장
		code = sstream.str();
	}
	//오류 있을 경우
	catch (std::ifstream::failure& e) {
		SDL_Log("Shader Compile - FileStream Error : %s", e.what());
		return false;
	}

	//const char*로 변환
	const char* code_c = code.c_str();

	//쉐이더 생성(id 받기)
	_out_id = glCreateShader(_shader_type);
	//쉐이더 데이터 넣기 & 컴파일
	glShaderSource(_out_id, 1, &code_c, nullptr);
	glCompileShader(_out_id);

	//컴파일 에러 체크
	if (!CheckErrors(_out_id, _shader_type))
		return false;

	//다 괜찮으면 true
	return true;
}

//에러 체크
bool Shader::CheckErrors(GLuint _id, GLenum _type) {
	//컴파일 상태 확인
	GLint status = GL_FALSE;
	switch (_type)
	{
	case GL_VERTEX_SHADER:
	case GL_FRAGMENT_SHADER:
		glGetShaderiv(_id, GL_COMPILE_STATUS, &status);
		break;
	case GL_PROGRAM:
		glGetProgramiv(_id, GL_LINK_STATUS, &status);
		break;
	default:
		SDL_Log("Please input correct type");
		return false;
	}

	//컴파일 상태가 true가 아니면 에러메세지 받아 출력
	if (status != GL_TRUE) {
		//512짜리 버퍼에 컴파일 에러내역 받음
		char error_buffer[512];
		memset(error_buffer, 0, 512);

		//타입(쉐이더인지 프로그램인지)에 따라 에러 출력
		switch (_type)
		{
		case GL_VERTEX_SHADER:
		case GL_FRAGMENT_SHADER:
			glGetShaderInfoLog(_id, 511, nullptr, error_buffer);
			SDL_Log("Shader Compile Failed : \n%s", error_buffer);
			break;
		case GL_PROGRAM:
			glGetProgramInfoLog(_id, 511, nullptr, error_buffer);
			SDL_Log("Shader Program Link Error : \n%s", error_buffer);
			break;
		default:
			SDL_Log("Please input correct type");
			break;
		}
		return false;
	}

	//그 외에는 정상
	return true;
}

//프로그램 사용
void Shader::Active() {
	glUseProgram(mProgramID);
}

//============ 유니폼 함수들 ============

void Shader::SetBool(const std::string& _name, bool _value) const {
	//이름으로 uniform 찾기
	GLuint loc = glGetUniformLocation(mProgramID, _name.c_str());
	//uniform에 데이터 보내기
	glUniform1i(loc, _value);
}
void Shader::SetInt(const std::string& _name, int _value) const {
	//이름으로 uniform 찾기
	GLuint loc = glGetUniformLocation(mProgramID, _name.c_str());
	//uniform에 데이터 보내기
	glUniform1i(loc, _value);
}
void Shader::SetFloat(const std::string& _name, float _value) const {
	//이름으로 uniform 찾기
	GLuint loc = glGetUniformLocation(mProgramID, _name.c_str());
	//uniform에 데이터 보내기
	glUniform1f(loc, _value);
}

void Shader::SetVec2(const std::string& _name, const glm::vec2& _value) const {
	//이름으로 uniform 찾기
	GLuint loc = glGetUniformLocation(mProgramID, _name.c_str());
	//uniform에 데이터 보내기
	glUniform2fv(loc, 1, &_value[0]);
}
void Shader::SetVec2(const std::string& _name, float _x, float _y) const {
	//이름으로 uniform 찾기
	GLuint loc = glGetUniformLocation(mProgramID, _name.c_str());
	//uniform에 데이터 보내기
	glUniform2f(loc, _x, _y);
}

void Shader::SetVec3(const std::string& _name, const glm::vec3& _value) const {
	//이름으로 uniform 찾기
	GLuint loc = glGetUniformLocation(mProgramID, _name.c_str());
	//uniform에 데이터 보내기
	glUniform3fv(loc, 1, &_value[0]);
}
void Shader::SetVec3(const std::string& _name, float _x, float _y, float _z) const {
	//이름으로 uniform 찾기
	GLuint loc = glGetUniformLocation(mProgramID, _name.c_str());
	//uniform에 데이터 보내기
	glUniform3f(loc, _x, _y, _z);
}

void Shader::SetVec4(const std::string& _name, const glm::vec4& _value) const {
	//이름으로 uniform 찾기
	GLuint loc = glGetUniformLocation(mProgramID, _name.c_str());
	//uniform에 데이터 보내기
	glUniform4fv(loc, 1, &_value[0]);
}
void Shader::SetVec4(const std::string& _name, float _x, float _y, float _z, float _w) const {
	//이름으로 uniform 찾기
	GLuint loc = glGetUniformLocation(mProgramID, _name.c_str());
	//uniform에 데이터 보내기
	glUniform4f(loc, _x, _y, _z, _w);
}

void Shader::SetMat2(const std::string& _name, const glm::mat2& _mat) const {
	//이름으로 uniform 찾기
	GLuint loc = glGetUniformLocation(mProgramID, _name.c_str());
	//uniform에 데이터 보내기
	glUniformMatrix2fv(loc, 1, GL_TRUE, &_mat[0][0]);
}
void Shader::SetMat3(const std::string& _name, const glm::mat3& _mat) const {
	//이름으로 uniform 찾기
	GLuint loc = glGetUniformLocation(mProgramID, _name.c_str());
	//uniform에 데이터 보내기
	glUniformMatrix3fv(loc, 1, GL_TRUE, &_mat[0][0]);
}
void Shader::SetMat4(const std::string& _name, const glm::mat4& _mat) const {
	//이름으로 uniform 찾기
	GLuint loc = glGetUniformLocation(mProgramID, _name.c_str());
	//uniform에 데이터 보내기
	glUniformMatrix4fv(loc, 1, GL_TRUE, &_mat[0][0]);
}