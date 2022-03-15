
//����
#version 330

//���� ��
out vec4 frag_color;

//vertex ���̴��� ���� ������
in vec2 tex_coord;

//�ؽ��� ������(���ε��� �ؽ��ĸ� ������)
//Texture0, 1, 2 �̷��� ������� ������
uniform sampler2D our_texture1;
uniform sampler2D our_texture2;

void main(){
	frag_color = mix(texture(our_texture1, tex_coord), texture(our_texture2, tex_coord), 0.2f);
}