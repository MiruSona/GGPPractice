
//OpenGL 버전
#version 330

//입력 속성값
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_tex_coord;

//fragment 쉐이더에 보내는 값
out vec3 our_color;
out vec2 tex_coord;

//유니폼 입력값
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//메인
void main(){
	gl_Position = projection * view * model * vec4(in_pos, 1.0f);
	tex_coord = in_tex_coord;
}