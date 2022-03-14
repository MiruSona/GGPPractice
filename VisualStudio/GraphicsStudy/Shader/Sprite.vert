
//OpenGL 버전
#version 330

//입력 속성값
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_color;
layout (location = 2) in vec2 in_tex_coord;

//fragment 쉐이더에 보내는 값
out vec3 our_color;
out vec2 tex_coord;

//메인
void main(){
	gl_Position = vec4(in_pos, 1.0);
	our_color = in_color;
	tex_coord = in_tex_coord;
}