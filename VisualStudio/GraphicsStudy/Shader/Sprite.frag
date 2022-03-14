
//����
#version 330

//���� ��
out vec4 frag_color;

//vertex ���̴��� ���� ������
in vec3 our_color;
in vec2 tex_coord;

uniform float mix_value;

//�ؽ��� ������(���ε��� �ؽ��ĸ� ������)
//Texture0, 1, 2 �̷��� ������� ������
uniform sampler2D our_texture1;
uniform sampler2D our_texture2;

void main(){
	vec2 new_coord = vec2(tex_coord.x, 1.0 - tex_coord.y);
	frag_color = mix(texture(our_texture1, tex_coord), texture(our_texture2, new_coord), mix_value);
}