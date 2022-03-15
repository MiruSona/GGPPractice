
//버전
#version 330

//최종 색
out vec4 frag_color;

//vertex 쉐이더로 부터 받은값
in vec2 tex_coord;

//텍스쳐 데이터(바인딩한 텍스쳐를 가져옴)
//Texture0, 1, 2 이렇게 순서대로 가져옴
uniform sampler2D our_texture1;
uniform sampler2D our_texture2;

void main(){
	frag_color = mix(texture(our_texture1, tex_coord), texture(our_texture2, tex_coord), 0.2f);
}