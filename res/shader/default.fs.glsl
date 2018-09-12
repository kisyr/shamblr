#version 330

uniform sampler2D u_Sampler;
in vec2 fs_TexCoord;
out vec4 fs_Color;

void main() {
	fs_Color = vec4(1);
	fs_Color += texture2D(u_Sampler, fs_TexCoord);
}
