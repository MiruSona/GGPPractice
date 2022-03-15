
//OpenGL ����
#version 330

//�Է� �Ӽ���
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_tex_coord;

//fragment ���̴��� ������ ��
out vec3 our_color;
out vec2 tex_coord;

//������ �Է°�
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//����
void main(){
	gl_Position = projection * view * model * vec4(in_pos, 1.0f);
	tex_coord = in_tex_coord;
}