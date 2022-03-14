#include "Shader.h"
#include <fstream>
#include <sstream>
#include <SDL2/SDL.h>

Shader::Shader() :
	mProgramID(0)
{

}

Shader::~Shader() {
	//���̴� ���α׷� ����
	if (mLoad) glDeleteProgram(mProgramID);
}

//���̴� �ε� �� �������ؼ� ���α׷� ����
bool Shader::Load(const char* _vertex_path, const char* _fragment_path) {
	GLuint vertex_id, fragment_id;

	//���̴� ������ �� id �ޱ�
	if (!CompileShader(_vertex_path, GL_VERTEX_SHADER, vertex_id))
		return false;
	if (!CompileShader(_fragment_path, GL_FRAGMENT_SHADER, fragment_id))
		return false;

	//���α׷� ���� �� id �ޱ�
	mProgramID = glCreateProgram();
	//Vertex & Fragment ���̴� ��ũ
	glAttachShader(mProgramID, vertex_id);
	glAttachShader(mProgramID, fragment_id);
	glLinkProgram(mProgramID);

	//���α׷� ��ũ Ȯ��
	if (!CheckErrors(mProgramID, GL_PROGRAM))
		return false;

	//�� ������ ���̴� ����
	//���α׷��� ��ũ�Ǿ��־ ������
	glDeleteShader(vertex_id);
	glDeleteShader(fragment_id);

	//�� �������� true
	mLoad = true;
	return true;
}

//���̴� ������
bool Shader::CompileShader
(const std::string& _path, GLenum _shader_type, GLuint& _out_id) {

	//���� ���� �����
	std::string code;
	//���̴� ���� �б��
	std::ifstream shader_file;
	//���� üũ ����
	shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		//���� ����
		shader_file.open(_path);
		//���� �б�
		std::stringstream sstream;
		sstream << shader_file.rdbuf();
		shader_file.close();
		//�ڵ� ����
		code = sstream.str();
	}
	//���� ���� ���
	catch (std::ifstream::failure& e) {
		SDL_Log("Shader Compile - FileStream Error : %s", e.what());
		return false;
	}

	//const char*�� ��ȯ
	const char* code_c = code.c_str();

	//���̴� ����(id �ޱ�)
	_out_id = glCreateShader(_shader_type);
	//���̴� ������ �ֱ� & ������
	glShaderSource(_out_id, 1, &code_c, nullptr);
	glCompileShader(_out_id);

	//������ ���� üũ
	if (!CheckErrors(_out_id, _shader_type))
		return false;

	//�� �������� true
	return true;
}

//���� üũ
bool Shader::CheckErrors(GLuint _id, GLenum _type) {
	//������ ���� Ȯ��
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

	//������ ���°� true�� �ƴϸ� �����޼��� �޾� ���
	if (status != GL_TRUE) {
		//512¥�� ���ۿ� ������ �������� ����
		char error_buffer[512];
		memset(error_buffer, 0, 512);

		//Ÿ��(���̴����� ���α׷�����)�� ���� ���� ���
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

	//�� �ܿ��� ����
	return true;
}

//���α׷� ���
void Shader::Active() {
	glUseProgram(mProgramID);
}

//============ ������ �Լ��� ============

void Shader::SetBool(const std::string& _name, bool _value) const {
	//�̸����� uniform ã��
	GLuint loc = glGetUniformLocation(mProgramID, _name.c_str());
	//uniform�� ������ ������
	glUniform1i(loc, _value);
}
void Shader::SetInt(const std::string& _name, int _value) const {
	//�̸����� uniform ã��
	GLuint loc = glGetUniformLocation(mProgramID, _name.c_str());
	//uniform�� ������ ������
	glUniform1i(loc, _value);
}
void Shader::SetFloat(const std::string& _name, float _value) const {
	//�̸����� uniform ã��
	GLuint loc = glGetUniformLocation(mProgramID, _name.c_str());
	//uniform�� ������ ������
	glUniform1f(loc, _value);
}

void Shader::SetVec2(const std::string& _name, const glm::vec2& _value) const {
	//�̸����� uniform ã��
	GLuint loc = glGetUniformLocation(mProgramID, _name.c_str());
	//uniform�� ������ ������
	glUniform2fv(loc, 1, &_value[0]);
}
void Shader::SetVec2(const std::string& _name, float _x, float _y) const {
	//�̸����� uniform ã��
	GLuint loc = glGetUniformLocation(mProgramID, _name.c_str());
	//uniform�� ������ ������
	glUniform2f(loc, _x, _y);
}

void Shader::SetVec3(const std::string& _name, const glm::vec3& _value) const {
	//�̸����� uniform ã��
	GLuint loc = glGetUniformLocation(mProgramID, _name.c_str());
	//uniform�� ������ ������
	glUniform3fv(loc, 1, &_value[0]);
}
void Shader::SetVec3(const std::string& _name, float _x, float _y, float _z) const {
	//�̸����� uniform ã��
	GLuint loc = glGetUniformLocation(mProgramID, _name.c_str());
	//uniform�� ������ ������
	glUniform3f(loc, _x, _y, _z);
}

void Shader::SetVec4(const std::string& _name, const glm::vec4& _value) const {
	//�̸����� uniform ã��
	GLuint loc = glGetUniformLocation(mProgramID, _name.c_str());
	//uniform�� ������ ������
	glUniform4fv(loc, 1, &_value[0]);
}
void Shader::SetVec4(const std::string& _name, float _x, float _y, float _z, float _w) const {
	//�̸����� uniform ã��
	GLuint loc = glGetUniformLocation(mProgramID, _name.c_str());
	//uniform�� ������ ������
	glUniform4f(loc, _x, _y, _z, _w);
}

void Shader::SetMat2(const std::string& _name, const glm::mat2& _mat) const {
	//�̸����� uniform ã��
	GLuint loc = glGetUniformLocation(mProgramID, _name.c_str());
	//uniform�� ������ ������
	glUniformMatrix2fv(loc, 1, GL_TRUE, &_mat[0][0]);
}
void Shader::SetMat3(const std::string& _name, const glm::mat3& _mat) const {
	//�̸����� uniform ã��
	GLuint loc = glGetUniformLocation(mProgramID, _name.c_str());
	//uniform�� ������ ������
	glUniformMatrix3fv(loc, 1, GL_TRUE, &_mat[0][0]);
}
void Shader::SetMat4(const std::string& _name, const glm::mat4& _mat) const {
	//�̸����� uniform ã��
	GLuint loc = glGetUniformLocation(mProgramID, _name.c_str());
	//uniform�� ������ ������
	glUniformMatrix4fv(loc, 1, GL_TRUE, &_mat[0][0]);
}