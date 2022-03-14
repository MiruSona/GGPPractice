#pragma once
#include <GL/glew.h>
#include <string>
#include <GLM/glm.hpp>

class Shader {
private:
	//ID 저장용(프로그램 ID)
	GLuint mProgramID;

	//로드 여부
	bool mLoad = false;

	//쉐이더 컴파일
	bool CompileShader(const std::string& _path, GLenum _shader_type, GLuint& _out_id);

	//에러 체크
	bool CheckErrors(GLuint _id, GLenum _type);

public:
	Shader();
	~Shader();

	//쉐이더 로드 및 컴파일해서 프로그램 생성
	bool Load(const char* _vertex_path, const char* _fragment_path);

	//프로그램 사용
	void Active();

	//Uniform 설정 함수들
	void SetBool(const std::string& _name, bool _value) const;
	void SetInt(const std::string& _name, int _value) const;
	void SetFloat(const std::string& _name, float _value) const;

	void SetVec2(const std::string& _name, const glm::vec2& _value) const;
	void SetVec2(const std::string& _name, float _x, float _y) const;

	void SetVec3(const std::string& _name, const glm::vec3& _value) const;
	void SetVec3(const std::string& _name, float _x, float _y, float _z) const;

	void SetVec4(const std::string& _name, const glm::vec4& _value) const;
	void SetVec4(const std::string& _name, float _x, float _y, float _z, float _w) const;

	void SetMat2(const std::string& _name, const glm::mat2& _mat) const;
	void SetMat3(const std::string& _name, const glm::mat3& _mat) const;
	void SetMat4(const std::string& _name, const glm::mat4& _mat) const;
};